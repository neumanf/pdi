#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <unistd.h>

int main(int argc, char **argv) {
    cv::Mat image;
    int width, height;
    cv::VideoCapture cap;
    std::vector<cv::Mat> planes;
    cv::Mat histR, histG, histB;
    int nbins = 64;
    float range[] = {0, 255};
    const float *histrange = {range};
    bool uniform = true;
    bool acummulate = false;
    int key;
    int currentHistSum = 0, lastHistSum = 0;

    cap.open(0);

    if (!cap.isOpened()) {
        std::cout << "cameras indisponiveis";
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "largura = " << width << std::endl;
    std::cout << "altura  = " << height << std::endl;

    int histw = nbins, histh = nbins / 2;
    cv::Mat histImgR(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat histImgG(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat histImgB(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat lastHistImgR(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));

    while (1) {
        cap >> image;
        cv::split(image, planes);
        cv::calcHist(&planes[0], 1, 0, cv::Mat(), histB, 1, &nbins, &histrange,
                     uniform, acummulate);
        cv::calcHist(&planes[1], 1, 0, cv::Mat(), histG, 1, &nbins, &histrange,
                     uniform, acummulate);
        cv::calcHist(&planes[2], 1, 0, cv::Mat(), histR, 1, &nbins, &histrange,
                     uniform, acummulate);

        cv::normalize(histR, histR, 0, histImgR.rows, cv::NORM_MINMAX, -1,
                      cv::Mat());
        cv::normalize(histG, histG, 0, histImgG.rows, cv::NORM_MINMAX, -1,
                      cv::Mat());
        cv::normalize(histB, histB, 0, histImgB.rows, cv::NORM_MINMAX, -1,
                      cv::Mat());

        histImgR.setTo(cv::Scalar(0));
        histImgG.setTo(cv::Scalar(0));
        histImgB.setTo(cv::Scalar(0));

        for (int i = 0; i < nbins; i++) {
            cv::line(histImgR, cv::Point(i, histh),
                     cv::Point(i, histh - cvRound(histR.at<float>(i))),
                     cv::Scalar(0, 0, 255), 1, 8, 0);
            cv::line(histImgG, cv::Point(i, histh),
                     cv::Point(i, histh - cvRound(histG.at<float>(i))),
                     cv::Scalar(0, 255, 0), 1, 8, 0);
            cv::line(histImgB, cv::Point(i, histh),
                     cv::Point(i, histh - cvRound(histB.at<float>(i))),
                     cv::Scalar(255, 0, 0), 1, 8, 0);
        }

        histImgR.copyTo(image(cv::Rect(0, 0, nbins, histh)));
        histImgG.copyTo(image(cv::Rect(0, histh, nbins, histh)));
        histImgB.copyTo(image(cv::Rect(0, 2 * histh, nbins, histh)));

        for (int i = 0; i < histh; i++) {
            for (int j = 0; j < histw; j++) {
                lastHistSum += lastHistImgR.at<uchar>(i, j);
                currentHistSum += histImgR.at<uchar>(i, j);
            }
        }

        double comparationResult = abs(currentHistSum - lastHistSum);
        int detectionThreshold = 1000;
        cv::String status = comparationResult >= detectionThreshold
                                ? "Movement detected"
                                : "No movement";

        cv::putText(image, "Status: " + status, cv::Point(100, 30),
                    cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(250, 250, 250), 1,
                    cv::FILLED);

        cv::imshow("image", image);

        key = cv::waitKey(30);
        if (key == 27)
            break;

        lastHistImgR = histImgR.clone();
        currentHistSum = 0;
        lastHistSum = 0;
    }

    return 0;
}