---
layout: post
title:  "[EX-02] Processo de rotulação e algoritmo de contagem"
author: neumanf
date:   2022-04-16 09:00:00 +0000
categories: pdi
---

## Exercício 2.1

### Objetivo

Observando-se o programa `labeling.cpp` como exemplo, é possível verificar que caso existam mais de 255 objetos na cena, o processo de rotulação poderá ficar comprometido. Identifique a situação em que isso ocorre e proponha uma solução para este problema.

### Solução

O problema ocorre devido ao fato do algoritmo usar a variável que conta a quantidade de objetos, `nobjects`, como tonalidade de cinza a ser preenchida pelo `floodFill()`, assim, como 255 é o número máximo representável com 8 bits do tipo `uchar`, com uma quantidade de objetos maior que essa, o algoritmo irá gerar um resultado errôneo.

Uma possível solução é utilizar o sistema RGB, sabido que será possível contar `255*255*255 = 1.6581.375` objetos na cena.