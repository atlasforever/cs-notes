#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void mat()
{
    // 创建 Mat 对象，单通道8位无符号整型，初始化为 255
    Mat m1(4, 4, CV_8UC1, Scalar(255));
    std::cout << "m1:\n"  << m1 << std::endl;

    // 三通道8位无符号整型，初始化为 (0, 255, 255)
    Mat m2(4, 4, CV_8UC3, Scalar(0, 255, 255));
    std::cout << "m2:\n"  << m2 << std::endl;

    Mat m3(4, 4, CV_8UC3, Scalar::all(255));
    std::cout << "m3:\n"  << m3 << std::endl;

    // clone and copyTo
    Mat m4 = m3.clone();
    std::cout << "m4:\n"  << m4 << std::endl;
    Mat m5;
    m3.copyTo(m5);
    std::cout << "m5:\n"  << m5 << std::endl;

}

int main()
{
    mat();
    return 0;
}