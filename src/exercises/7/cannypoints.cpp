#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <opencv2/core/matx.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 5

int main(int argc, char **argv) {
    vector<int> yrange;
    vector<int> xrange;

    Mat image, border, frame, points;

    int width, height, gray;
    int x, y;

    image = imread("./exercises/7/hunt.jpg", cv::IMREAD_GRAYSCALE);

    srand(time(0));

    if (!image.data) {
        cout << "nao abriu" << argv[1] << endl;
        cout << argv[0] << " imagem.jpg";
        exit(0);
    }

    width = image.size().width;
    height = image.size().height;

    int cannyThreshold = 30;
    Canny(image, border, cannyThreshold, 3 * cannyThreshold);

    xrange.resize(height / STEP);
    yrange.resize(width / STEP);

    iota(xrange.begin(), xrange.end(), 0);
    iota(yrange.begin(), yrange.end(), 0);

    for (uint i = 0; i < xrange.size(); i++) {
        xrange[i] = xrange[i] * STEP + STEP / 2;
    }

    for (uint i = 0; i < yrange.size(); i++) {
        yrange[i] = yrange[i] * STEP + STEP / 2;
    }

    points = Mat(height, width, CV_8U, Scalar(255));

    random_shuffle(xrange.begin(), xrange.end());

    for (auto i : xrange) {
        random_shuffle(yrange.begin(), yrange.end());
        for (auto j : yrange) {
            x = i + rand() % (2 * JITTER) - JITTER + 1;
            y = j + rand() % (2 * JITTER) - JITTER + 1;
            gray = image.at<uchar>(x, y);

            circle(points, cv::Point(y, x), RAIO, CV_RGB(gray, gray, gray), -1,
                   cv::LINE_AA);
        }
    }

    for (int i = 0; i < 10; i++) {
        for (auto i : xrange) {
            random_shuffle(yrange.begin(), yrange.end());
            for (auto j : yrange) {
                x = i + rand() % (2 * JITTER) - JITTER + 1;
                y = j + rand() % (2 * JITTER) - JITTER + 1;
                gray = image.at<uchar>(x, y);

                if (border.at<uchar>(x, y) == 255) {
                    circle(points, cv::Point(y, x), RAIO - 2,
                           CV_RGB(gray, gray, gray), -1, cv::LINE_AA);
                }
            }
        }
    }

    imshow("points", points);

    waitKey();

    imwrite("./exercises/7/pontos.jpg", points);
    imwrite("./exercises/7/cannyborders.png", border);

    return 0;
}