#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;

int clamp(const int& x)
{
    return std::min(std::max(x, 0), 255);
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


    const auto pixel = image.at<cv::Vec3b>(50, 100);
    std::cout << "Pixel en (50, 100) : R=" << (float) pixel[2] << " V=" << (float) pixel[1] << " B=" << (float) pixel[0]
              << std::endl;

    image.at<Vec3b>(10, 20) = Vec3b(0, 0, 255);

    for (int i = 0; i < image.cols; i++) {
        image.at<Vec3b>(42, i) = Vec3b(0, 0, 255);
    }

    /* On observe des zones de couleurs étranges, cela est dû à l'overflow de uchar.
     * Grâce à clamp, on se limite à des blancs cramés.
     */
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            image.at<Vec3b>(i, j) += Vec3b(50, 50, 50);
        }
    }

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            image.at<Vec3b>(i, j) = Vec3b(255, 255, 255) - image.at<Vec3b>(i, j);
        }
    }

    cvtColor(image, image, COLOR_BGR2GRAY);

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            auto p = image.at<unsigned char>(i, j);
            image.at<unsigned char>(i, j) = p < 128 ? 0 : 255;
        }
    }


    // display an image
    cv::imshow("une image", image);
    std::cout << "appuyer sur une touche ..." << std::endl;
    cv::waitKey();

    // save the image
    cv::imwrite("output/tp1ex1.jpg", image);

    return 1;
}
