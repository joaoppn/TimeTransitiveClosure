<<<<<<< HEAD
# Grafos Temporais Dinâmicos: Estruturas de Fechamento Transitivo Temporal (TTC) Sob Análise

## Visão Geral

Este projeto tem como objetivo implementar uma estrutura de dados dinâmica para lidar com grafos temporais dinâmicos, concentrando-se especificamente no timed transitive closure (TTC). A implementação é inspirada no trabalho do Dr. Marcelo K. Albertini, meu orientador, disponível em [seu repositório no GitHub](https://github.com/albertiniufu/dynamictemporalgraph). O conceito subjacente e a estrutura são baseados no artigo de pesquisa intitulado "A dynamic data structure for temporal reachability with unsorted contact insertions", de Luiz F. A. Brito, Marcelo K. Albertini, Arnaud Casteigts e Bruno A. N. Travençolo.

## Artigo de Referência

O artigo de pesquisa original pode ser encontrado [aqui](https://link.springer.com/article/10.1007/s13278-021-00851-y). O artigo aborda os desafios de gerenciar informações de alcance temporal em grafos dinâmicos, onde contatos podem ser inseridos em uma ordem arbitrária, até mesmo não cronológica. Esse cenário está alinhado com situações em que as informações são coletadas retrospectivamente, como na reconstrução de cadeias de contaminação entre pessoas em estudos epidemiológicos.

## Objetivo da Pesquisa

Este projeto vai além da simples implementação; ele analisa criticamente duas abordagens distintas para construir uma estrutura de Fechamento Transitivo Temporal (TTC) em grafos temporais dinâmicos. A pesquisa visa entender as vantagens e desvantagens de duas implementações específicas: uma construída a partir de uma matriz e outra a partir de uma tabela de dispersão (HashTable), representada por um `unordered_map` em C++.

## Timed Transitive Closure (TTC)

Grafos temporais representam interações entre entidades ao longo do tempo, onde as interações podem ser diretas ou indiretas por meio de caminhos temporais. A principal contribuição deste projeto é a implementação da timed transitive closure (TTC), uma generalização do fechamento transitivo que mantém informações de alcance relativas a todos os intervalos temporais aninhados sem armazenar explicitamente esses intervalos ou os próprios caminhos.

## Estrutura do Código

O código é implementado em C++ e utiliza a classe RTuple para armazenar informações essenciais. A classe RTuple encapsula dados sobre um contato temporal, como nós de origem e destino, o momento de início (tMinus) e o momento de término (tPlus). Ambas as implementações fazem uso da estrutura de conjunto 'set' da biblioteca padrão (STL) em C++, representando uma árvore balanceada. Essa escolha proporciona uma eficiente ordenação de contatos temporais entre dois nós.

### Implementação com Matriz

Na implementação com matriz, cada entrada corresponde a um par de nós, e o conjunto associado armazena RTuplas representando contatos temporais entre esses nós.

### Implementação com HashTable

A implementação com HashTable utiliza um `unordered_map`, da biblioteca padrão (STL), para atingir o mesmo propósito. Nesse caso, a chave é um par de nós, e o valor é um conjunto de RTuplas.

Ambas as implementações apresentam tempos de resposta eficientes para consultas de alcance e suportam a adição de contatos em qualquer ordem. 


## Contexto da Pesquisa

Este projeto faz parte de uma pesquisa em andamento sobre grafos temporais dinâmicos, explorando aplicações em diversos campos, como redes de comunicação e epidemiologia. O objetivo é contribuir para a compreensão e o gerenciamento eficiente de interações temporais entre entidades ao longo do tempo.
=======
