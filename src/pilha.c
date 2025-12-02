#include "pilha.h"
#include "cartas.h"

// Inicializa a pilha, começando vazia
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Retorna 1 se a pilha estiver vazia, 0 caso contrário
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Retorna a quantidade atual de cartas na pilha
int tamPilha(Pilha *p) {
    return p->topo + 1;
}

// Retorna 1 se a pilha estiver cheia, 0 caso contrário
int pilhaCheia(Pilha *p) {
    return tamPilha(p) == MAX_CARTAS;
}

// Adiciona uma carta no topo da pilha
void empilharCarta(Pilha *p, Carta c) {
    p->topo++;
    p->cartas[p->topo] = c;
}

// "Remove"(Só decrementa o topo) e retorna a carta do topo da pilha
Carta desempilhar(Pilha *p) {
    Carta cartaRetirada = p->cartas[p->topo];
    p->topo--;
    return cartaRetirada;
}

// Embaralha todas as cartas da pilha
void embaralharPilha(Pilha *p) {
    embaralharCartas(p->cartas, tamPilha(p));
}

// Cria a pilha de compras a partir do baralho inicial
void criarPilhaDeCompras(Pilha *compras, Carta baralho[], int tamanho) {
    inicializarPilha(compras);

    for (int i = 0; i < tamanho; i++) {
        empilharCarta(compras, baralho[i]);
    }

    embaralharPilha(compras);
}

// Move todas as cartas de uma pilha para outra (ex: descarte → compras)
void moverPilha(Pilha *origem, Pilha *destino) {
    while (!pilhaVazia(origem)) {
        empilharCarta(destino, desempilhar(origem));
    }

    embaralharPilha(destino);
}


