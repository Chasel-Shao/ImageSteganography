//
// Created by Chasel on 2020/4/21.
//

#include "rc4.h"

void RC4::ksa(string key, int keylength) {
  for (int i = 0; i < 256; i++) {
    S[i] = i;
  }
  int j = 0, temp;
  for (int i = 0; i < 256; i++) {
    j = (j + S[i] + key[i % keylength]) % 256;
    temp = S[i];
    S[i] = S[j];
    S[j] = temp;
  }
}

unsigned char* RC4::prga(unsigned char* in, int len) {
  int i = 0, j = 0, x = 0, temp;
  unsigned char *t = (unsigned char *)malloc(sizeof(unsigned char) * len);
  for (x = 0; x < len; x++) {
    i = (i + 1) % 256;
    j = (j + S[i]) % 256;
    temp = S[i];
    S[i] = S[j];
    S[j] = temp;
    t[x] = (unsigned char)(in[x] ^ S[(S[i] + S[j]) % 256]);
  }
  return t;
}

unsigned char* RC4::doRC4(unsigned char* in, int len, string key) {
  int key_size = (int) key.size();
  // Create Key Stream
  ksa(key, key_size);
  // Encrypt or Decrypt input (plaintext)
  return prga(in, len);
}
