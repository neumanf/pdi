#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

int main(int argvc, char **argv) {
    cv::VideoCapture cap("./exercises/5/norway.mp4");
    int frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    cv::VideoWriter output("./exercises/5/norway_output.avi",
                           cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 15,
                           cv::Size(frameWidth, frameHeight));

    float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                     0.1111, 0.1111, 0.1111, 0.1111};

    cv::Mat mask = cv::Mat(3, 3, CV_32F, media);

    if (!cap.isOpened()) {
        std::cout << "Cannot open the video file. \n";
        return -1;
    }

    std::cout << "> Processing video, please wait...\n";

    while (1) {
        cv::Mat frame, filteredFrame, frameTop;
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        cv::filter2D(frame, filteredFrame, frame.depth(), mask, cv::Point(1, 1),
                     0);
        for (int i = 0; i < 5; i++) {
            cv::filter2D(filteredFrame, filteredFrame, frame.depth(), mask,
                         cv::Point(1, 1), 0);
        }
        filteredFrame.copyTo(frameTop);

        for (int i = 0; i < filteredFrame.rows; i++) {
            double d = 100.0;
            double l1 = -200.0 + round(frame.size().height / 2);
            double l2 = 200.0 + round(frame.size().height / 2);
            double alpha = (0.5) * (tanh((i - l1) / d) - tanh((i - l2) / d));

            addWeighted(frame.row(i), alpha, frameTop.row(i), 1 - alpha, 0,
                        filteredFrame.row(i));
        }

        output.write(filteredFrame);
    }

    std::cout << "> Video processed succesfully.\n";

    cap.release();
    output.release();

    return 0;
}