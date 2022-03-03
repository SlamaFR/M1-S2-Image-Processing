#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>


///////////////////////////////////////////////////////////////////////////////
unsigned char clamp_uchar(const int value) {
    return std::min(std::max(value, 0), 255);
}

///////////////////////////////////////////////////////////////////////////////
cv::Mat computeHistogramGS(const cv::Mat &image) {
    // create and initialize and 1d array of integers
    cv::Mat histogram(256, 1, CV_32S, cv::Scalar(0));

    // compute the histogram
    for (int i = 0; i < image.rows; ++i)
        for (int j = 0; j < image.cols; ++j)
            histogram.at<int>(image.at<unsigned char>(i, j))++;

    return histogram;
}

cv::Mat computeCumulatedHistogramGS(const cv::Mat &imageGS) {
    // create and initialize and 1d array of integers

    cv::Mat histogram = computeHistogramGS(imageGS);
    cv::Mat cumulatedHistogram(256, 1, CV_32S, cv::Scalar(0));

    // compute the histogram
    cumulatedHistogram.at<int>(0) = histogram.at<int>(0);
    for (int i = 1; i < 256; i++) {
        cumulatedHistogram.at<int>(i) = cumulatedHistogram.at<int>(i - 1) + histogram.at<int>(i);
    }

    return cumulatedHistogram;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat histogramToImageGS(const cv::Mat &histogram) {
    // create an image
    const unsigned int histoHeight = 100;
    cv::Mat histogramImage(histoHeight, 256, CV_8U, cv::Scalar(0));

    // find the max value of the histogram
    double minValue, maxValue;
    cv::minMaxLoc(histogram, &minValue, &maxValue);

    // write the histogram lines
    for (int j = 0; j < 256; ++j)
        cv::line(histogramImage, cv::Point(j, histoHeight),
                 cv::Point(j, histoHeight - (histoHeight * histogram.at<int>(j)) / maxValue), cv::Scalar(255), 1);

    return histogramImage;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat generateLUTplop(const cv::Mat &image) {

    // create a LUT
    cv::Mat myLUT(256, 1, CV_8U);

    // find min and max value
    double minValue, maxValue;
    cv::minMaxLoc(image, &minValue, &maxValue);

    // compute the normalized image LUT
    for (int i = minValue; i < 256; i++)
        myLUT.at<unsigned char>(i) = clamp_uchar(i - minValue);

    return myLUT;
}

cv::Mat generateLUTnormalize(const cv::Mat &image) {

    // create a LUT
    cv::Mat myLUT(256, 1, CV_8U);

    // find min and max value
    double minValue, maxValue;
    cv::minMaxLoc(image, &minValue, &maxValue);

    // compute the normalized image LUT
    for (int i = minValue; i < 256; i++)
        myLUT.at<unsigned char>(i) = clamp_uchar((i - minValue) / (maxValue - minValue) * 256);

    return myLUT;
}

cv::Mat generateLUThistogramNormalize(const cv::Mat &imageGS) {

    // create a LUT
    cv::Mat myLUT(256, 1, CV_8U);

    auto hc = computeCumulatedHistogramGS(imageGS);
    for (int i = 0; i < 256; i++) {
        myLUT.at<unsigned char>(i) = clamp_uchar((255 * hc.at<int>(i)) / (imageGS.rows * imageGS.cols));
    }

    return myLUT;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat apply_LUT(const cv::Mat &image, const cv::Mat &lut) {

    // make a copy of image
    cv::Mat finalImage;
    image.copyTo(finalImage);

    // aplly the LUT
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
            finalImage.at<unsigned char>(i, j) = lut.at<unsigned char>(image.at<unsigned char>(i, j));

    return finalImage;
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

    // convert the image to grayscale
    // cvtColor(image,image,CV_BGR2GRAY);
    cvtColor(image, image, cv::COLOR_BGR2GRAY);


    // display an image
    std::cout << "appuyer sur une touche ..." << std::endl;
    cv::imshow("image", image);
    cv::imshow("histogramme", histogramToImageGS(computeHistogramGS(image)));
    cv::imshow("histogramme cumule", histogramToImageGS(computeCumulatedHistogramGS(image)));
    cv::waitKey();

    // normalize the image
    cv::Mat normalizeImage = apply_LUT(image, generateLUThistogramNormalize(image));

    // display an image
    std::cout << "appuyer sur une touche ..." << std::endl;
    cv::imshow("image", normalizeImage);
    cv::imshow("histogramme", histogramToImageGS(computeHistogramGS(normalizeImage)));
    cv::imshow("histogramme cumule", histogramToImageGS(computeCumulatedHistogramGS(normalizeImage)));
    cv::waitKey();

    return 1;
}
