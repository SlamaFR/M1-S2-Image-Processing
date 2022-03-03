#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <math.h>
#include <iostream>


//////////////////////////////////////////////////////////////////////////////////////////////////
void callBackKeyboard(int event, int x, int y, int flags, void *userdata) {
    switch (event) {
        case cv::EVENT_LBUTTONDOWN :
            std::cout << "left button pressed at : " << x << ", " << y << std::endl;
            break;

        case cv::EVENT_RBUTTONDOWN :
        case cv::EVENT_MBUTTONDOWN :
        case cv::EVENT_MOUSEMOVE   :
            break;
    }
}

double dtor(double degree) {
    return (degree * (M_PI / 180));
}

cv::Vec3b bilinearInterpolation(const cv::Mat &src, double x, double y) {
    int l = floor(x);
    int k = floor(y);
    double a = x - l;
    double b = y - k;
    cv::Vec3b res = 0;

    res += src.at<cv::Vec3b>(l, k) * (1 - a) * (1 - b);
    res += src.at<cv::Vec3b>(l + 1, k) * a * (1 - b);
    res += src.at<cv::Vec3b>(l, k + 1) * (1 - a) * b;
    res += src.at<cv::Vec3b>(l + 1, k + 1) * a * b;
    return res;
}

cv::Mat rotate(const cv::Mat &src, int x, int y, double alpha) {
    double rad = dtor(alpha);
    auto res = cv::Mat(src.rows, src.cols, src.type());

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            double di = x + (i - y) * cos(rad) - (j - x) * sin(rad);
            double dj = y + (i - y) * sin(rad) + (j - x) * cos(rad);
            if (0 <= di && di < src.rows && 0 <= dj && dj < src.cols) {
                res.at<cv::Vec3b>(i, j) = bilinearInterpolation(src, di, dj);
            }
        }
    }
    return res;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    // check arguments
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " image output" << std::endl;
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

    // setup a window
    cv::namedWindow("image", 1);
    cv::setMouseCallback("image", callBackKeyboard, NULL);


    auto extra = rotate(image, 256, 256, 20);
    std::cout << "done." << std::endl;

    // main loop
    bool loopOn = true;
    while (loopOn) {

        // display the image
        cv::imshow("image", extra);

        // if esc button is pressed
        int key = cv::waitKey(500) % 256;
        if (key == 27 || key == 'q')
            loopOn = false;
    }

    cv::imwrite(argv[2], extra);

    return 1;
}
