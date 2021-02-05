#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "WatermarkText.h"

using namespace std;
using namespace cv;

double calculatePSNR(const Mat &I1, const Mat &I2);

int main() {
  WaterMarkText wmt;
  string path = "../images/";
  string filepath = path + "lena.png";
  string dftpath = path + "dft.png";
  string stegopath = path + "lena-stego.png";
  string decoded_dft_path = path + "decoded-dft.png";
  Mat img = imread(filepath);
  imshow("original", img);

  //  DFT
  Mat imgDFT = wmt.getWatermarkColorImage(img);
  imshow("DFT", imgDFT);
  imwrite(dftpath, imgDFT * 255);

  Mat result = wmt.addTextWatermarkColorImage(img, "CS590 HPC");
  imshow("encoded Image", result);
  imwrite(stegopath, result * 255);

  Mat stego_image_mat = imread(stegopath);

  // Decoded FFT
  Mat decodedDFT = wmt.getWatermarkColorImage(stego_image_mat);
  imshow("Decoded DFT", decodedDFT);
  imwrite(decoded_dft_path, decodedDFT * 255);

  calculatePSNR(img, stego_image_mat);


  cv::waitKey(0);
  return 0;
}

double calculatePSNR(const Mat &I1, const Mat &I2) {
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
    cout << "psnr: " << psnr << endl;
    return psnr;
  }
}
