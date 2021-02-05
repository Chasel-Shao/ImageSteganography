//
// Created by Chasel on 2020/4/21.
//

#ifndef LODEPNGTEST_RC4_H
#define LODEPNGTEST_RC4_H
#include <iostream>

using namespace std;

class RC4 {
public:
  void ksa(string key, int keylength);
  unsigned char* prga(unsigned char* in, int len);
  unsigned char* doRC4(unsigned char* in, int len, string key);

private:
  int S[256];
  char k[256];
};


#endif //LODEPNGTEST_RC4_H
