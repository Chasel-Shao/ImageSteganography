//
// Created by Chasel on 2020/5/7.
//

#include "inthaar2.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


WaveletComponet IntHaar2::intWaveletTransform(cv::Mat image) {

  int m = image.rows;
  int n = image.cols;

  Mat S = Mat::zeros(Size(n, m), CV_32FC1);
  Mat DD = Mat::zeros(Size(n, m), CV_32FC1);

  Mat A = Mat::zeros(Size(n / 2, m / 2), CV_32FC1);
  Mat H = Mat::zeros(Size(n / 2, m / 2), CV_32FC1);
  Mat V = Mat::zeros(Size(n / 2, m / 2), CV_32FC1);
  Mat D = Mat::zeros(Size(n / 2, m / 2), CV_32FC1);

//  Mat A = Mat::zeros(Size(n , m), CV_32FC1);
//  Mat H = Mat::zeros(Size(n , m), CV_32FC1);
//  Mat V = Mat::zeros(Size(n , m), CV_32FC1);
//  Mat D = Mat::zeros(Size(n , m), CV_32FC1);

  Mat wavelet = Mat::zeros(Size(n, m), CV_32FC1);

  Mat DIm = image.clone();
  DIm.convertTo(DIm, CV_32FC1);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n - 1; j += 2) {
      int ii = i;
      int jj = (j + 1) / 2;

      S.at<float>(jj, ii) = int((DIm.at<float>(j, i) + DIm.at<float>(j + 1, i)) / 2);
      DD.at<float>(jj, ii) = DIm.at<float>(j, i) - DIm.at<float>(j + 1, i);;
    }
  }


  for (int j = 0; j < n / 2; j++) {
    for (int i = 0; i < m - 1; i += 2) {
      int ii = (i + 1) / 2;
      int jj = j;

      A.at<float>(jj, ii) = int((S.at<float>(j, i) + S.at<float>(j, i + 1)) / 2);
      H.at<float>(jj, ii) = int((DD.at<float>(j, i) + DD.at<float>(j, i + 1)) / 2);
      V.at<float>(jj, ii) = S.at<float>(j, i) - S.at<float>(j, i + 1);
      D.at<float>(jj, ii) = DD.at<float>(j, i) - DD.at<float>(j, i + 1);
    }
  }


  WaveletComponet componet;
  componet.A = A;
  componet.H = H;
  componet.V = V;
  componet.D = D;


//  Mat A2 = A(Rect(0,0, n/2, m/2));
//  Mat H2 = H(Rect(0,0, n/2, m/2));
//  Mat V2 = V(Rect(0,0, n/2, m/2));
//  Mat D2 = D(Rect(0,0, n/2, m/2));
//
//  Mat P1 = wavelet( Range(0, m / 2), Range(0, n / 2));
//  Mat P2 = wavelet( Range(0, m / 2), Range(n / 2, n));
//  Mat P3 = wavelet( Range(m / 2, m), Range(0, n / 2));
//  Mat P4 = wavelet( Range(m / 2, m), Range(n / 2, n));
//
//  A2.copyTo(P1);
//  H2.copyTo(P2);
//  V2.copyTo(P3);
//  D2.copyTo(P4);

  return componet;
}


Mat IntHaar2::invIntWaveletTransform(WaveletComponet components) {

  Mat A = components.A;
  Mat H = components.H;
  Mat V = components.V;
  Mat D = components.D;

  int m = A.rows;
  int n = A.cols;

  Mat image = Mat::zeros(Size(n * 2, m * 2), CV_32FC1);
  Mat S = Mat::zeros(Size(n * 2, m * 2), CV_32FC1);
  Mat DD = Mat::zeros(Size(n * 2, m * 2), CV_32FC1);

  for (int j = 0; j < n; j++) {
    for (int i = 0; i < m; i++) {
      int ii = 2 * i;
      int jj = j;

      S.at<float>(jj, ii) = A.at<float>(j, i) + int((A.at<float>(j + 1, i) + 1) / 2);
      S.at<float>(jj, ii + 1) = S.at<float>(jj, ii) - V.at<float>(j, i);
      DD.at<float>(jj, ii) = H.at<float>(j, i) + int((D.at<float>(j, i) + 1) / 2);
      DD.at<float>(jj, ii + 1) = DD.at<float>(jj, ii) - D.at<float>(j, i);
    }
  }

  for (int i = 0; i < m * 2; i++) {
    for (int j = 0; j < n; j++) {
      int ii = i;
      int jj = 2 * j;

      image.at<float>(jj, ii) = S.at<float>(j, i) + int((DD.at<float>(j, i) + 1) / 2);
      image.at<float>(jj + 1, ii) = image.at<float>(jj, ii) - DD.at<float>(j, i);
    }
  }


  return image;
}


Mat convertMatToShow(Mat image) {
  Mat dst;
  image.convertTo(dst, CV_8UC1);
  return dst;
}

Mat mergeChannels(Mat m1, Mat m2, Mat m3) {
  std::vector<Mat> channels2;
  channels2.push_back(m1);
  channels2.push_back(m2);
  channels2.push_back(m3);

  Mat dst;
  merge(channels2, dst);
  return dst;
}
