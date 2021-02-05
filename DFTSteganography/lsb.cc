//
// Created by Chasel on 2020/4/11.
//

#include "lsb.h"
#include <vector>
#include "math.h"

#define DATA_LENGTH_BIT_LENGTH 32
#define BITS_TO_STEAL_BIT_LENGTH 3
#define IMAGE_ROWS 12
#define IMAGE_COLS 12
#define IMAGE_TYPE 8

static int getHeaderLength() {
  return DATA_LENGTH_BIT_LENGTH + BITS_TO_STEAL_BIT_LENGTH + IMAGE_ROWS + IMAGE_COLS
         + IMAGE_TYPE;
}

unsigned char encodeBit(unsigned char data, short imageBits, short dataBits) {
  if (imageBits != dataBits) {
    if (data >= 255) {
      data--;
    } else if (data == 0) {
      data++;
    } else {
      //randomly either go up one or down one to modifiy the coefficient bit
      if (rand() % 2 == 1) {
        ++data;
      } else {
        --data;
      }
    }
  }

  return data;
}


void addPayload(std::vector<unsigned char> &image, unsigned char * data, int
dataByteIndex, int dataLength, int numberOfBitsToSteal) {
  unsigned char temp;
  short shortBuf = 0;
  short dataBits = 0;
  int dataBitIndex = 0;
  int dataMask = (int) (pow((float) 2, numberOfBitsToSteal) - 1);
  time_t t;
  srand((unsigned) time(&t));
  int index = dataByteIndex * 8 / numberOfBitsToSteal + getHeaderLength();
  for (int i = index; i < image.size(); i++) {
    if (dataByteIndex < dataLength) {
      temp = image[i];
      // use a short buf here to pack the current
      // byte and the next byte of the data.  this
      // makes it easier for values for bits to steal like
      // 3 which can spill over between bytes
      shortBuf = data[dataByteIndex];
      if (dataByteIndex < dataLength - 1) {
        shortBuf = (shortBuf << 8) | data[dataByteIndex + 1];
      } else {
        shortBuf = shortBuf << 8;
      }

      dataBits = (shortBuf >> (16 - (dataBitIndex + numberOfBitsToSteal))) & dataMask;
      temp = (temp >> numberOfBitsToSteal << numberOfBitsToSteal) | dataBits;
      if (temp == 0) {
        temp = temp | (1 << numberOfBitsToSteal);
      }

      //add the bits we stole to the bit count and byte count
      dataBitIndex += numberOfBitsToSteal;
      if (dataBitIndex >= 8) {
        dataBitIndex %= 8;
        ++dataByteIndex;
      }

      image[i] = temp;
    } else {
      break;
    }
  }
}

void readPayload(std::vector<unsigned char> &image, unsigned char *data, int dataByteIndex, int
dataLength, int numberOfBitsToSteal) {
  unsigned char temp;
  short dataBits = 0;
  int intBuff = 1;
  int intBuffCount = 0;
  int dataMask = (int) (pow((float) 2, numberOfBitsToSteal) - 1);
  int index = dataByteIndex * 8 / numberOfBitsToSteal + getHeaderLength();
  for (int i = index; i < image.size(); i++) {
    if (dataByteIndex < dataLength) {
      temp = image[i];
      dataBits = temp & dataMask;

      intBuff = (intBuff << numberOfBitsToSteal) | dataBits;
      intBuffCount += numberOfBitsToSteal;
      if (intBuffCount >= 8) {
        intBuffCount -= 8;
        data[dataByteIndex] = ((intBuff >> intBuffCount) & 255);
        ++dataByteIndex;
      }
    }
  }
}

void setHeader(std::vector<unsigned char> &image, MessageData & messageData) {
  unsigned char c;
  short imageBits = 0;
  short dataBits = 0;
  short imageRowsCount = 0;
  short imageColsCount = 0;
  short imageTypeCount = 0;
  short lengthPayloadBitCount = 0;
  short numberOfBitsToStealPayloadCount = 0;
  int avaiableBits = image.size() * messageData.bitToSteal - getHeaderLength();
  messageData.lengthPayload = min<int>(messageData.lengthPayload, avaiableBits / 8);
  int end = min<long>(image.size(), getHeaderLength());
  for (int i = 0; i < end; i++) {
    c = image[i];
    if (lengthPayloadBitCount < DATA_LENGTH_BIT_LENGTH) { // 4 bytes
      imageBits = c & 1;
      dataBits = (messageData.lengthPayload >> lengthPayloadBitCount) & 1;
      image[i] = encodeBit(c, imageBits, dataBits);
      ++lengthPayloadBitCount;
    } else if (numberOfBitsToStealPayloadCount < BITS_TO_STEAL_BIT_LENGTH) {
      //encode the number of bits we stole when encoding this image
      imageBits = c & 1;
      dataBits = (messageData.bitToSteal >> numberOfBitsToStealPayloadCount) & 1;
      image[i] = encodeBit(c, imageBits, dataBits);
      ++numberOfBitsToStealPayloadCount;
    } else if (imageRowsCount < IMAGE_ROWS) {
      imageBits = c & 1;
      dataBits = (messageData.rows >> imageRowsCount) & 1;
      image[i] = encodeBit(c, imageBits, dataBits);
      ++imageRowsCount;
    } else if (imageColsCount < IMAGE_COLS) {
      imageBits = c & 1;
      dataBits = (messageData.cols >> imageColsCount) & 1;
      image[i] = encodeBit(c, imageBits, dataBits);
      ++imageColsCount;
    }else if (imageTypeCount < IMAGE_COLS) {
      imageBits = c & 1;
      dataBits = (messageData.type >> imageTypeCount) & 1;
      image[i] = encodeBit(c, imageBits, dataBits);
      ++imageTypeCount;
    }
  }
}

void getHeader(std::vector<unsigned char> &image, MessageData & messageData) {
  unsigned char c;
  short imageBits = 0;
  short imageRowsCount = 0;
  short imageColsCount = 0;
  short imageTypeCount = 0;
  short lengthPayloadBitCount = 0;
  short numberOfBitsToStealPayloadCount = 0;
  int end = min<long>(image.size(), getHeaderLength());
  for (int i = 0; i < end; i++) {
    c = image[i];
    if (lengthPayloadBitCount < DATA_LENGTH_BIT_LENGTH) {
      imageBits = c & 1;
      messageData.lengthPayload = messageData.lengthPayload | (imageBits << lengthPayloadBitCount);
      ++lengthPayloadBitCount;
    } else if (numberOfBitsToStealPayloadCount < BITS_TO_STEAL_BIT_LENGTH) {
      //encode the number of bits we stole when encoding this image
      imageBits = c & 1;
      messageData.bitToSteal = messageData.bitToSteal | (imageBits << numberOfBitsToStealPayloadCount);
      ++numberOfBitsToStealPayloadCount;
    } else if (imageRowsCount < IMAGE_ROWS) {
      //encode the number of bits we stole when encoding this image
      imageBits = c & 1;
      messageData.rows = messageData.rows | (imageBits << imageRowsCount);
      ++imageRowsCount;
    } else if (imageColsCount < IMAGE_COLS) {
      //encode the number of bits we stole when encoding this image
      imageBits = c & 1;
      messageData.cols = messageData.cols | (imageBits << imageColsCount);
      ++imageColsCount;
    } else if (imageTypeCount < IMAGE_TYPE) {
      //encode the number of bits we stole when encoding this image
      imageBits = c & 1;
      messageData.type = messageData.type | (imageBits << imageTypeCount);
      ++imageTypeCount;
    } else {
      break;
    }
  }
}


