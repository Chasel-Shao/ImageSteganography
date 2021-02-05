//
// Created by Chasel on 2020/5/7.
//

#ifndef INTEGERWAVELET_INTHAAR2_H
#define INTEGERWAVELET_INTHAAR2_H

#include <opencv2/opencv.hpp>

using namespace cv;

class WaveletComponet{
public:
  Mat A, H, V, D;
};


class IntHaar2 {
public:
  static WaveletComponet intWaveletTransform(Mat image);
  static Mat invIntWaveletTransform(WaveletComponet components);
};


// utils method
Mat convertMatToShow(Mat image);
Mat mergeChannels(Mat m1, Mat m2, Mat m3);

#endif //INTEGERWAVELET_INTHAAR2_H
