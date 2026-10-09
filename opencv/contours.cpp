#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

int main()
{
    cv::Mat img = cv::imread("man.jpg");

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    cv::Mat binary;
    cv::threshold(gray, binary, 80, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

    cv:imshow("original", binary);
    cv::waitKey(0);
    return 0;
}