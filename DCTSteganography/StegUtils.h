#ifndef CSTEG_STEG_UTILS_H
#define CSTEG_STEG_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include "Utils.h"

class StegUtils {
public:
	static bool encodeLosslessJpegFile(const char *inputFileName, const char* outputFileName,
			const unsigned char* const data, int dataLength, unsigned short numberOfBitsToSteal, int quality);
	static bool decodeLosslessJpegFile(const char *inputFileName, DecodeResponse& decodeResponse);

	static void encodeMessageToJpeg(const char *inputFileName, const char* outputFileName,
                                  const unsigned char* const data, int dataLength,
                                  unsigned short numberOfBitsToSteal);

	static int doCoefficientDiff(const char* filePath1, const char* filePath2);
	static unsigned int getNumberOfCoefficients(const char *inputFileName);
	static void doCompressions(int n, const char* filePath, int quality);
	static void purge(const char* jpegFilePath, int limit);

private:
	static int diffCoefficients(JpegCoefficients& coefficients1, JpegCoefficients& coefficients2);
	static short getLosslessEncodedShort(short coef, short coefBits, short dataBits);

};

#endif
