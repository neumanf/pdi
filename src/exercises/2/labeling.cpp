#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char **argv) {
    cv::Mat image, realce;
    int width, height;

    cv::Point p;
    image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cout << "image nao carregou corretamente\n";
        return (-1);
    }

    width = image.cols;
    height = image.rows;
    std::cout << width << "x" << height << std::endl;

    p.x = 0;
    p.y = 0;

    // remove objetos que tocam na borda
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // se o pixel não for um pixel de borda, continue
            if ((i > 0 && i < height - 1) && (j > 0 && j < width - 1)) {
                continue;
            }

            p.x = j;
            p.y = i;

            // se houver uma bolha, a remove
            if (image.at<uchar>(i, j) == 255) {
                cv::floodFill(image, p, 0);
            }
        }
    }

    // reseta o ponto
    p.x = 0;
    p.y = 0;

    // muda a cor do fundo
    floodFill(image, p, 1);

    // conta quantas bolhas existem
    int numberOfObjects = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 255) {
                numberOfObjects++;
                p.x = j;
                p.y = i;
                floodFill(image, p, 100);
            }
        }
    }

    // reseta o ponto
    p.x = 0;
    p.y = 0;

    // conta quantas bolhas com buracos existem
    int numberOfObjectsWithHoles = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 0 &&
                image.at<uchar>(i, j - 1) == 100) {
                numberOfObjectsWithHoles++;
                p.x = j;
                p.y = i;
                floodFill(image, p, 150);
                p.x = j - 1;
                floodFill(image, p, 50);
            }
        }
    }

    std::cout << "a figura tem " << numberOfObjects << " bolhas e "
              << numberOfObjectsWithHoles << " bolhas com buracos.\n";
    // cv::equalizeHist(image, realce);
    cv::imshow("image", image);
    // cv::imshow("realce", realce);
    // cv::imwrite("labeling.png", image);
    cv::waitKey();
    return 0;
}
