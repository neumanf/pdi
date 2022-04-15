#include <iostream>
#include <opencv2/opencv.hpp>

struct Point {
    int x, y;
};

bool isAValidPoint(Point point, cv::Size imageSize) {
    if (point.x < 0 || point.x > imageSize.height) {
        return false;
    }

    if (point.y < 0 || point.y > imageSize.width) {
        return false;
    }

    return true;
}

struct Point getPointCoordinates(std::string pointName) {
    struct Point point;

    std::cout << "\n> Point " << pointName << " coordinates\n";
    std::cout << "x: ";
    std::cin >> point.x;
    std::cout << "y: ";
    std::cin >> point.y;

    return point;
}

int main(int argc, char **argv) {
    cv::Mat image =
        cv::imread("../assets/images/mountain.png", cv::IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cout << "Error while opening the image.\n";
        return -1;
    }

    cv::Size imageSize = image.size();

    struct Point p1 = getPointCoordinates("1");
    struct Point p2 = getPointCoordinates("2");

    if (!isAValidPoint(p1, imageSize)) {
        std::cout << "Invalid coordinates for point P1.\n";
        return -1;
    };

    if (!isAValidPoint(p2, imageSize)) {
        std::cout << "Invalid coordinates for point P2.\n";
        return -1;
    };

    for (int i = p1.x; i < p2.x; i++) {
        for (int j = p1.y; j < p2.y; j++) {
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
        }
    }

    cv::imshow("window", image);
    cv::waitKey();

    return 0;
}