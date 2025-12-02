#ifndef TURNO_JOGADOR_H
#define TURNO_JOGADOR_H

#include "combate.h"

void iniciarTurnoJogador(Combate *combate);
void AjustarSelecaoInimigo(Combate* combate);
void comprar5Cartas(Pilha *pilhaCompras, Pilha *pilhaDescarte, Mao *mao);
int jogarCarta(Combate *combate, int indiceCarta, int alvo);
void descartarMao(Combate *combate);
void encerrarTurnoJogador(Combate *combate);

#endif