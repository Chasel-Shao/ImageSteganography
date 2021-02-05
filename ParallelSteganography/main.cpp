#include <iostream>
#include <opencv2/opencv.hpp>
#include "steganography.h"
#include "InputParser.cc"
#include <omp.h>

using namespace cv;
using namespace std;

void test();
void test2();
int main(int argc, char *argv[]) {


//int c = 0;
//#pragma omp parallel for schedule(static, 4)
//  for(int i = 0; i < 10; i++ )
//  {
//    printf("%d i=%d, thread ID=%d\n", c++, i, omp_get_thread_num());
//  }
//
//  return 0;

  InputParser input(argc, argv);
  // Generate stego image
  if (input.cmdOptionExists("-e")) {
   string filename = input.getCmdOption("-src");
   if (filename.length() == 0)  {
     cout << "Missing parameter: -src "<< endl;
     exit(1);
   }
   string hidefilename = input.getCmdOption("-h");
    if (hidefilename.length() == 0)  {
      cout << "Missing parameter: -h "<< endl;
      exit(1);
    }
   string key = input.getCmdOption("-k");
    if (key.length() == 0)  {
      cout << "Missing parameter: -k "<< endl;
      exit(1);
    }
    int p = 2;
    string threads = input.getCmdOption("-p");
    if (threads.length() > 0)  {
      p = stoi(threads);
    }
    int bitToSteal = 3;
    string bits = input.getCmdOption("-bit");
    if (bits.length() > 0)  {
      bitToSteal = stoi(threads);
    }

    Mat cover_image_mat = imread(filename);
    Mat hide_image_mat = imread(hidefilename, IMREAD_GRAYSCALE);

    double parallelTime = 0;
    double serialTime = 0;

    serialEncode(cover_image_mat, hide_image_mat, key, serialTime);
    Mat stegoImage = parallelEncode(cover_image_mat, hide_image_mat, key, parallelTime, p, bitToSteal);

    cout << "Sppedup: " << serialTime / parallelTime << endl;

    string tofilename = input.getCmdOption("-dst");
    if (tofilename.length() > 0)  {
      // write to file
      imwrite(tofilename, stegoImage);
    }

    imshow("Stego Image", stegoImage);
    cvWaitKey(0);
  }
  // Decode stego image
  else if (input.cmdOptionExists("-d")) {
    string filename = input.getCmdOption("-src");
    if (filename.length() == 0)  {
      cout << "Missing parameter: -src "<< endl;
      exit(1);
    }
    string key = input.getCmdOption("-k");
    if (key.length() == 0)  {
      cout << "Missing parameter: -k "<< endl;
      exit(1);
    }
    int p = 2;
    string threads = input.getCmdOption("-p");
    if (threads.length() > 0)  {
      p = stoi(threads);
    }

    double parallelTime = 0;
    double serialTime = 0;
    // decode image from file
    Mat cover_image_mat = imread(filename);
    serialDecode(cover_image_mat, key, serialTime);
    Mat decodedImage = parallelDecode(cover_image_mat, key, parallelTime, p);

    cout << "Sppedup: " << serialTime / parallelTime << endl;

    // show the image
    imshow("Decoded from stego image", decodedImage);

    string tofilename = input.getCmdOption("-dst");
    if (tofilename.length() > 0) {
      imwrite(tofilename, decodedImage);
    }

    cvWaitKey(0);

  } else if (input.cmdOptionExists("-psnr")) {
    string filename = input.getCmdOption("-src");
    if (filename.length() == 0)  {
      cout << "Missing parameter: -src "<< endl;
      exit(1);
    }
    string tofilename = input.getCmdOption("-dst");
    if (tofilename.length() == 0)  {
      cout << "Missing parameter: -dst "<< endl;
      exit(1);
    }

    // calculate the psnr
    Mat img1 = imread(filename);
    Mat img2 = imread(tofilename);
    calculatePSNR(img1, img2);

  } else if (input.cmdOptionExists("-t")) {
    test();
  } else {
    cout << "Usage: -e -d -t -psnr" << endl;
  }
}


void test2() {
  string path = "../TestImage/";
  string filename = path + "lena.png";
  string tofilename = path + "lena-test.png";
  string hide_image_path = path + "baboon_gray.png";
  string decoded_image_path = path + "decoded_image.png";

  Mat cover_image_mat = imread(filename);
  Mat hide_image_mat = imread(hide_image_path, IMREAD_GRAYSCALE);
  string key = "High Performance Computing";


  double parallelTime = 0;
  double serialTime = 0;

  // warm-up
  serialEncode(cover_image_mat, hide_image_mat, key, serialTime);
  parallelEncode(cover_image_mat, hide_image_mat, key, parallelTime);
  serialTime = 0;
  parallelTime = 0;

  cout << "======================> begin <=====================" << endl;

  Mat encoded_image;
  for (int p = 2; p <= 8; p+=2) {
    double speedup = 0.0;
    double averageSerialTime = 0.0;
    for (int i = 0; i < 10; i++) {
      serialEncode(cover_image_mat, hide_image_mat, key, serialTime);
      encoded_image = parallelEncode(cover_image_mat, hide_image_mat, key, parallelTime, p);
      speedup += serialTime / parallelTime;
      averageSerialTime += serialTime;
    }
    cout << "=====> Average Serail Time for P = " << p<< " is: " << averageSerialTime/10 << endl;
    cout << "=====> Average Sppedup for P = " << p<< " is: " << speedup/10 << endl;
  }
  // write to file
  imshow("encoded_image", encoded_image);
  imwrite(tofilename, encoded_image);

cout << "=====================================================" << endl;

  // decode image from file
  double parallelDecodeTime = 0;
  double serialDecodeTime = 0;
  // decode image from file
  Mat stego_image_mat = imread(tofilename);

  Mat decodedImage;
  for (int p = 2; p <= 8; p+=2) {
    double speedup = 0.0;
    double averageSerialTime = 0.0;
    for (int i = 0; i < 10; i++) {
      serialDecode(stego_image_mat, key, serialDecodeTime);
      decodedImage = parallelDecode(stego_image_mat, key, parallelDecodeTime);
      speedup += serialDecodeTime / parallelDecodeTime;
      averageSerialTime += serialDecodeTime;
    }
    cout << "=====> Average Serail Time for P = " << p<< " is: " << averageSerialTime/10 << endl;
    cout << "=====> Average Sppedup for P = " << p<< " is: " << speedup/10 << endl;
  }

  // show the image
  imshow("Decoded Image", decodedImage);
  imwrite(decoded_image_path, decodedImage);

  // calculate the psnr
  Mat img1 = imread(filename);
  Mat img2 = imread(tofilename);
  calculatePSNR(img1, img2);

  cvWaitKey(0);
}

void test() {
  string path = "../TestImage/";
  string filename = path + "lena.png";
  string tofilename = path + "lena-test.png";
  string hide_image_path = path + "baboon_gray.png";
  string decoded_image_path = path + "decoded_image.png";

  Mat cover_image_mat = imread(filename);
  Mat hide_image_mat = imread(hide_image_path, IMREAD_GRAYSCALE);
  string key = "High Performance Computing";

  double parallelTime = 0;
  double serialTime = 0;

  serialEncode(cover_image_mat, hide_image_mat, key, serialTime);
  Mat encoded_image = parallelEncode(cover_image_mat, hide_image_mat, key, parallelTime);

  cout << "Encode Sppedup:" << serialTime / parallelTime << endl;

  // write to file
  imshow("encoded_image", encoded_image);
  imwrite(tofilename, encoded_image);

  // decode image from file
  double parallelDecodeTime = 0;
  double serialDecodeTime = 0;
  // decode image from file
  Mat stego_image_mat = imread(tofilename);
  serialDecode(stego_image_mat, key, serialDecodeTime);
  Mat decodedImage = parallelDecode(stego_image_mat, key, parallelDecodeTime);

  cout << "Decode Sppedup: " << serialDecodeTime / parallelDecodeTime << endl;

  // show the image
  imshow("Decoded Image", decodedImage);
  imwrite(decoded_image_path, decodedImage);

  // calculate the psnr
  Mat img1 = imread(filename);
  Mat img2 = imread(tofilename);
  calculatePSNR(img1, img2);

  cvWaitKey(0);
}
