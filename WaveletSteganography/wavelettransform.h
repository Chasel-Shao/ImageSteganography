
#ifndef WAVELETSTEGANOGRAPHY_WAVELETTRANSFORM_H
#define WAVELETSTEGANOGRAPHY_WAVELETTRANSFORM_H
#include <opencv2/opencv.hpp>

using namespace cv;

class WaveletTransform {
public:
  static Mat serialWaveletTransform(Mat image);
  static Mat serialInverseWaveletTransform(Mat image);

private:
  static Mat transform (Mat img);
  static Mat in_transform (Mat img);
  static Mat cvInvHaarWavelet(Mat &src, int NIter);
  static Mat cvHaarWavelet(Mat &src, int NIter);
};


#endif 
