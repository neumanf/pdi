---
layout: post
title:  "[EX-01] Região negativa e troca de regiões"
author: neumanf
date:   2022-04-15 15:00:00 +0000
categories: pdi
---

## Exercício 1.1

### Objetivo

Utilizando o programa `exemplos/pixels.cpp` como referência, implemente um programa regions.cpp. Esse programa deverá solicitar ao usuário as coordenadas de dois pontos P1 e P2 localizados dentro dos limites do tamanho da imagem e exibir que lhe for fornecida. Entretanto, a região definida pelo retângulo de vértices opostos definidos pelos pontos P1 e P2 será exibida com o negativo da imagem na região correspondente.

### Implementação

Para os testes, iremos utilizar uma imagem de resolução 800x600 denominada `mountain.png`. Primeiramente, precisamos ler a imagem do disco em modo de escala de cinza, para isso, utilizamos o método `imread()` do OpenCV com o argumento  `cv::IMREAD_GRAYSCALE`. Em seguida, verificamos se a imagem foi carregada corretamente, e caso não, retornamos um erro. Se a operação foi executada com sucesso, entretando, seguimos com a execução do programa, obtemos as dimensões da imagem e a armazenamos em uma variável chamada `imageSize`.

`regions.cpp`
{% highlight cpp %}
[...]
cv::Mat image =
    cv::imread("../assets/images/mountain.png", cv::IMREAD_GRAYSCALE);

if (!image.data) {
    std::cout << "Error while opening the image.\n";
    return -1;
}

cv::Size imageSize = image.size();
[...]
{% endhighlight %}

Então, obtemos os pontos fornecidos pelo usuário, através da função `getPointCoordinates()` e os validamos, com `isAValidPoint()`. A primeira função, utiliza `std::cout` e `std::cin` para printar e coletar informações do usuário no terminal, guardando-as em uma variável do tipo `Point`, as quais guarda as posições x e y. Já a segunda função, verifica se o ponto em questão está dentro do limite das dimensões da imagem, especificado pela variável `imageSize`.

`regions.cpp`
{% highlight cpp %}
[...]
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
[...]
{% endhighlight %}

Após isso, percorremos a matrix da imagem horizontalmente e verticalmente, de acordo com os pontos fornecidos, e para cada ponto, substituimos seu valor pelo seu complemento, com base no total de 255 tons que podemos representar utilizando 8 bits. Por fim, usamos `imshow()` para mostrar a imagem resultante.

`regions.cpp`
{% highlight cpp %}
[...]
for (int i = p1.x; i < p2.x; i++) {
    for (int j = p1.y; j < p2.y; j++) {
        image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
    }
}

cv::imshow("window", image);
cv::waitKey();
[...]
{% endhighlight %}

### Resultados

![Imagem original](../assets/images/mountain.png)
*Imagem original (mountain.png)*

Utilizando os valores

```bash
$ make regions && ./regions

> Point 1 coordinates
x: 0
y: 0

> Point 2 coordinates
x: 500
y: 700
```

Obtemos a seguinte imagem:

![Imagem processada](../src/exercises/1/output_regions.png)
*Imagem processada*