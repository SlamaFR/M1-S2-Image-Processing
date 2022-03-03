#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "fourierTransform.hpp"


cv::Mat sobelXFourierKernel(const int width, const int height) {
    cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

    kernel.at<float>(height / 2 - 1, width / 2 - 1) = -1;
    kernel.at<float>(height / 2, width / 2 - 1) = -2;
    kernel.at<float>(height / 2 + 1, width / 2 - 1) = -1;

    kernel.at<float>(height / 2 - 1, width / 2 + 1) = 1;
    kernel.at<float>(height / 2, width / 2 + 1) = 2;
    kernel.at<float>(height / 2 + 1, width / 2 + 1) = 1;

    return kernel;
}

cv::Mat sobelYFourierKernel(const int width, const int height) {
    cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

    kernel.at<float>(height / 2 - 1, width / 2 - 1) = -1;
    kernel.at<float>(height / 2 - 1, width / 2) = -2;
    kernel.at<float>(height / 2 - 1, width / 2 + 1) = -1;

    kernel.at<float>(height / 2 + 1, width / 2 - 1) = 1;
    kernel.at<float>(height / 2 + 1, width / 2) = 2;
    kernel.at<float>(height / 2 + 1, width / 2 + 1) = 1;

    return kernel;
}

cv::Mat laplaceFourierKernel(const int width, const int height) {
    cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

    kernel.at<float>(height / 2 - 1, width / 2 - 1) = 1;
    kernel.at<float>(height / 2 - 1, width / 2 + 1) = 1;
    kernel.at<float>(height / 2 + 1, width / 2 - 1) = 1;
    kernel.at<float>(height / 2 + 1, width / 2 + 1) = 1;
    kernel.at<float>(height / 2, width / 2) = -4;

    return kernel;
}

cv::Mat edgeAmplifierFourierKernel(const int width, const int height) {
    cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

    kernel.at<float>(height / 2 - 1, width / 2 - 1) = -1;
    kernel.at<float>(height / 2 - 1, width / 2 + 1) = -1;
    kernel.at<float>(height / 2 + 1, width / 2 - 1) = -1;
    kernel.at<float>(height / 2 + 1, width / 2 + 1) = -1;
    kernel.at<float>(height / 2, width / 2) = 5;

    return kernel;
}

cv::Mat gaussianFourierKernel(const int width, const int height, const double sigma) {
    cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int x = j - (width / 2);
            int y = i - (height / 2);
            float g = 1. / (2 * M_PI * sigma * sigma) * exp(-(x * x + y * y) / (2 * sigma * sigma));
            kernel.at<float>(i, j) = g;
        }
    }

    return kernel;
}

cv::Mat myFourierKernel(const int width, const int height) {
    cv::Mat kernel(height, width, CV_32F, cv::Scalar(0.0));

    kernel.at<float>(height / 2 - 1, width / 2 - 1) = -10;
    kernel.at<float>(height / 2 - 1, width / 2 + 1) = -10;
    kernel.at<float>(height / 2 + 1, width / 2 - 1) = -10;
    kernel.at<float>(height / 2 + 1, width / 2 + 1) = -10;
    kernel.at<float>(height / 2, width / 2) = 45;

    return kernel;
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

    // convert to gray scale
    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);




    // discrete Fourier Transform
    cv::Mat imageFourierMagnitude, imageFourierPhase;
    discreteFourierTransform(image, imageFourierMagnitude, imageFourierPhase);

    // do someting here ....
    // ...
    //cv::Mat Xfilter = sobelXFourierKernel(image.cols, image.rows);
    //cv::Mat Yfilter = sobelYFourierKernel(image.cols, image.rows);
    //cv::Mat XfilterFourierMagnitude, XfilterFourierPhase;
    //cv::Mat YfilterFourierMagnitude, YfilterFourierPhase;
    //discreteFourierTransform(Xfilter, XfilterFourierMagnitude, XfilterFourierPhase);
    //discreteFourierTransform(Yfilter, YfilterFourierMagnitude, YfilterFourierPhase);
    //for (int i = 0; i < image.rows; i++) {
    //    for (int j = 0; j < image.cols; j++) {
    //        imageFourierMagnitude.at<float>(i, j) *= XfilterFourierMagnitude.at<float>(i, j);
    //        imageFourierMagnitude.at<float>(i, j) *= YfilterFourierMagnitude.at<float>(i, j);
    //    }
    //}


    //cv::Mat filter = laplaceFourierKernel(image.cols, image.rows);
    //cv::Mat filterFourierMagnitude, filterFourierPhase;
    //discreteFourierTransform(filter, filterFourierMagnitude, filterFourierPhase);
    //for (int i = 0; i < image.rows; i++) {
    //    for (int j = 0; j < image.cols; j++) {
    //        imageFourierMagnitude.at<float>(i, j) *= filterFourierMagnitude.at<float>(i, j);
    //    }
    //}


    //cv::Mat filter = edgeAmplifierFourierKernel(image.cols, image.rows);
    //cv::Mat filterFourierMagnitude, filterFourierPhase;
    //discreteFourierTransform(filter, filterFourierMagnitude, filterFourierPhase);
    //for (int i = 0; i < image.rows; i++) {
    //    for (int j = 0; j < image.cols; j++) {
    //        imageFourierMagnitude.at<float>(i, j) *= filterFourierMagnitude.at<float>(i, j);
    //    }
    //}


    //cv::Mat filter = gaussianFourierKernel(image.cols, image.rows, 5);
    //cv::Mat filterFourierMagnitude, filterFourierPhase;
    //discreteFourierTransform(filter, filterFourierMagnitude, filterFourierPhase);
    //for (int i = 0; i < image.rows; i++) {
    //    for (int j = 0; j < image.cols; j++) {
    //        imageFourierMagnitude.at<float>(i, j) *= filterFourierMagnitude.at<float>(i, j);
    //    }
    //}


    cv::Mat filter = myFourierKernel(image.cols, image.rows);
    cv::Mat filterFourierMagnitude, filterFourierPhase;
    discreteFourierTransform(filter, filterFourierMagnitude, filterFourierPhase);
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            imageFourierMagnitude.at<float>(i, j) *= filterFourierMagnitude.at<float>(i, j);
        }
    }

    // inverse Fourier Transform
    cv::Mat outputImage;
    inverseDiscreteFourierTransform(imageFourierMagnitude, imageFourierPhase, outputImage, CV_8U);


    // display everything
    cv::namedWindow("Input image");
    cv::namedWindow("Magnitude");
    cv::namedWindow("Output image");

    cv::moveWindow("Input image", 100, 50);
    cv::moveWindow("Magnitude", 700, 50);
    cv::moveWindow("Output image", 100, 400);

    cv::imshow("Input image", image);
    cv::imshow("Magnitude", fourierMagnitudeToDisplay(imageFourierMagnitude));
    cv::imshow("Output image", outputImage);
    cv::waitKey();

    // save the images
    cv::imwrite("output/inputImage.jpg", image);
    cv::imwrite("output/magnitude.png", fourierMagnitudeToDisplay(imageFourierMagnitude));
    cv::imwrite("output/phase.png", fourierPhaseToDisplay(imageFourierPhase));
    cv::imwrite("output/filteredImage.png", outputImage);

    return 0;
}
