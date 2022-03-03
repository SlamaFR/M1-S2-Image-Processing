#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "fourierTransform.hpp"



//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  // check arguments
  if(argc != 2){
    std::cout << "usage: " << argv[0] << " image" << std::endl;
    return -1;
  }

  // load the input image
  std::cout << "load image ..." << std::endl;
  cv::Mat image = cv::imread(argv[1]);
  if(image.empty()){
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
  imageFourierMagnitude.at<float>(imageFourierMagnitude.rows / 2, imageFourierMagnitude.cols / 2 - 10) = 0;
  imageFourierMagnitude.at<float>(imageFourierMagnitude.rows / 2, imageFourierMagnitude.cols / 2 + 10) = 0;

  // inverse Fourier Transform
  cv::Mat outputImage;
  inverseDiscreteFourierTransform(imageFourierMagnitude, imageFourierPhase, outputImage, CV_8U);



  // display everything
  cv::namedWindow("Input image");
  cv::namedWindow("Magnitude");
  cv::namedWindow("Output image");

  cv::moveWindow("Input image",100, 50);
  cv::moveWindow("Magnitude",700, 50);
  cv::moveWindow("Output image",100, 400);

  cv::imshow("Input image", image);
  cv::imshow("Magnitude", fourierMagnitudeToDisplay(imageFourierMagnitude));
  cv::imshow("Output image", outputImage);
  cv::waitKey();
 
  // save the images
  cv::imwrite("output/inputImage.jpg",image);
  cv::imwrite("output/magnitude.png", fourierMagnitudeToDisplay(imageFourierMagnitude));
  cv::imwrite("output/phase.png", fourierPhaseToDisplay(imageFourierPhase));
  cv::imwrite("output/filteredImage.png",outputImage);

  return 0;
}
