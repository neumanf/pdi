#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
    cv::Mat image =
        cv::imread("../assets/images/mountain.png", cv::IMREAD_GRAYSCALE);

    int imageWidth = image.size().width;
    int imageHeight = image.size().height;

    if (!image.data)
        std::cout << "Error while opening the image.\n";

    cv::namedWindow("window", cv::WINDOW_AUTOSIZE);

    cv::Mat newImage = cv::Mat::zeros(imageHeight, imageWidth, CV_8UC1);

    // First quadrant
    for (int i = 0; i < imageHeight / 2; i++) {
        for (int j = 0; j < imageWidth / 2; j++) {
            newImage.at<uchar>(imageHeight / 2 + i, imageWidth / 2 + j) =
                image.at<uchar>(i, j);
        }
    }

    // Second quadrant
    for (int i = 0; i < imageHeight / 2; i++) {
        for (int j = imageWidth / 2; j < imageWidth; j++) {
            newImage.at<uchar>(imageHeight / 2 + i - 1, imageWidth / 2 + j) =
                image.at<uchar>(i, j);
        }
    }

    // Third quadrant
    for (int i = imageHeight / 2; i < imageHeight; i++) {
        for (int j = 0; j < imageWidth / 2; j++) {
            newImage.at<uchar>(i - imageHeight / 2, j + imageWidth / 2) =
                image.at<uchar>(i, j);
        }
    }

    // Forth quadrant
    for (int i = imageHeight / 2; i < imageHeight; i++) {
        for (int j = imageWidth / 2; j < imageWidth; j++) {
            newImage.at<uchar>(i - imageHeight / 2, j - imageWidth / 2) =
                image.at<uchar>(i, j);
        }
    }

    cv::imshow("window", newImage);
    cv::waitKey();

    return 0;
}