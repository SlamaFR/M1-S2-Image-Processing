#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#define FLT_NONE 0
#define FLT_AVERAGE 1
#define FLT_MEDIAN 2
#define FLT_LAPLACIAN 3

int clamp(const int &x) {
    return std::min(std::max(x, 0), 255);
}

///////////////////////////////////////////////////////////////////////////////
cv::Vec3b identity(const cv::Mat &src, int x, int y, int) {
    return src.at<cv::Vec3b>(x, y);
}

cv::Vec3b average_filter(const cv::Mat &src, int x, int y, int argument) {
    auto offset = argument / 2;
    cv::Vec3b sum = 0;
    for (int k = x - offset; k <= x + offset; k++) {
        for (int l = y - offset; l <= y + offset; l++) {
            if (k < 0 || l < 0 || k >= src.cols || l >= src.rows) continue;
            sum += src.at<cv::Vec3b>(k, l) / (argument * argument);
        }
    }
    return sum;
}

cv::Vec3b median_filter(const cv::Mat &src, int x, int y, int argument) {
    auto offset = argument / 2;
    std::vector<unsigned char> r;
    std::vector<unsigned char> g;
    std::vector<unsigned char> b;
    for (int k = x - offset; k <= x + offset; k++) {
        for (int l = y - offset; l <= y + offset; l++) {
            if (k < 0 || l < 0 || k >= src.cols || l >= src.rows) continue;
            auto vec = src.at<cv::Vec3b>(k, l);
            b.emplace_back(vec[0]);
            g.emplace_back(vec[1]);
            r.emplace_back(vec[2]);
        }
    }
    std::sort(r.begin(), r.end());
    std::sort(g.begin(), g.end());
    std::sort(b.begin(), b.end());
    return {b[argument + 1], g[argument + 1], r[argument + 1]};
}

cv::Vec3b laplacian_filter(const cv::Mat &src, int x, int y, int) {
    int r = 0;
    int g = 0;
    int b = 0;

    auto vec = src.at<cv::Vec3b>(x, y);
    b += 4 * vec[0];
    g += 4 * vec[1];
    r += 4 * vec[2];
    if (x > 0) {
        vec = src.at<cv::Vec3b>(x - 1, y);
        b -= vec[0];
        g -= vec[1];
        r -= vec[2];
    }
    if (y > 0) {
        vec = src.at<cv::Vec3b>(x, y - 1);
        b -= vec[0];
        g -= vec[1];
        r -= vec[2];
    }
    if (x < src.cols - 1) {
        vec = src.at<cv::Vec3b>(x + 1, y);
        b -= vec[0];
        g -= vec[1];
        r -= vec[2];
    }
    if (y < src.rows - 1) {
        vec = src.at<cv::Vec3b>(x, y + 1);
        b -= vec[0];
        g -= vec[1];
        r -= vec[2];
    }

    return {
        static_cast<unsigned char>(clamp(b)),
        static_cast<unsigned char>(clamp(g)),
        static_cast<unsigned char>(clamp(r))
    };
}

void filtering(const cv::Mat &src, cv::Mat &dst, int filterType, int argument) {
    dst = cv::Mat(src.rows, src.cols, src.type());

    for (int x = 0; x < dst.cols; x++) {
        for (int y = 0; y < dst.rows; y++) {
            switch (filterType) {
                case FLT_NONE:
                    dst.at<cv::Vec3b>(x, y) = identity(src, x, y, argument);
                    break;
                case FLT_AVERAGE:
                    dst.at<cv::Vec3b>(x, y) = average_filter(src, x, y, argument);
                    break;
                case FLT_MEDIAN:
                    dst.at<cv::Vec3b>(x, y) = median_filter(src, x, y, argument);
                    break;
                case FLT_LAPLACIAN:
                    dst.at<cv::Vec3b>(x, y) = laplacian_filter(src, x, y, argument);
                    break;
                default:
                    continue;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    // check arguments
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " image N" << std::endl;
        return -1;
    }

    // load the input image
    std::cout << "load image ..." << std::endl;
    cv::Mat image = cv::imread(argv[1]);
    int n = atoi(argv[2]);

    if (n % 2 == 0) {
        std::cout << "N should be odd" << std::endl;
        return -1;
    }

    if (image.empty()) {
        std::cout << "error loading " << argv[1] << std::endl;
        return -1;
    }
    std::cout << "image size : " << image.cols << " x " << image.rows << std::endl;

    // display an image
    std::cout << "appuyer sur une touche ..." << std::endl;
    cv::imshow("image", image);
    cv::waitKey();

    cv::Mat filteredImage;
    filtering(image, filteredImage, FLT_LAPLACIAN, n);

    std::cout << "appuyer sur une touche ..." << std::endl;
    cv::imshow("image", filteredImage);
    cv::waitKey();

    return 1;
}
