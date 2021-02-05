#include "StegUtils.h"
#include "JpegUtils.h"
#include <time.h>
#include <iostream>
#include <math.h>

#define COEFFICIENT_MAX 32765
#define COEFFICIENT_MIN -32765
#define CSTEG_JPEG_VERSION 1
#define CSTEG_JPEG_VERSION_1_HEADER_LENGTH 51 //16 + 32 + 3
#define VERSION_BIT_LENGTH 16
#define DATA_LENGTH_BIT_LENGTH 32
#define BITS_TO_STEAL_BIT_LENGTH 3

short StegUtils::getLosslessEncodedShort(short coef, short coefBits, short dataBits) {
    if (coefBits != dataBits) {
        if (coef >= COEFFICIENT_MAX) {
            --coef;
        } else if (coef <= COEFFICIENT_MIN) {
            ++coef;
        } else {
            switch (coef) {
            case 1:
                ++coef;
                break;
            case -1:
                --coef;
                break;
            default:
                if (rand() % 2 == 1) {
                    ++coef;
                } else {
                    --coef;
                }
            }
        }
    }

    return coef;
}

bool StegUtils::encodeLosslessJpegFile(const char *inputFileName, const char* outputFileName,
		const unsigned char* const data, int dataLength, unsigned short numberOfBitsToSteal, int quality) {
	JpegCoefficients inputJpeg;
	if (!JpegUtils::readJpegCoefficients(inputFileName, inputJpeg)) {
		std::cout << "error loading jpeg file: " << inputFileName << std::endl;
		return false;
	}
  clock_t runtime = clock();
	// -------------------------------------------------------------------------------------------------
	// begin LSB methodology
	// loop over the coefficients and replace LSBs with data bits
	// -------------------------------------------------------------------------------------------------
	int numberOfCoefficients = 0;
	int dataByteIndex = 0;
	int dataBitIndex = 0;
	int coef = 0;
	int coefBits = 0;
	int dataBits = 0;
	int dataMask = (int) (pow((float) 2, numberOfBitsToSteal) - 1);
	int lengthPayloadBitCount = 0;
	int versionPayloadBitCount = 0;
	int numberOfBitsToStealPayloadCount = 0;
	int lengthPayload = dataLength;
	int dataLengthMinusOne = dataLength - 1;
	unsigned short shortBuf;
	bool headerComplete = false;
	int encodeBitCount = 0;
	bool stop = false;
	time_t t;
	srand((unsigned) time(&t));
	for (JDIMENSION compnum = 0; compnum < inputJpeg.numComponents; ++compnum) {
		if (stop) {
			break;
		}

		for (JDIMENSION rownum = 0; rownum < inputJpeg.heightInBlocks[compnum]; ++rownum) {
			if (stop) {
				break;
			}

			for (JDIMENSION blocknum = 0; blocknum < inputJpeg.widthInBlocks[compnum]; ++blocknum) {
				if (stop) {
					break;
				}
				for (JDIMENSION i = 0; i < DCTSIZE2; ++i) {
					coef = inputJpeg.coefBuffers[compnum][rownum][blocknum][i];
					if (coef != 0) {
						if (headerComplete) {
							if (dataByteIndex < dataLength) {
								shortBuf = data[dataByteIndex];
								if (dataByteIndex < dataLengthMinusOne) {
									shortBuf = (shortBuf << 8) | data[dataByteIndex + 1];
								} else {
									shortBuf = shortBuf << 8;
								}

								dataBits = (shortBuf >> (16 - (dataBitIndex + numberOfBitsToSteal))) & dataMask;
								coef = (coef >> numberOfBitsToSteal << numberOfBitsToSteal) | dataBits;
								if (coef == 0) {
									coef = coef | (1 << numberOfBitsToSteal);
								}

								dataBitIndex += numberOfBitsToSteal;
								if (dataBitIndex >= 8) {
									dataBitIndex %= 8;
									++dataByteIndex;
								}

								inputJpeg.coefBuffers[compnum][rownum][blocknum][i] = coef;
							} else {
								stop = true;
								break;
							}
						} else {
							if (versionPayloadBitCount < VERSION_BIT_LENGTH) { // 2 bytes
								coefBits = coef & 1;
								dataBits = (CSTEG_JPEG_VERSION >> versionPayloadBitCount) & 1;

								inputJpeg.coefBuffers[compnum][rownum][blocknum][i] =
										StegUtils::getLosslessEncodedShort(coef, coefBits, dataBits);
								++versionPayloadBitCount;
							} else if (lengthPayloadBitCount < DATA_LENGTH_BIT_LENGTH) { // 4 bytes
								coefBits = coef & 1;
								dataBits = (lengthPayload >> lengthPayloadBitCount) & 1;

								inputJpeg.coefBuffers[compnum][rownum][blocknum][i] =
										StegUtils::getLosslessEncodedShort(coef, coefBits, dataBits);
								++lengthPayloadBitCount;
							} else if (numberOfBitsToStealPayloadCount < BITS_TO_STEAL_BIT_LENGTH) {
								coefBits = coef & 1;
								dataBits = (numberOfBitsToSteal >> numberOfBitsToStealPayloadCount) & 1;

								inputJpeg.coefBuffers[compnum][rownum][blocknum][i] =
										StegUtils::getLosslessEncodedShort(coef, coefBits, dataBits);
								++numberOfBitsToStealPayloadCount;
							} else {
								headerComplete = true;
							}

							++encodeBitCount;
						}

						++numberOfCoefficients;
					}
				}
			}
		}
	}

  printf("dataByteIndex : %d \n" , dataByteIndex);
  double duration = (double)(clock() - runtime) / CLOCKS_PER_SEC * 1000;
  printf("decode time : %lf \n" , duration);

	return JpegUtils::writeJpegCoefficients(inputJpeg, outputFileName, quality);
}

bool StegUtils::decodeLosslessJpegFile(const char *inputFileName, DecodeResponse& decodeResponse) {
	JpegCoefficients inputJpeg;
	if (!JpegUtils::readJpegCoefficients(inputFileName, inputJpeg)) {
		std::cout << "error loading jpeg file: " << inputFileName << std::endl;
		return false;
	}

	// -------------------------------------------------------------------------------------------
	// begin LSB decoding
	// -------------------------------------------------------------------------------------------
	int dataByteIndex = 0;
	int coef;
	int bufferLength = 0;
	unsigned char* buffer = NULL;
	unsigned int decodeBitCount = 0;
	unsigned char coefBit = 0;
	int coefficient_mask = 0;
	int intBuff = 1;
	int intBuffCount = 0;
	int dataBits = 0;
	bool stop = false;
	int numberOfBitsToSteal = 0;
	bool headerComplete = false;
	int versionPayloadBitCount = 0;
	int numberOfBitsToStealPayloadCount = 0;
	int lengthPayloadBitCount = 0;
	int version = 0;
	for (JDIMENSION compnum = 0; compnum < inputJpeg.numComponents; ++compnum) {
		if (stop) {
			break;
		}
		for (JDIMENSION rownum = 0; rownum < inputJpeg.heightInBlocks[compnum]; ++rownum) {
			if (stop) {
				break;
			}
			for (JDIMENSION blocknum = 0; blocknum < inputJpeg.widthInBlocks[compnum]; ++blocknum) {
				if (stop) {
					break;
				}
				for (JDIMENSION i = 0; i < DCTSIZE2; ++i) {
					coef = inputJpeg.coefBuffers[compnum][rownum][blocknum][i];
					if (coef != 0) {
						if (headerComplete) {
							if (dataByteIndex < bufferLength) {
								dataBits = coef & coefficient_mask;
								intBuff = (intBuff << numberOfBitsToSteal) | dataBits;
								intBuffCount += numberOfBitsToSteal;
								if (intBuffCount >= 8) {
									intBuffCount -= 8;
									buffer[dataByteIndex] = ((intBuff >> intBuffCount) & 255);
									++dataByteIndex;
								}
							} else {
								stop = true;
								break;
							}
						} else {
							if (versionPayloadBitCount < VERSION_BIT_LENGTH) {
								coefBit = coef & 1;

								version = version | (coefBit << versionPayloadBitCount);
								++versionPayloadBitCount;
							} else if (lengthPayloadBitCount < DATA_LENGTH_BIT_LENGTH) {
								coefBit = coef & 1;

								bufferLength = bufferLength | (coefBit << lengthPayloadBitCount);
								++lengthPayloadBitCount;
							} else if (numberOfBitsToStealPayloadCount < BITS_TO_STEAL_BIT_LENGTH) {
								//encode the number of bits we stole when encoding this image
								coefBit = coef & 1;

								numberOfBitsToSteal = numberOfBitsToSteal | (coefBit << numberOfBitsToStealPayloadCount);
								++numberOfBitsToStealPayloadCount;
							} else {
								headerComplete = true;
								buffer = (unsigned char*) malloc(bufferLength);
								memset(buffer, 0, bufferLength);
								coefficient_mask = (int) pow((float) 2, numberOfBitsToSteal) - 1;
							}

							++decodeBitCount;
						}
					}
				}
			}
		}
	}

	decodeResponse.data = buffer;
	decodeResponse.length = bufferLength;

	return true;
}

void StegUtils::doCompressions(int n, const char* filePath, int quality) {
	char file_path_buffer[256];
	Utils::getFileNameWithoutExtension(filePath, file_path_buffer);

	char file_name_buffer_1[256];
	char file_name_buffer_2[256];
	memset(file_name_buffer_1, 0, 256);

	char* ptr_1 = file_name_buffer_1;
	char* ptr_2 = file_name_buffer_2;

	strcat(ptr_1, filePath);
	Image image;

	for (int i = 0; i < n; ++i) {
		memset(ptr_2, 0, 256);
		sprintf(ptr_2, "%s-%d.jpeg", file_path_buffer, i);

		JpegUtils::readJpegFile(image, ptr_1);
		JpegUtils::writeJpegFile(image, ptr_2, quality);

		char* temp = ptr_1;
		ptr_1 = ptr_2;
		ptr_2 = temp;
	}
}


int StegUtils::doCoefficientDiff(const char* filePath1, const char* filePath2) {
	JpegCoefficients coefficients1;
	JpegUtils::readJpegCoefficients(filePath1, coefficients1);
	JpegCoefficients coefficients2;
	JpegUtils::readJpegCoefficients(filePath2, coefficients2);

	return StegUtils::diffCoefficients(coefficients1, coefficients2);
}


void StegUtils::purge(const char* jpegFilePath, int limit) {
	char jpegPathBuffer[256];
	Utils::getFileNameWithoutExtension(jpegFilePath, jpegPathBuffer);

	char filePathBuffer[256];
	for (int i = 0; i < limit; ++i) {
		memset(filePathBuffer, 0, 256);
		sprintf(filePathBuffer, "%s-%d.jpeg", jpegPathBuffer, i);
		remove(filePathBuffer);
	}
}


unsigned int StegUtils::getNumberOfCoefficients(const char *inputFileName) {
	JpegCoefficients inputJpeg;
	if (!JpegUtils::readJpegCoefficients(inputFileName, inputJpeg)) {
		printf("error loading jpeg file %s\n!", inputFileName);
		return -1;
	}

	int number_of_coefficients = 0;
	short coef = 0;

	for (JDIMENSION compnum = 0; compnum < inputJpeg.numComponents; ++compnum) {
		for (JDIMENSION rownum = 0; rownum < inputJpeg.heightInBlocks[compnum]; ++rownum) {
			for (JDIMENSION blocknum = 0; blocknum < inputJpeg.widthInBlocks[compnum]; ++blocknum) {
				for (JDIMENSION i = 0; i < DCTSIZE2; ++i) {
					coef = inputJpeg.coefBuffers[compnum][rownum][blocknum][i];

					// we only use non-zero coefficients
					if (coef != 0) {
						++number_of_coefficients;
					}
				}
			}
		}
	}

	return number_of_coefficients;
}


int StegUtils::diffCoefficients(JpegCoefficients& coefficients1, JpegCoefficients& coefficients2) {
	int diff = 0;
	int count = 0;
	int num_components = coefficients1.info->num_components;
	size_t block_row_size[num_components];
	int width_in_blocks[num_components];
	int height_in_blocks[num_components];
	int x = 0;
	int y = 0;
	for (JDIMENSION compnum = 0; compnum < coefficients1.info->num_components; ++compnum) {
		for (JDIMENSION rownum = 0; rownum < height_in_blocks[compnum]; ++rownum) {
			for (JDIMENSION blocknum = 0; blocknum < width_in_blocks[compnum]; blocknum++) {
				x = 0;
				y = 0;
				for (JDIMENSION i = 0; i < DCTSIZE2; i++) {
					x = (x + 1) % DCTSIZE;
					if (x == 0) {
						++y;
					}

					int val1 = coefficients1.coefBuffers[compnum][rownum][blocknum][i];
					int val2 = coefficients2.coefBuffers[compnum][rownum][blocknum][i];

					if (val1 != val2) {
						std::cout << val1 << " != " << val2 << ", for " << x << ", " << y << " at rowblock "
								<< rownum << ", " << blocknum << "\n" << std::endl;
						++diff;
					}
				}
			}
		}
	}

	std::cout << "number of diffs = " << diff << std::endl;

	return diff;
}

void StegUtils::encodeMessageToJpeg(const char *inputFileName, const char* outputFileName,
                                    const unsigned char* const data, int dataLength, unsigned short numberOfBitsToSteal) {
  JpegCoefficients coefficients;
  struct jpeg_error_mgr jerr;
  JDIMENSION i, rownum, blocknum;
  JBLOCKARRAY* coef_buffers = new JBLOCKARRAY[MAX_COMPONENTS];
  JBLOCKARRAY* row_ptrs = new JBLOCKARRAY[MAX_COMPONENTS];

  FILE* jpeg_file = fopen(inputFileName, "rb");
  if (!jpeg_file) {
    printf("error opening jpeg file %s\n!", inputFileName);
    return ;
  }

  struct jpeg_decompress_struct* info =
      (struct jpeg_decompress_struct*) malloc(sizeof(struct jpeg_decompress_struct));
  (*info).err = jpeg_std_error(&jerr);
  jpeg_create_decompress(info);
  jpeg_stdio_src(info, jpeg_file);
  (void) jpeg_read_header(info, TRUE);

  for (JDIMENSION compnum = 0; compnum < (*info).num_components; ++compnum) {
    coef_buffers[compnum] = (info->mem->alloc_barray)((j_common_ptr) info,
                                                      JPOOL_IMAGE, (*info).comp_info[compnum].width_in_blocks,
                                                      (*info).comp_info[compnum].height_in_blocks);
  }

  jvirt_barray_ptr *coef_arrays = jpeg_read_coefficients(info);
  clock_t runtime = clock();

  int numberOfCoefficients = 0;
  int dataByteIndex = 0;
  int dataBitIndex = 0;
  int coef = 0;
  int coefBits = 0;
  int dataBits = 0;
  int dataMask = (int) (pow((float) 2, numberOfBitsToSteal) - 1);
  int lengthPayloadBitCount = 0;
  int versionPayloadBitCount = 0;
  int numberOfBitsToStealPayloadCount = 0;
  int lengthPayload = dataLength;
  int dataLengthMinusOne = dataLength - 1;
  unsigned short shortBuf;
  bool headerComplete = false;
  int encodeBitCount = 0;
  bool stop = false;
  time_t t;
  srand((unsigned) time(&t));

  int num_components = (*info).num_components;
  size_t* block_row_size = new size_t[num_components];
  int* width_in_blocks = new int[num_components];
  int* height_in_blocks = new int[num_components];
  for (JDIMENSION compnum = 0; compnum < num_components; ++compnum) {
    height_in_blocks[compnum] = (*info).comp_info[compnum].height_in_blocks;
    width_in_blocks[compnum] = (*info).comp_info[compnum].width_in_blocks;
    block_row_size[compnum] = (size_t) sizeof(JCOEF) * DCTSIZE2 * width_in_blocks[compnum];
    for (rownum = 0; rownum < height_in_blocks[compnum]; rownum++) {
      row_ptrs[compnum] = (info->mem->access_virt_barray)(
          (j_common_ptr) &info, coef_arrays[compnum], rownum,
          (JDIMENSION) 1, FALSE);
      for (blocknum = 0; blocknum < width_in_blocks[compnum]; blocknum++) {
        for (i = 0; i < DCTSIZE2; i++) {
          coef_buffers[compnum][rownum][blocknum][i] = row_ptrs[compnum][0][blocknum][i];
          if (stop) continue;
          coef = coef_buffers[compnum][rownum][blocknum][i];
          if (coef != 0) {
            if (headerComplete) {
              if (dataByteIndex < dataLength) {
                shortBuf = data[dataByteIndex];
                if (dataByteIndex < dataLengthMinusOne) {
                  shortBuf = (shortBuf << 8) | data[dataByteIndex + 1];
                } else {
                  shortBuf = shortBuf << 8;
                }

                dataBits = (shortBuf >> (16 - (dataBitIndex + numberOfBitsToSteal))) & dataMask;
                coef = (coef >> numberOfBitsToSteal << numberOfBitsToSteal) | dataBits;
                if (coef == 0) {
                  coef = coef | (1 << numberOfBitsToSteal);
                }

                dataBitIndex += numberOfBitsToSteal;
                if (dataBitIndex >= 8) {
                  dataBitIndex %= 8;
                  ++dataByteIndex;
                }

                coef_buffers[compnum][rownum][blocknum][i] = coef;
              } else {
                stop = true;
                continue;
              }
            } else {
              if (versionPayloadBitCount < VERSION_BIT_LENGTH) { // 2 bytes
                coefBits = coef & 1;
                dataBits = (CSTEG_JPEG_VERSION >> versionPayloadBitCount) & 1;

                coef_buffers[compnum][rownum][blocknum][i] =
                    StegUtils::getLosslessEncodedShort(coef, coefBits, dataBits);
                ++versionPayloadBitCount;
              } else if (lengthPayloadBitCount < DATA_LENGTH_BIT_LENGTH) { // 4 bytes
                coefBits = coef & 1;
                dataBits = (lengthPayload >> lengthPayloadBitCount) & 1;

                coef_buffers[compnum][rownum][blocknum][i] =
                    StegUtils::getLosslessEncodedShort(coef, coefBits, dataBits);
                ++lengthPayloadBitCount;
              } else if (numberOfBitsToStealPayloadCount < BITS_TO_STEAL_BIT_LENGTH) {
                //encode the number of bits we stole when encoding this image
                coefBits = coef & 1;
                dataBits = (numberOfBitsToSteal >> numberOfBitsToStealPayloadCount) & 1;

                coef_buffers[compnum][rownum][blocknum][i] =
                    StegUtils::getLosslessEncodedShort(coef, coefBits, dataBits);
                ++numberOfBitsToStealPayloadCount;
              } else {
                headerComplete = true;
              }

              ++encodeBitCount;
            }

            ++numberOfCoefficients;
          }

        }
      }
    }
  }

  double duration = (double)(clock() - runtime) / CLOCKS_PER_SEC * 1000;
  printf("read time: %lf \n" , duration);

  fclose(jpeg_file);

  coefficients.coefBuffers = coef_buffers;
  coefficients.rowPtrs = row_ptrs;
  coefficients.info = info;
  coefficients.numComponents = num_components;
  coefficients.blockRowSize = block_row_size;
  coefficients.widthInBlocks = width_in_blocks;
  coefficients.heightInBlocks = height_in_blocks;
  coefficients.coefArrays = coef_arrays;

  JpegUtils::writeJpegCoefficients(coefficients, outputFileName, 100);
}

