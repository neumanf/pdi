---
layout: post
title: "[EX-05] Tilt Shift"
author: neumanf
date: 2022-05-14 09:00:00 +0000
categories: pdi
---

## Exercício 5.1

### Objetivo

Utilizando o programa `exemplos/addweighted.cpp` como referência, implemente um programa `tiltshift.cpp`. Três ajustes deverão ser providos na tela da interface:
- um ajuste para regular a altura da região central que entrará em foco;
- um ajuste para regular a força de decaimento da região borrada;
- um ajuste para regular a posição vertical do centro da região que entrará em foco. Finalizado o programa, a imagem produzida deverá ser salva em arquivo.

### Implementação

Para realizar o efeito de borramento, primeiramente foi aplicado o filtro da média na imagem original e seu resultado armazenado em uma nova matriz denominada `filteredImage`.

`tiltshift.cpp`
{% highlight cpp linenos %}
[...]
float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                    0.1111, 0.1111, 0.1111, 0.1111};

cv::Mat mask = cv::Mat(3, 3, CV_32F, media);

cv::filter2D(image, filteredImage, image.depth(), mask, cv::Point(1, 1), 0);

filteredImage.copyTo(imageTop);
[...]
{% endhighlight %}

O programa conta com 3 _sliders_, sendo eles: o ajuste para regular a altura da região central que entrará em foco, o ajuste para regular a força de decaimento da região borrada e o ajuste para regular a posição vertical do centro da região que entrará em foco. Cada slider depende de um _callback_, de seu valor inicial/atual e máximo. Esses são definidos da seguinte maneira:

`tiltshift.cpp`
{% highlight cpp linenos %}
[...]
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
[...]
{% endhighlight %}

A função `on_trackbar_decay` é responsável por mudar o decaimento, e utiliza como base a equação fornecida no tutorial, que define a ponderação da imagem, `alpha`. Para cada linha da imagem, o alpha é calculado e o resultado é combinado utilizando a função `addWeighted`, dada no exemplo. A equação que define alpha necessita de dois valores, l1 e l2, que representam as linhas cujo valor de alpha assume valor em torno de 0,5. Os demais sliders atribuem à essas variáveis valores de acordo com que o valor do slider seja modificado, e ao final, chamam a função de decaimento, para que a nova imagem seja mostrada em tela.

`tiltshift.cpp`
{% highlight cpp linenos %}
[...]
void on_trackbar_decay(int, void *) {
    for (int i = 0; i < filteredImage.rows; i++) {
        double d = (double)decay_slider;
        double alpha = (0.5) * (tanh((i - l1) / d) - tanh((i - l2) / d));

        cv::addWeighted(image.row(i), alpha, imageTop.row(i), 1 - alpha, 0,
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
[...]
{% endhighlight %}

### Resultados

![Imagem original_](../src/exercises/5/alaska.png)
_Imagem original_

Selecionando a altura como 75, decaimento como 100 e a posição do centro como 100, obtemos um foco na casa principal.

![Imagem filtrada demo](../src/exercises/5/output.png)
_Imagem filtrada demo_

O resultado é a seguinte imagem:

![Imagem filtrada](../src/exercises/5/raw_output.png)
_Imagem filtrada_
