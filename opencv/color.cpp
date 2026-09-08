#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

int main()
{
    cv::Mat img = cv::imread("lena.png");

    // BGR 转 HSV
    // HSV
    // H：Hue，色相，即“是什么颜色”。红、黄、绿、蓝这些颜色在一个环上排列。
    // S：Saturation，饱和度，即“颜色的纯度”。饱和度越高，颜色越纯；饱和度越低，颜色越灰。
    // V：Value，明度，即“颜色的亮度”。明度越高
    // HSV 方便用 H 找颜色，然后 S/V 排除灰暗区域或调整
    std::cout << "BGR to HSV" << std::endl;
    cv::Mat hsv;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    for (int i = 0; i < 10; i++) {
            auto p_hsv = hsv.at<cv::Vec3b>(i, 0);
            auto p_bgr = img.at<cv::Vec3b>(i, 0);
            printf("(%d, %d) BGR: (%d, %d, %d) to HSV: (%d, %d, %d)\n", i, 0,
                   p_bgr[0], p_bgr[1], p_bgr[2],
                   p_hsv[0], p_hsv[1], p_hsv[2]);
    }
    std::cout << std::endl;


    // BGR 转灰度图
    std::cout << "BGR to GRAY" << std::endl;
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    for (int i = 0; i < 10; i++) {
        auto p_gray = gray.at<uchar>(i, 0);
        auto p_bgr = img.at<cv::Vec3b>(i, 0);
        printf("(%d, %d) BGR: (%d, %d, %d) to Gray: (%d)\n", i, 0,
                p_bgr[0], p_bgr[1], p_bgr[2],
                p_gray);
    }
    std::cout << std::endl;

    // BGR 转 YUV444
    std::cout << "BGR to YUV" << std::endl;
    cv::Mat yuv;
    cv::cvtColor(img, yuv, cv::COLOR_BGR2YUV);
    for (int i = 0; i < 10; i++) {
        auto p_yuv = yuv.at<cv::Vec3b>(i, 0);
        auto p_bgr = img.at<cv::Vec3b>(i, 0);
        printf("(%d, %d) BGR: (%d, %d, %d) to YUV: (%d, %d, %d)\n", i, 0,
                p_bgr[0], p_bgr[1], p_bgr[2],
                p_yuv[0], p_yuv[1], p_yuv[2]);
    }
    std::cout << std::endl;

    return 0;
}