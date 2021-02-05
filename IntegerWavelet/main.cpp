#include <iostream>
#include "inthaar2.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {

  IntHaar2 intHaar2;
  Mat image = imread("../images/lena.png");
  imshow("original image", image);

  vector<Mat> channels;
  split(image, channels);

  // 1. integer wavelet transform
  WaveletComponet componet1 = intHaar2.intWaveletTransform(channels.at(0));
  WaveletComponet componet2 = intHaar2.intWaveletTransform(channels.at(1));
  WaveletComponet componet3 = intHaar2.intWaveletTransform(channels.at(2));

  // 2. Merge colored channels from each component
  Mat colored_A = mergeChannels(componet1.A, componet2.A, componet3.A);
  Mat colored_H = mergeChannels(componet1.H, componet2.H, componet3.H);
  Mat colored_V = mergeChannels(componet1.V, componet2.V, componet3.V);
  Mat colored_D = mergeChannels(componet1.D, componet2.D, componet3.D);

  imshow("Colored A", convertMatToShow(colored_A));
  imshow("Colored H", colored_H);
  imshow("Colored V", colored_V);
  imshow("Colored D", colored_D);

  // 3. inverse integer wavelet transform
  Mat rc1 = intHaar2.invIntWaveletTransform(componet1);
  Mat rc2 = intHaar2.invIntWaveletTransform(componet2);
  Mat rc3 = intHaar2.invIntWaveletTransform(componet3);

  // 4. normalize the recoved image
  Mat merged = mergeChannels(rc1, rc2, rc3);
  Mat recovered = convertMatToShow(merged);
  imshow("Recovered image", recovered);

  cvWaitKey(0);


  return 0;
}


