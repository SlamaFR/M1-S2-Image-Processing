#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;

int clamp(const int& x)
{
    return std::min(std::max(x, 0), 255);
}

void draw_circle(Mat& image, const int& x, const int& y, const int& radius)
{
    for (int i = y - radius; i < y + radius; i++) {
        for (int j = x - radius; j < x + radius; j++) {
            if (std::pow(x - j, 2) + std::pow(y - i, 2) < radius * radius)
            {
                image.at<Vec3b>(i, j) = Vec3b(0, 255, 255);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    // check arguments
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " image" << std::endl;
        return -1;
    }

    // load the input image
    std::cout << "load image ..." << std::endl;
    cv::Mat image = cv::imread(argv[1]);
    if (image.empty()) {
        std::cout << "error loading " << argv[1] << std::endl;
        return -1;
    }
    std::cout << "image size : " << image.cols << " x " << image.rows << std::endl;

    const auto midX = image.cols / 2;
    const auto midY = image.rows / 2;

    for (int i = midY - 50; i < midY + 50; i++) {
        for (int j = midX - 50; j < midX + 50; j++) {
            image.at<Vec3b>(i, j) = Vec3b(0, 255, 0);
        }
    }

    for (int i = midY - 50; i < midY + 50; i++) {
        for (int j = midX - 50; j < midX + 50; j++) {
            image.at<Vec3b>(i, j) = Vec3b(255, 255, 255) - Vec3b(255 * (i - midY + 50) / 100, 255 * (i - midY + 50) / 100, 255 * (i - midY + 50) / 100);
        }
    }

    draw_circle(image, 100, 100, 90);

    // display an image
    cv::imshow("une image", image);
    std::cout << "appuyer sur une touche ..." << std::endl;
    cv::waitKey();

    // save the image
    cv::imwrite("output/tp1ex1.jpg", image);

    return 1;
}
