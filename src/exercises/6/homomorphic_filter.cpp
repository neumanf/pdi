#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

cv::Mat image;

// valores utilizados no filtro e trackbars
float gammaH = 0, gammaL = 0, c = 0, d0 = 0;
float gammaHMax = 100, gammaLMax = 100, cMax = 250, d0Max = 100,
      trackbarMax = 100;
int gammaHSlider = 0, gammaLSlider = 0, cSlider = 0, d0Slider = 0;

// troca os quadrantes da imagem da DFT
void swapImageQuadrants(cv::Mat &image) {
    cv::Mat tmp, A, B, C, D;

    // se a imagem tiver tamanho impar, recorta a regiao para
    // evitar cópias de tamanho desigual
    image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));
    int cx = image.cols / 2;
    int cy = image.rows / 2;

    // reorganiza os quadrantes da transformada
    // A B   ->  D C
    // C D       B A
    A = image(cv::Rect(0, 0, cx, cy));
    B = image(cv::Rect(cx, 0, cx, cy));
    C = image(cv::Rect(0, cy, cx, cy));
    D = image(cv::Rect(cx, cy, cx, cy));

    // A <-> D
    A.copyTo(tmp);
    D.copyTo(A);
    tmp.copyTo(D);

    // C <-> B
    C.copyTo(tmp);
    B.copyTo(C);
    tmp.copyTo(B);
}

void homomorphicFilter() {
    cv::Mat imaginaryInput, complexImage, multsp;
    cv::Mat padded, filter, mag;
    cv::Mat imagegray, tmp;
    cv::Mat_<float> realInput, zeros;
    std::vector<cv::Mat> planos;

    // valores ideais dos tamanhos da imagem
    // para calculo da DFT
    int dft_M, dft_N;

    // identifica os tamanhos otimos para
    // calculo do FFT
    dft_M = cv::getOptimalDFTSize(image.rows);
    dft_N = cv::getOptimalDFTSize(image.cols);

    // realiza o padding da imagem
    cv::copyMakeBorder(image, padded, 0, dft_M - image.rows, 0,
                       dft_N - image.cols, cv::BORDER_CONSTANT,
                       cv::Scalar::all(0));

    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = cv::Mat_<float>::zeros(padded.size());

    // prepara a matriz complexa para ser preenchida
    complexImage = cv::Mat(padded.size(), CV_32FC2, cv::Scalar(0));

    // a função de transferencia (filtro de frequencia) deve ter o
    // mesmo tamanho e tipo da matriz complexa
    filter = complexImage.clone();

    // cria uma matriz temporária para criar as componentes real
    // e imaginaria do filtro ideal
    tmp = cv::Mat(dft_M, dft_N, CV_32F);

    // prepara o filtro homomorfico
    for (int i = 0; i < dft_M; i++) {
        for (int j = 0; j < dft_N; j++) {
            float d = std::pow(
                std::pow(i - dft_M / 2, 2) + std::pow(j - dft_N / 2, 2), 0.5);
            tmp.at<float>(i, j) =
                (gammaH - gammaL) *
                    (1 -
                     std::exp(-1 * c * (std::pow(d, 2) / std::pow(d0, 2)))) +
                gammaL;
        }
    }

    // cria a matriz com as componentes do filtro e junta
    // ambas em uma matriz multi-canal complexa
    cv::Mat comps[] = {tmp, tmp};
    cv::merge(comps, 2, filter);

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();

    // cria a compoente real
    realInput = cv::Mat_<float>(padded);

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    cv::merge(planos, complexImage);

    // calcula o dft
    cv::dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    swapImageQuadrants(complexImage);

    // aplica o filtro de frequencia
    cv::mulSpectrums(complexImage, filter, complexImage, 0);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria para modifica-las
    cv::split(complexImage, planos);

    // recompoe os planos em uma unica matriz complexa
    cv::merge(planos, complexImage);

    // troca novamente os quadrantes
    swapImageQuadrants(complexImage);

    // calcula a DFT inversa
    cv::idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    cv::split(complexImage, planos);

    // normaliza a parte real para exibicao
    cv::normalize(planos[0], planos[0], 0, 1, cv::NORM_MINMAX);

    cv::imshow("Filtro Homomorfico", planos[0]);
}

void on_trackbar_gamma_h(int, void *) {
    gammaH = gammaHSlider * gammaHMax / trackbarMax;
    homomorphicFilter();
}

void on_trackbar_gamma_l(int, void *) {
    gammaL = (float)gammaLSlider;
    gammaL = gammaLMax * gammaL / trackbarMax;
    homomorphicFilter();
}

void on_trackbar_c(int, void *) {
    c = cSlider * cMax / trackbarMax;
    homomorphicFilter();
}

void on_trackbar_d0(int, void *) {
    d0 = d0Slider * d0Max / trackbarMax;
    homomorphicFilter();
}

int main(int, char **) {
    char trackbarName[50];

    // carrega a imagem e a converte para escala de cinza
    image = cv::imread("./exercises/6/forest.jpg");
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::namedWindow("Filtro Homomorfico");

    // configura as trackbars
    sprintf(trackbarName, "gamma_H");
    cv::createTrackbar(trackbarName, "Filtro Homomorfico", &gammaHSlider,
                       trackbarMax, on_trackbar_gamma_h);

    sprintf(trackbarName, "gamma_L");
    cv::createTrackbar(trackbarName, "Filtro Homomorfico", &gammaLSlider,
                       trackbarMax, on_trackbar_gamma_l);

    sprintf(trackbarName, "C");
    cv::createTrackbar(trackbarName, "Filtro Homomorfico", &cSlider,
                       trackbarMax, on_trackbar_c);

    sprintf(trackbarName, "D0");
    cv::createTrackbar(trackbarName, "Filtro Homomorfico", &d0Slider,
                       trackbarMax, on_trackbar_d0);

    homomorphicFilter();

    cv::waitKey(0);

    return 0;
}