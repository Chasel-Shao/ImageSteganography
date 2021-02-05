#include <iostream>
#include "botan/block_cipher.h"

#include "lubyrackoff.h"
#include "teaEncode.cpp"

using namespace std;
using namespace Botan;
int main() {


  string f1 = "abc";			// holds key
  string *key = &f1;	// pointer to key
  string str = "hello World";			// holds user input to encipher
  string outdata;		// holds enciphered text
  string decoded;		// holds deciphered text


  // Encipher and display
  encipher(str, f1, &outdata);
  str = "0";	// clear string
  cout << "\nCiphertext: " << outdata << endl;


  // Decipher and display
  decipher(outdata, f1, &decoded);
  cout << "Decoded: " << decoded << endl;


  return 0;
}