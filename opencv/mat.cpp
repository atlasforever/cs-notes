#include <iostream>
#include <opencv2/core/matx.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

void mat()
{
    // 创建 Mat 对象，单通道8位无符号整型，初始化为 255
    Mat m1(4, 4, CV_8UC1, Scalar(255));
    std::cout << "m1:\n"  << m1 << std::endl;

    // BGR 三通道8位无符号整型，初始化为 (0, 255, 255)
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

    // Mat_<> 模板类可以列表初始化
    Mat m6 = (Mat_<float>(3, 3) <<
    0, -1,  0,
   -1,  5, -1,
    0, -1,  0);
    std::cout << "m6:\n"  << m6 << std::endl;

    // Rect 提取 ROI
    Rect rect(1, 0, 2, 2);
    Mat m7 = m6(rect);
    std::cout << "m7:\n"  << m7 << std::endl;
}

void pixel()
{
    Mat img = imread("lena.png");

    std::cout << "lena size: " << img.rows << "x" << img.cols << std::endl;

    // 遍历像素
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            // 每个像素是一个 Vec3b 类型的向量，包含 BGR 三个通道的值
            // 灰度图时是 uchar 类型的单通道值
            auto pixel = img.at<cv::Vec3b>(i, j);
            auto b = pixel[0];
            auto g = pixel[1];
            auto r = pixel[2];
        }
    }

    // 支持加减乘除、位运算
    // 加减乘除可以调整亮度
    Mat contrast = Mat(img.rows, img.cols, img.type(), Scalar(100, 100, 100));
    Mat dark = img - contrast;
    Mat bright = img + contrast;
    cv::imshow("dark", dark);
    cv::imshow("bright", bright);
}

void type()
{
    Mat img = imread("lena.png");

    // 当前类型 CV_8UC3
    std::cout << "lena type: " << img.type() << std::endl;

    // 当前通道 BGR(3)
    // cv::cvtColor 可以转为灰度图
    std::cout << "lena channels: " << img.channels() << std::endl;
    Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::imshow("gray", gray);

    // spit / merge 合并与分离通道
    std::vector<Mat> channels;
    cv::split(img, channels);
    auto &g = channels[1];
    g = g * 2; // 将绿色通道增强
    Mat dst;
    cv::merge(channels, dst);
    cv::imshow("merge", dst);
}

int main()
{
    // Mat 对象存储每个像素
    mat();
    // 访问像素
    pixel();
    // 图像类型和通道
    type();

    cv::waitKey(0);
    return 0;
}