#include <iostream>
#include <opencv2/opencv.hpp>

struct Point {
    int x, y;
};

int main(int argc, char **argv) {
    struct Point p1;
    struct Point p2;

    std::cout << "> Point 1 coordinates\n";
    std::cout << "x: ";
    std::cin >> p1.x;
    std::cout << "y: ";
    std::cin >> p1.y;

    std::cout << "\n> Point 2 coordinates\n";
    std::cout << "x: ";
    std::cin >> p2.x;
    std::cout << "y: ";
    std::cin >> p2.y;

    cv::Mat image =
        cv::imread("../assets/images/car.png", cv::IMREAD_GRAYSCALE);

    if (!image.data)
        std::cout << "Error while opening the image.\n";

    cv::namedWindow("window", cv::WINDOW_AUTOSIZE);

    for (int i = p1.x; i < p1.y; i++) {
        for (int j = p2.x; j < p2.y; j++) {
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
        }
    }

    cv::imshow("window", image);
    cv::waitKey();

    return 0;
}