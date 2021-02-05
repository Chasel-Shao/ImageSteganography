//
// Created by Chasel on 2020/4/27.
//

#include "steganography.h"
#include <opencv2/opencv.hpp>
#include <omp.h>

using namespace cv;
using namespace std;

void addPayloadInParallel(std::vector<unsigned char> &image, MessageData &message, int numThread);

Mat serialEncode(Mat cover_image_mat, Mat hide_image_mat, string key, double &duration) {
  clock_t time = clock();

  RC4 rc4;
  std::vector<unsigned char> image = (vector<unsigned char>) (cover_image_mat.reshape(1, 1));
  std::vector<unsigned char> hideImage = (vector<unsigned char>) (hide_image_mat.reshape(1, 1));

  MessageData messageData;
  messageData.bitToSteal = 3;
  messageData.lengthPayload = hideImage.size();
  messageData.rows = hide_image_mat.rows;
  messageData.cols = hide_image_mat.cols;
  messageData.type = hide_image_mat.type();
  messageData.data = rc4.doRC4(&hideImage[0], hideImage.size(), key);

  setHeader(image, messageData);
  serialAddPayload(image, messageData);

  cv::Mat mat = cv::Mat(image);
  cv::Mat dest = mat.reshape(cover_image_mat.channels(), cover_image_mat.rows).clone();

  duration = (double) (clock() - time) / CLOCKS_PER_SEC * 1000;
  cout << "serial runtime: " << duration << endl;

  return dest;
}

Mat parallelEncode(Mat cover_image_mat, Mat hide_image_mat, string key, double &duration, int
numThread, int bitToSteal) {
  omp_set_num_threads(numThread);
  double runtime = omp_get_wtime();

  std::vector<unsigned char> image = (vector<unsigned char>) (cover_image_mat.reshape(1, 1));
  std::vector<unsigned char> hideImage = (vector<unsigned char>) (hide_image_mat.reshape(1, 1));
  MessageData messageData;
  messageData.bitToSteal = 3;
  messageData.lengthPayload = hideImage.size();
  messageData.rows = hide_image_mat.rows;
  messageData.cols = hide_image_mat.cols;
  messageData.type = hide_image_mat.type();
#pragma omp parallel sections
  {
#pragma omp section
    {
      setHeader(image, messageData);
    }
#pragma omp section
    {
      RC4 rc4;
      messageData.data = rc4.doRC4(&hideImage[0], hideImage.size(), key);
    }
  };

  parallelAddPayload(image, messageData, numThread);
  Mat mat = cv::Mat(image);
  Mat dest = mat.reshape(cover_image_mat.channels(), cover_image_mat.rows).clone();

  duration = (omp_get_wtime() - runtime) * 1000;
  cout << "parallel runtime: " << duration << endl;

  return dest;
}

Mat parallelDecode(Mat mat, string key, double &duration, int numThread) {
  omp_set_num_threads(numThread);
  double runtime = omp_get_wtime();
  RC4 rc4;
  std::vector<unsigned char> image = (vector<unsigned char>) (mat.reshape(1, 1));
  MessageData messageData2;
  getHeader(image, messageData2);
  messageData2.data = (unsigned char *) malloc(messageData2.lengthPayload + 1);
  memset(messageData2.data, 0, messageData2.lengthPayload + 1);
#pragma omp parallel
  {
    int num = omp_get_num_threads();
    int id = omp_get_thread_num();
    int lengthPayload = messageData2.lengthPayload;

    int partion = lengthPayload / num;
    partion = (partion / messageData2.bitToSteal + 1) * messageData2.bitToSteal;

    int start = id * partion;
    int end = (id + 1) * partion;
    if (id == num - 1) {
      end = lengthPayload;
    }
    readPayload(image, messageData2.data, start, end, messageData2.bitToSteal);
  };

  Mat decodedImage = Mat(messageData2.rows, messageData2.cols, messageData2.type, rc4.doRC4
                                                                                         (messageData2
                                                                                              .data,
                                                                                          messageData2
                                                                                              .lengthPayload,
                                                                                          key));

  duration = (omp_get_wtime() - runtime) * 1000;
  cout << "parallel decode runtime: " << duration << endl;

  return decodedImage;
}


Mat serialDecode(Mat mat, string key, double &duration) {
  clock_t time = clock();
  RC4 rc4;
  std::vector<unsigned char> image = (vector<unsigned char>) (mat.reshape(1, 1));

  MessageData messageData2;
  getHeader(image, messageData2);

  messageData2.data = (unsigned char *) malloc(messageData2.lengthPayload + 1);
  memset(messageData2.data, 0, messageData2.lengthPayload + 1);
  readPayload(image, messageData2.data, 0, messageData2.lengthPayload, messageData2.bitToSteal);

  Mat decodedImage = Mat(messageData2.rows, messageData2.cols, messageData2.type, rc4.doRC4
                                                                                         (messageData2
                                                                                              .data,
                                                                                          messageData2
                                                                                              .lengthPayload,
                                                                                          key));
  duration = (double) (clock() - time) / CLOCKS_PER_SEC * 1000;
  cout << "serial decode runtime: " << duration << endl;
  return decodedImage;
}




void serialAddPayload(std::vector<unsigned char> &image, MessageData &message) {
  addPayload(image, message.data, 0, message.lengthPayload, message.bitToSteal);
}

void parallelAddPayload(std::vector<unsigned char> &image, MessageData &message, int numThread) {
#pragma omp parallel
  {
    int num = omp_get_num_threads();
    int id = omp_get_thread_num();
    int lengthPayload = message.lengthPayload;

    int partion = lengthPayload / num;
    partion = (partion / message.bitToSteal + 1) * message.bitToSteal;

    int start = id * partion;
    int end = (id + 1) * partion;
    if (id == num - 1) {
      end = lengthPayload;
    }

    addPayload(image, message.data, start, end, message.bitToSteal);
  };

//  addPayloadInParallel(image, message, numThread);

}

void addPayloadInParallel(std::vector<unsigned char> &image, MessageData &message, int numThread) {
  int numberOfBitsToSteal = 3;
  int dataByteIndex = 0;
  int dataLength =  message.lengthPayload;
  unsigned char *data = message.data;

  unsigned char temp;
  short shortBuf = 0;
  short dataBits = 0;
  int dataBitIndex = 0;
  int dataMask = (int) (pow((float) 2, numberOfBitsToSteal) - 1);
  time_t t;
  srand((unsigned) time(&t));
  int index = dataByteIndex * 8 / numberOfBitsToSteal + getHeaderBitsLength();
  int chunkSize = dataLength / numThread / 8;

#pragma omp parallel for schedule(static, chunkSize) private(shortBuf,dataBits,dataBitIndex,dataByteIndex) shared(image)
  for (int i = index; i < image.size(); i++) {
    dataByteIndex = (i - index) * 3 / 8;

    if (dataByteIndex < dataLength) {
      shortBuf = data[dataByteIndex];
      if (dataByteIndex < dataLength - 1) {
        shortBuf = (shortBuf << 8) | data[dataByteIndex + 1];
      } else {
        shortBuf = shortBuf << 8;
      }

      temp = image[i];
      dataBits = (shortBuf >> (16 - (dataBitIndex + numberOfBitsToSteal))) & dataMask;
      temp = (temp >> numberOfBitsToSteal << numberOfBitsToSteal) | dataBits;

      //add the bits we stole to the bit count and byte count
      dataBitIndex += numberOfBitsToSteal;
      if (dataBitIndex >= 8) {
        dataBitIndex %= 8;
      }

      image[i] = temp;

    } else {
//      break;
    }
  }
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
