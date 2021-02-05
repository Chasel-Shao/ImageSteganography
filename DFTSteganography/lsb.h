//
// Created by Chasel on 2020/4/11.
//

#ifndef LODEPNGTEST_LSB_H
#define LODEPNGTEST_LSB_H
#include <iostream>

using namespace std;

class MessageData {
public:
  int bitToSteal = 0;
  unsigned int lengthPayload = 0;
  unsigned char* data;

  // image property
  int rows = 0;
  int cols = 0;
  int type = 0;
};

void setHeader(std::vector<unsigned char> &image, MessageData & messageData);
void getHeader(std::vector<unsigned char> &image, MessageData & messageData);
void readPayload(std::vector<unsigned char> &image, unsigned char *data, int dataByteIndex, int
dataLength, int numberOfBitsToSteal);
void addPayload(std::vector<unsigned char> &image,  unsigned char * data, int
dataByteIndex, int dataLength, int numberOfBitsToSteal);


#endif //LODEPNGTEST_LSB_H
