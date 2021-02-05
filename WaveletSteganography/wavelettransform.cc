#include "wavelettransform.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


Mat WaveletTransform::serialWaveletTransform(Mat image) {
  vector<Mat> channels;
  split(image, channels);

  // version 1
  Mat img1 = transform(channels.at(0));
  Mat img2 = transform(channels.at(1));
  Mat img3 = transform(channels.at(2));

// version 2
//  Mat img1 = cvHaarWavelet(channels.at(0), 2);
//  Mat img2 = cvHaarWavelet(channels.at(1), 2);
//  Mat img3 = cvHaarWavelet(channels.at(2), 2);

  vector<Mat> channels2;
  channels2.push_back(img1);
  channels2.push_back(img2);
  channels2.push_back(img3);

  Mat dst;
  merge(channels2, dst);

  return dst;
}

Mat WaveletTransform::serialInverseWaveletTransform(Mat image) {
  // inverse
  vector<Mat> channels;
  split(image, channels);

  // version 1
  Mat img1 = in_transform(channels.at(0));
  Mat img2 = in_transform(channels.at(1));
  Mat img3 = in_transform(channels.at(2));

// version 2
//  Mat img1 = cvInvHaarWavelet(channels.at(0), 2);
//  Mat img2 = cvInvHaarWavelet(channels.at(1), 2);
//  Mat img3 = cvInvHaarWavelet(channels.at(2), 2);


  vector<Mat> channels2;
  channels2.push_back(img1);
  channels2.push_back(img2);
  channels2.push_back(img3);

  Mat dst;
  merge(channels2, dst);
  dst.convertTo(dst, CV_8UC1);

  return dst;
}


//--------------------------------
// wavelet transform version 1
//--------------------------------
Mat WaveletTransform::transform(Mat img) {
  int height = img.rows;
  int width = img.cols;

  int depth = 2;
  int depthcount = 1;

  Mat tmp = Mat::ones(Size(width, height), CV_32FC1);
  Mat wavelet = Mat::ones(Size(width, height), CV_32FC1);
  Mat imgtmp = img.clone();

  imgtmp.convertTo(imgtmp, CV_32FC1);

  //---------------------------------------Wavelet decomposition-----------------------------//
  while (depthcount <= depth) {
    height = img.rows / pow(2, depthcount - 1);
    width = img.cols / pow(2, depthcount - 1);

    // cols inverse transformation
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width / 2; ++j) {
        float value1 = imgtmp.at<float>(i, 2 * j);
        float value2 = imgtmp.at<float>(i, 2 * j + 1);
        tmp.at<float>(i, j) = (value1 + value2) / 2;
        tmp.at<float>(i, j + width / 2) = (value1 - value2) / 2;
      }
    }

    // rows inverse transformation
    for (int i = 0; i < height / 2; ++i) {
      for (int j = 0; j < width; ++j) {
        float value1 = tmp.at<float>(2 * i, j);
        float value2 = tmp.at<float>(2 * i + 1, j);
        wavelet.at<float>(i, j) = (value1 + value2) / 2;
        wavelet.at<float>(i + height / 2, j) = (value1 - value2) / 2;
      }
    }
    /*
    // Low pass filter, choose gaussian low pass filter
    Mat ROI = wavelet(Rect(0, 0, width, height));
    GaussianBlur(ROI, ROI, Size(7, 7), 0.5);
    Mat dst(wavelet, Rect(0, 0, width, height));
    ROI.copyTo(dst);
    */
    imgtmp = wavelet;
    depthcount++;
  }

  return wavelet;
}


//--------------------------------------
// Inverse wavelet transform version 1
//-------------------------------------
Mat WaveletTransform::in_transform(Mat img) {
  int depth = 2;
  int height = img.rows;
  int width = img.cols;

  Mat tmp = Mat::ones(Size(width, height), CV_32FC1);
  Mat wavelet = Mat::ones(Size(width, height), CV_32FC1);
  Mat imgtmp = img.clone();

  imgtmp.convertTo(imgtmp, CV_32FC1);

//------------------------------------------wavelet reconstruction--------------------------------------//
  while (depth > 0) {
    height = img.rows / pow(2, depth - 1);
    width = img.cols / pow(2, depth - 1);
    // cols inverse transformation
    for (int i = 0; i < height / 2; ++i) {
      for (int j = 0; j < width; ++j) {
        float value1 = imgtmp.at<float>(i, j);
        float value2 = imgtmp.at<float>(i + height / 2, j);
        tmp.at<float>(2 * i, j) = value1 + value2;
        tmp.at<float>(2 * i + 1, j) = value1 - value2;
      }
    }

    // rows inverse transformation
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width / 2; ++j) {
        float value1 = tmp.at<float>(i, j);
        float value2 = tmp.at<float>(i, j + width / 2);
        wavelet.at<float>(i, 2 * j) = value1 + value2;
        wavelet.at<float>(i, 2 * j + 1) = value1 - value2;
      }
    }

    Mat ROI=wavelet(Rect(0, 0, width, height));
    Mat dst(imgtmp, Rect(0, 0, width, height));
    ROI.copyTo(dst);
    depth--;
  }
  return wavelet;
}

//--------------------------------
// Wavelet transform version 2
//--------------------------------
Mat WaveletTransform::cvHaarWavelet(Mat &src, int NIter)
{
  Mat dst = Mat(src.rows, src.cols, CV_32FC1);
  src.convertTo(src, CV_32FC1);

  float c,dh,dv,dd;
  int width = src.cols;
  int height = src.rows;
  for (int k=0;k<NIter;k++)
  {
    for (int y=0;y<(height>>(k+1));y++)
    {
      for (int x=0; x<(width>>(k+1));x++)
      {
        c=(src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)+src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
        dst.at<float>(y,x)=c;

        dh=(src.at<float>(2*y,2*x)+src.at<float>(2*y+1,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x+1))*0.5;
        dst.at<float>(y,x+(width>>(k+1)))=dh;

        dv=(src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)-src.at<float>(2*y+1,2*x+1))*0.5;
        dst.at<float>(y+(height>>(k+1)),x)=dv;

        dd=(src.at<float>(2*y,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
        dst.at<float>(y+(height>>(k+1)),x+(width>>(k+1)))=dd;
      }
    }
    dst.copyTo(src);
  }
  return dst;
}

//------------------------------------
//Inverse wavelet transform version 2
//------------------------------------
Mat WaveletTransform::cvInvHaarWavelet(Mat &src, int NIter)
{
  Mat dst = Mat(src.rows, src.cols, CV_32FC1);
  src.convertTo(src, CV_32FC1);
  float c,dh,dv,dd;
  assert( dst.type() == CV_32FC1 );
  int width = src.cols;
  int height = src.rows;
  //--------------------------------
  // NIter - number of iterations
  //--------------------------------
  for (int k=NIter;k>0;k--)
  {
    for (int y=0;y<(height>>k);y++)
    {
      for (int x=0; x<(width>>k);x++)
      {
        c=src.at<float>(y,x);
        dh=src.at<float>(y,x+(width>>k));
        dv=src.at<float>(y+(height>>k),x);
        dd=src.at<float>(y+(height>>k),x+(width>>k));

        dst.at<float>(y*2,x*2)=0.5*(c+dh+dv+dd);
        dst.at<float>(y*2,x*2+1)=0.5*(c-dh+dv-dd);
        dst.at<float>(y*2+1,x*2)=0.5*(c+dh-dv-dd);
        dst.at<float>(y*2+1,x*2+1)=0.5*(c-dh-dv+dd);
      }
    }
    Mat C=src(Rect(0,0,width>>(k-1),height>>(k-1)));
    Mat D=dst(Rect(0,0,width>>(k-1),height>>(k-1)));
    D.copyTo(C);
  }
  return dst;
}

