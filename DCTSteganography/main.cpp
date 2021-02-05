#include <iostream>
#include "StegUtils.h"
#include "JpegUtils.h"
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
double getPSNR(const Mat& I1, const Mat& I2);

int main() {

  string path = "../images/";
  StegUtils stegUtils;
  string input_path = path + "sea.jpeg";
  string output_path = path + "stego-sea.jpg";
  string hide_image_path = path + "baboon_gray.png";
  string decodec_image_path = path + "decodec_secret.png";

  Mat cover_image_mat = imread(hide_image_path, IMREAD_GRAYSCALE);
  std::vector<unsigned char> image = (vector<unsigned char>) (cover_image_mat.reshape(1, 1));

  stegUtils.encodeLosslessJpegFile(input_path.data(), output_path.data(), (unsigned char* )
      &image[0], image.size(), 5, 90);

//  stegUtils.encodeLosslessJpegFile(input_path.data(), output_path.data(), (unsigned char* )
//  message.data(), message.length(), 1, 90);

  DecodeResponse decodeResponse;
  stegUtils.decodeLosslessJpegFile(output_path.data(), decodeResponse);
//  std::string str2((char *)decodeResponse.getData());
//  cout<< decodeResponse.getLength() << ":" << str2 << endl;

  int  rows = 512;
  Mat decodedImage = Mat(rows, rows, CV_8UC1, (unsigned char* )decodeResponse.getData());
  imwrite(decodec_image_path, decodedImage);

  imshow("original ", imread(input_path));
  imshow("modified ", imread(output_path));
  imshow("decoded ", decodedImage);


  Mat image1 = imread(input_path);
  Mat image2 = imread(output_path);

  double psnr = getPSNR(image1, image2);
  cout<< "psnr: " << psnr << endl;

  cvWaitKey(0);
  return 0;
}

double getPSNR(const Mat& I1, const Mat& I2) {
  Mat s1;
  absdiff(I1, I2, s1);       // |I1 - I2|
  s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
  s1 = s1.mul(s1);           // |I1 - I2|^2

  Scalar s = sum(s1);         // sum elements per channel

  double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

  if (sse <= 1e-10) { // for small values return zero
    return 0;
  } else {
    double mse = sse / (double) (I1.channels() * I1.total());
    cout << "mse: " << mse << endl;
    double psnr = 10.0 * log10((255 * 255) / mse);
    return psnr;
  }
}