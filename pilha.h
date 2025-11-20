#ifndef PILHA_H
#define PILHA_H

#include "cartas.h"

// Representa uma pilha de cartas (por exemplo, compra ou descarte)
typedef struct {
    Carta cartas[MAX_CARTAS];  // Vetor que guarda as cartas
    int topo;                  // Índice da próxima posição livre (quantas cartas há)
} Pilha;

void inicializarPilha(Pilha *p);
int pilhaVazia(Pilha *p);
int tamPilha(Pilha *p);
int pilhaCheia(Pilha *p);
void empilharCarta(Pilha *p, Carta c);
Carta desempilhar(Pilha *p);
void embaralharPilha(Pilha *p);
void criarPilhaDeCompras(Pilha *compras, Carta baralho[], int tamanho);
void moverPilha(Pilha *origem, Pilha *destino);

#endif