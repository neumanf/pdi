---
layout: post
title:  "[EX-06] Filtro Homomófico"
author: neumanf
date:   2022-06-11 10:00:00 +0000
categories: pdi
---

## Exercício 6

### Objetivo

Utilizando o programa `exemplos/dft.cpp` como referência, implemente o filtro homomórfico para melhorar imagens com iluminação irregular. Crie uma cena mal iluminada e ajuste os parâmetros do filtro homomórfico para corrigir a iluminação da melhor forma possível. Assuma que a imagem fornecida é em tons de cinza.

### Implementação

Primeiramente, definimos variáveis globais para armazenar a imagem inicial, os valores dos filtros e seus valores iniciais e máximos de suas _trackbars_.

`homomorphic_filter.cpp`
{% highlight cpp %}
[...]
cv::Mat image;

// valores utilizados no filtro e trackbars
float gammaH = 0, gammaL = 0, c = 0, d0 = 0;
float gammaHMax = 100, gammaLMax = 100, cMax = 250, d0Max = 100,
      trackbarMax = 100;
int gammaHSlider = 0, gammaLSlider = 0, cSlider = 0, d0Slider = 0;
[...]
{% endhighlight %}

Na função principal, carregamos a imagem original, a convertemos para escala de cinza,  criamos a janela e as _trackbars_ responsáveis por mudar as variáveis do filtro homomórfico.

`homomorphic_filter.cpp`
{% highlight cpp %}
[...]
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
[...]
{% endhighlight %}

A função `homomorphicFilter` descreve o filtro homomórfico, baseando-se na implementação de `dft.cpp`. A característica que as diferenciam é a troca do filtro passa-baixas ideal para o filtro homomórfico, que deriva da equação a seguir:

![equacao](https://i.stack.imgur.com/VSKA5.png)

`homomorphic_filter.cpp`
{% highlight cpp %}
[...]
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
[...]
{% endhighlight %}

### Resultados

![Imagem original](../src/exercises/6/forest.jpg)
*Imagem original*

Escolhendo os valores `gammaH = 25`, `gammaL = 10`, `C = 25` e `D0 = 35`, através dos _sliders_, obtemos a seguinte imagem:

![Imagem filtrada](../src/exercises/6/output.png)
*Imagem filtrada com o filtro homomórfico*
