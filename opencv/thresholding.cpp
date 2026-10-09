#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

/*
 * 全局阈值：整张图都使用同一个阈值进行二值化。
 * 适合光照较均匀、前景和背景差异明显的图像。
 */
void global_threshold(cv::Mat &gray)
{
    cv::Mat binary;

    // THRESH_BINARY：像素值 > thresh 时设为 maxval，否则设为 0。
    cv::threshold(gray, binary, 127, 255, cv::THRESH_BINARY);
    cv::imshow("threshold-binary", binary);

    // THRESH_BINARY_INV：像素值 > thresh 时设为 0，否则设为 maxval。
    cv::threshold(gray, binary, 127, 255, cv::THRESH_BINARY_INV);
    cv::imshow("threshold-binary-inv", binary);

    // THRESH_TRUNC：像素值 > thresh 时截断为 thresh，否则保持原值。
    cv::threshold(gray, binary, 127, 255, cv::THRESH_TRUNC);
    cv::imshow("threshold-trunc", binary);

    // THRESH_TOZERO：像素值 > thresh 时保持原值，否则设为 0。
    cv::threshold(gray, binary, 127, 255, cv::THRESH_TOZERO);
    cv::imshow("threshold-tozero", binary);

    /*
     * Otsu（大津法）根据图像的灰度分布自动选择阈值。
     * 适合前景和背景亮度差异明显的图像。
     * THRESH_OTSU 需要与基本阈值类型组合；此时传入的 thresh 会被忽略。
     */
    cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imshow("threshold-otsu", binary);

}

/*
 * 自适应阈值：不为整张图使用同一个阈值，而是为每个局部区域分别计算阈值。
 * 它适合光照不均的图像，能让较亮和较暗区域都得到合适的二值化结果。
 */
void adaptive_threshold(const cv::Mat &gray)
{
    cv::Mat binary;
    // ADAPTIVE_GAUSSIAN_C：按邻域的高斯加权平均计算阈值。
    // 11：邻域大小（blockSize）；10：从计算出的阈值中减去的常数 C。
    cv::adaptiveThreshold(gray, binary, 255,
        cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 5);
    cv::imshow("threshold-adaptive", binary);
}

int main()
{
    cv::Mat img = cv::imread("bug.png");
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::imshow("threshold-original", gray);

    global_threshold(gray);
    adaptive_threshold(gray);
    cv::waitKey(0);
    return 0;
}
