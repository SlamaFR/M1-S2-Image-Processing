#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>


//////////////////////////////////////////////////////////////////////////////////////////////////
void callBackKeyboard(int event, int x, int y, int, void *) {
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

cv::Mat extrapolate(const cv::Mat &src) {
    auto res = cv::Mat(src.rows * 3, src.cols * 3, src.type());

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            res.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(src.rows - 1 - i, src.cols - 1 - j);
            res.at<cv::Vec3b>(i, j + src.cols) = src.at<cv::Vec3b>(src.rows - 1 - i, j);
            res.at<cv::Vec3b>(i, j + src.cols * 2) = src.at<cv::Vec3b>(src.rows - 1 - i, src.cols - 1 - j);
            res.at<cv::Vec3b>(i + src.rows, j) = src.at<cv::Vec3b>(i, src.cols - 1 - j);
            res.at<cv::Vec3b>(i + src.rows, j + src.cols) = src.at<cv::Vec3b>(i, j);
            res.at<cv::Vec3b>(i + src.rows, j + src.cols * 2) = src.at<cv::Vec3b>(i, src.cols - 1 - j);
            res.at<cv::Vec3b>(i + src.rows * 2, j) = src.at<cv::Vec3b>(src.rows - 1 - i, src.cols - 1 - j);
            res.at<cv::Vec3b>(i + src.rows * 2, j + src.cols) = src.at<cv::Vec3b>(src.rows - 1 - i, j);
            res.at<cv::Vec3b>(i + src.rows * 2, j + src.cols * 2) = src.at<cv::Vec3b>(src.rows - 1 - i,
                                                                                      src.cols - 1 - j);
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


    auto extra = extrapolate(image);
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
