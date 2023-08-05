#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>

int threshold_value = 60;
double shrinking_value = 0.1;
int const max_binary_value = 255;


int main() {
    cv::Mat im_gray = imread("/Users/bdmpea/CLionProjects/logic/c.jpg",cv::ImreadModes::IMREAD_GRAYSCALE);
    cv::Mat dst;
    cv::threshold(im_gray, dst, threshold_value, max_binary_value, cv::THRESH_BINARY);
    resize(dst, dst, cv::Size(), shrinking_value, shrinking_value, cv::INTER_NEAREST);
    imwrite("c_new.bmp",  dst);
    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            int value = dst.at<uchar>(i, j) == 255 ? 1 : 0;
            std::cout << value;
        }
        std::cout << "\n";
    }
}

