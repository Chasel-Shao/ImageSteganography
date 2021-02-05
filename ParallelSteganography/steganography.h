//
// Created by Chasel on 2020/4/27.
//

#ifndef LODEPNGTEST_STEGANOGRAPHY_H
#define LODEPNGTEST_STEGANOGRAPHY_H

#include "lsb.h"
#include "rc4.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


void serialAddPayload(std::vector<unsigned char> &image, MessageData &message);
void parallelAddPayload(std::vector<unsigned char> &image, MessageData &message, int numThread);


Mat serialEncode(Mat cover_image_mat, Mat hide_image_mat, string key, double & duration);
Mat parallelEncode(Mat cover_image_mat, Mat hide_image_mat, string key, double & duration, int
                   numThread = 2, int bitToSteal = 3);

Mat serialDecode(Mat mat, string key, double &duration);
Mat parallelDecode(Mat mat, string key, double &duration, int numThread = 2);


double calculatePSNR(const Mat &I1, const Mat &I2);

#endif //LODEPNGTEST_STEGANOGRAPHY_H
