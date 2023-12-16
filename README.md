# Grafos Temporais Dinâmicos com Fechamento Transitivo Temporal
## Visão Geral

Este projeto tem como objetivo implementar uma estrutura de dados dinâmica para lidar com grafos temporais, focando especificamente no fechamento transitivo temporal (TTC). A implementação é inspirada no trabalho do Dr. Marcelo K. Albertini, meu orientador, disponível em [seu repositório GitHub](https://github.com/albertiniufu/dynamictemporalgraph). O conceito subjacente e a estrutura são baseados no artigo de pesquisa intitulado "A dynamic data structure for temporal reachability with unsorted contact insertions", de Luiz F. A. Brito, Marcelo K. Albertini, Arnaud Casteigts e Bruno A. N. Travençolo.

## Artigo de Pesquisa

O artigo de pesquisa original pode ser encontrado [aqui](https://link.springer.com/article/10.1007/s13278-021-00851-y). O artigo aborda os desafios de gerenciar informações de alcance temporal em grafos dinâmicos, onde contatos podem ser inseridos em uma ordem arbitrária, até mesmo não cronológica. Esse cenário está alinhado com situações em que as informações são coletadas retrospectivamente, como na reconstrução de cadeias de contaminação entre pessoas em estudos epidemiológicos.

## Fechamento Transitivo Temporal (TTC)

Grafos temporais representam interações entre entidades ao longo do tempo, onde as interações podem ser diretas ou indiretas por meio de caminhos temporais. A principal contribuição deste projeto é a implementação do fechamento transitivo temporal (TTC), uma generalização do fechamento transitivo que mantém informações de alcance relativas a todos os intervalos temporais aninhados sem armazenar explicitamente esses intervalos ou os próprios caminhos.

## Estrutura do Código

O código é implementado em C++ e utiliza uma matriz de árvores balanceadas de RTuplas. A classe RTuple encapsula informações sobre um contato, incluindo o nó de origem, o nó de destino, o momento de início do contato (tMinus), o momento de término do contato (tPlus) e um peso opcional.

## Uso

A estrutura de dados implementada permite responder a consultas de alcance de maneira eficiente e suporta a adição de contatos em qualquer ordem. 

## Contexto de Pesquisa

Este projeto faz parte de uma pesquisa em andamento sobre grafos temporais dinâmicos, explorando aplicações em diversos campos, como redes de comunicação e epidemiologia. O objetivo é contribuir para a compreensão e o gerenciamento eficiente de interações temporais entre entidades ao longo do tempo.