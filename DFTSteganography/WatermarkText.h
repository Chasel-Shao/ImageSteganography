#ifndef WATERMARK_TEXT_H
#define WATERMARK_TEXT_H

#include <iostream>
#include <opencv2/opencv.hpp>

class WaterMarkText
{
public:

	cv::Mat addTextWatermarkSingleChannel(const cv::Mat &input, const std::string &text);

	cv::Mat addTextWatermarkColorImage(const cv::Mat &inputImage, const std::string &text);

	cv::Mat getWaterMarkSingleChannel(const cv::Mat &input);

	cv::Mat getWatermarkColorImage(const cv::Mat &input);

	cv::Mat getTransposeImage(const cv::Mat &input);

  cv::Mat moveToCenter(cv::Mat &result);

private:

};

#endif