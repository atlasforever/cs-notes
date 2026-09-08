#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

/*
 * 卷积（convolution）：让一个较小的卷积核（kernel）在图像上滑动。
 * 每到一个位置，将核与其覆盖的像素逐项相乘并求和，结果就是输出图像的新像素值。
 *
 * 例如一个 3 x 3 核覆盖图像的一小块区域：
 *
 *     图像邻域              卷积核
 *     a  b  c              k1 k2 k3
 *     d  e  f       x      k4 k5 k6
 *     g  h  i              k7 k8 k9
 *
 *     输出中心像素 = a*k1 + b*k2 + ... + i*k9
 *
 * 不同的核会产生不同效果，例如模糊、锐化或边缘检测。
 */

/*
 * 模糊：降低图像噪声，但可能损失细节和边缘。
 */
void blur()
{
    cv::Mat img = cv::imread("lena.png");

    cv::imshow("blur-original", img);

    // 使用内置函数 blur() 进行均值模糊
    cv::Mat blur_img;
    cv::blur(img, blur_img, cv::Size(5, 5));
    cv::imshow("blur-blur", blur_img);

    // 高斯模糊使用高斯分布生成卷积核：离中心越近的像素权重越大，越远的权重越小
    // 相比均值模糊，更加自然
    cv::Mat gaussian_img;
    cv::GaussianBlur(img, gaussian_img, cv::Size(5, 5), 1.2);
    cv::imshow("blur-gaussian", gaussian_img);

    // 中值模糊：将卷积核覆盖的像素按大小排序，取中间值作为输出像素
    // 常用于去除椒盐噪声(0或者255的离群值噪点)
    cv::Mat median_img;
    cv::medianBlur(img, median_img, 5);
    cv::imshow("blur-median", median_img);

}

/*
 * 锐化
 * 这里通过 filter2D() 应用自定义卷积核实现。
 */
void sharpen()
{
    cv::Mat img = cv::imread("lena.png");

    // 输出 = 5 * 中心像素 - 上 - 下 - 左 - 右。
    // 平坦区域中各像素接近，结果基本不变；边缘处差异会被放大。
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
        0, -1,  0,
       -1,  5, -1,
        0, -1,  0);

    cv::Mat filtered;
    cv::filter2D(img, filtered, -1, kernel);
    cv::imshow("filter2D-original", img);
    cv::imshow("filter2D-sharpen", filtered);
}

/*
 * 虽然模糊能降低图像噪声，但也可能损失重要细节和清晰边缘。
 * 双边滤波（bilateral filter）用于在平滑、降噪的同时尽量保留边缘。
 *
 * 普通高斯模糊主要看空间距离：靠近中心的邻居权重大。即使邻居位于边缘另一侧，
 * 仍会参与平均，所以边缘容易变模糊。
 *
 * 双边滤波还会看颜色（或亮度）是否相近：距离近且颜色相近的邻居权重大；
 * 若跨过边缘、颜色差异很大，该邻居的权重会降低，因此能尽量避免模糊边缘。
 *
 * 它的权重会随当前位置的图像内容改变，不像普通高斯模糊那样使用固定的卷积核。
 */
void bilateral_filter()
{
    cv::Mat img = cv::imread("lena.png");

    // d：邻域直径
    // sigmaColor：允许多大的颜色差参与平均
    // sigmaSpace：允许多远的像素参与平均
    cv::Mat bilateral_img;
    cv::bilateralFilter(img, bilateral_img, 9, 50, 50);

    cv::imshow("bilateral-original", img);
    cv::imshow("bilateral-filter", bilateral_img);
}

/*
 * Sobel 边缘检测：分别计算图像在 x、y 方向的亮度变化（梯度）。
 * 亮度变化大的位置通常就是边缘，这里将两个方向的结果合并显示。
 */
void sobel_edge()
{
    cv::Mat img = cv::imread("edge.jpg");
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Sobel 分别计算 x、y 方向的梯度
    cv::Mat sobel_x, sobel_y, sobel_mag;
    cv::Sobel(gray, sobel_x, CV_32F, 1, 0, 5);
    cv::Sobel(gray, sobel_y, CV_32F, 0, 1, 5);

    // 梯度幅值：sqrt(sobel_x^2 + sobel_y^2)
    cv::magnitude(sobel_x, sobel_y, sobel_mag);

    // 归一化后显示
    cv::Mat sobel_img;
    cv::normalize(sobel_mag, sobel_img, 0, 255, cv::NORM_MINMAX, CV_8U);
    cv::imshow("sobel-original", img);
    cv::imshow("sobel-edge", sobel_img);
}

/*
 * Canny 边缘检测比单纯的 Sobel 更完整，通常能得到更细、更干净的边缘。
 * 它可以理解为四步：
 *
 * 1. 降噪：先进行高斯模糊，减少噪点和不重要的小纹理。
 * 2. 求梯度：内部通过类似 Sobel 的方式，计算每个像素的边缘强度和方向。
 * 3. 非极大值抑制：沿梯度方向比较邻居，只保留局部最强的点，
 *    将原本较粗的边缘带压缩成细线。
 * 4. 双阈值与滞后连接：大于高阈值的是确定边缘，小于低阈值的被丢弃；
 *    介于两者之间的是弱边缘，只有与确定边缘相连时才会被保留。
 */
void canny_edge()
{
    cv::Mat img = cv::imread("edge.jpg");
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // 先高斯模糊以减少噪声导致的伪边缘。
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 1.4);

    cv::Mat edge;
    cv::Canny(gray, edge, 50, 150);

    cv::imshow("canny-original", img);
    cv::imshow("canny-edge", edge);
}

int main()
{
    // 滤波/模糊
    blur();
    // 锐化
    sharpen();
    // 边缘保留滤波
    bilateral_filter();
    // 边缘检测
    sobel_edge();
    canny_edge();

    cv::waitKey(0);
    return 0;
}
