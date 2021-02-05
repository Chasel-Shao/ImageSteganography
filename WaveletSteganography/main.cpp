#include <iostream>
#include <opencv2/opencv.hpp>
#include "wavelettransform.h"
#include "lsb.h"

using namespace cv;
using namespace std;

void compareMat(Mat src1, Mat src2) {
  int r = src1.rows;
  int c = src1.cols * src1.channels();
  bool isequal = true;
  for (int k = 0; k < r; k++) {
    // 每一行图像的指针
    uchar *inData1 = src1.ptr<uchar>(k);
    uchar *inData2 = src2.ptr<uchar>(k);
    for (int i = 0; i < c; i++) {
      if (inData1[i] != inData2[i]) {
        cout << (int) inData1[i] << " != " << (int) inData2[i] << endl;
        isequal = false;
      }
    }
  }
  if (isequal) {
    cout << "mat is equal" << endl;
  }
}
double calculatePSNR(const Mat &I1, const Mat &I2);
Mat serialEncode(Mat src, Mat secret);
Mat decode(Mat src);

#define IMAGE_ROWS 512
#define IMAGE_COLS 512
#define BIT_TO_STEAL 3
int main() {
  
  string path = "../images/";
  string filepath = path + "lena.png";
  string secret_image_path = path + "baboon_gray.png";
  string stego_image_path = path + "lena-stego.png";
  string decoded_image_path = path + "secret-decoded.png";
     
  Mat cover_image_mat = imread(filepath);
  imshow("original image", cover_image_mat);

  Mat secret_image_mat = imread(secret_image_path, IMREAD_GRAYSCALE);
  imshow("secret image", secret_image_mat);

  Mat dst = serialEncode(cover_image_mat, secret_image_mat);
  imshow("Stego image", dst);
  imwrite(stego_image_path, dst);

  // decode
  Mat stego_image_mat = imread(stego_image_path);
  Mat dest_mat = decode(stego_image_mat);
  imshow("Decoded Image", dest_mat);
  imwrite(decoded_image_path, dest_mat);

//  compareMat(secret_image_mat, dest_mat);

  calculatePSNR(cover_image_mat, stego_image_mat);

  cvWaitKey(0);
  return 0;
}


Mat serialEncode(Mat src, Mat secret) {

  Mat dst = WaveletTransform::serialWaveletTransform(src);

  vector<unsigned char> image = (vector<unsigned char>)(dst.reshape(1, 1));
  vector<unsigned char> message = (vector<unsigned char>)(secret.reshape(1, 1));
  MessageData messageData;
  messageData.bitToSteal = BIT_TO_STEAL;
  messageData.data = &message[0];
  messageData.lengthPayload = message.size();
  messageData.rows = secret.rows;
  messageData.cols = secret.cols;
  messageData.type = secret.type();
  setHeader(image, messageData);
  addPayload(image, messageData.data, 0, messageData.lengthPayload, messageData.bitToSteal);

  cv::Mat mat = cv::Mat(image);
  Mat dst2 = WaveletTransform::serialInverseWaveletTransform(mat.reshape(src.channels(), src.rows).clone());

  return dst2;
}

Mat decode(Mat src) {

  src = WaveletTransform::serialWaveletTransform(src);
  vector<unsigned char> image = (vector<unsigned char>)(src.reshape(1, 1));

  MessageData messageData2;
  getHeader(image, messageData2);
  messageData2.lengthPayload = IMAGE_ROWS * IMAGE_COLS;
  messageData2.rows = IMAGE_ROWS;
  messageData2.cols = IMAGE_COLS;
  messageData2.type = CV_8UC1;
  messageData2.data = (unsigned char *) malloc(messageData2.lengthPayload + 1);
  memset(messageData2.data, 0, messageData2.lengthPayload + 1);
  readPayload(image, messageData2.data, 0, messageData2.lengthPayload, messageData2.bitToSteal);

  Mat dest_mat = Mat(messageData2.rows, messageData2.cols, messageData2.type, messageData2.data, 0);
  return dest_mat;
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

