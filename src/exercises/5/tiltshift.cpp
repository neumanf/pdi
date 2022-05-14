#include <cmath>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>

cv::Mat image, filteredImage, imageTop;

char TrackbarName[50];

int height_slider = 75;
int height_slider_max = 100;

int decay_slider = 100;
int decay_slider_max = 100;

int center_slider = 100;
int center_slider_max = 100;

double l1, l2;

void on_trackbar_decay(int, void *) {
    for (int i = 0; i < filteredImage.rows; i++) {
        double d = (double)decay_slider;
        double alpha = (0.5) * (tanh((i - l1) / d) - tanh((i - l2) / d));

        addWeighted(image.row(i), alpha, imageTop.row(i), 1 - alpha, 0,
                    filteredImage.row(i));
    }

    cv::imshow("addweighted", filteredImage);
}

void on_trackbar_height(int, void *) {
    l1 = height_slider - center_slider + round(image.size().height / 2);
    l2 = height_slider + center_slider + round(image.size().height / 2);
    on_trackbar_decay(decay_slider, 0);
}

void on_trackbar_center(int, void *) {
    l1 = height_slider - center_slider + round(image.size().height / 2);
    l2 = height_slider + center_slider + round(image.size().height / 2);
    on_trackbar_decay(decay_slider, 0);
}

int main(int argvc, char **argv) {
    image = cv::imread("./exercises/5/alaska.png");

    cv::namedWindow("addweighted", 1);

    float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                     0.1111, 0.1111, 0.1111, 0.1111};

    cv::Mat mask = cv::Mat(3, 3, CV_32F, media);

    cv::filter2D(image, filteredImage, image.depth(), mask, cv::Point(1, 1), 0);

    filteredImage.copyTo(imageTop);

    std::sprintf(TrackbarName, "Height x %d", height_slider_max);
    cv::createTrackbar(TrackbarName, "addweighted", &height_slider,
                       height_slider_max, on_trackbar_height);
    on_trackbar_height(height_slider, 0);

    std::sprintf(TrackbarName, "Decay x %d", decay_slider_max);
    cv::createTrackbar(TrackbarName, "addweighted", &decay_slider,
                       decay_slider_max, on_trackbar_decay);
    on_trackbar_decay(decay_slider, 0);

    std::sprintf(TrackbarName, "Center x %d", center_slider_max);
    cv::createTrackbar(TrackbarName, "addweighted", &center_slider,
                       center_slider_max, on_trackbar_center);
    on_trackbar_center(center_slider, 0);

    cv::waitKey(0);

    return 0;
}
