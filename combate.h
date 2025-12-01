#ifndef COMBATE_H
#define COMBATE_H

#include "personagem.h"
#include "inimigo.h"
#include "pilha.h"
#include "cartas.h"
#include "mao.h"
#include "constants.h"

typedef enum {
    TURNO_JOGADOR,
    TURNO_INIMIGOS
} TipoTurno;

typedef enum {
    SELECIONANDO_CARTA,
    SELECIONANDO_INIMIGO
} EstadoSelecao;

typedef struct {
    Personagem jogador;        // Jogador
    Inimigo inimigos[NUMINIMIGOS];// NUMINIMIGOS inimigos por combate
    Pilha pilhaCompras;       // Pilha de compras do jogador
    Pilha pilhaDescarte;      // Pilha de descarte do jogador
    Mao mao;                  // Mão atual do jogador
    TipoTurno turno;          // Variável para controlar o turno
    int alvoSelecionado;      // Inimigo a ser atacado
    int cartaSelecionada;     // índice da carta destacada
    EstadoSelecao estadoSelecao; // Modo atual de seleção(Carta ou Inimigo)
    int energia;              // Energia atual do jogador
} Combate;

void iniciarCombate(Combate *combate, Carta baralho[], int tamBaralho);
int combateTerminado(Combate *combate);

#endif
