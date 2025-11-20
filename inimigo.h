#ifndef INIMIGO_H
#define INIMIGO_H

#include "cartas.h"
#include "entidade.h"



// Enum que define a força do inimigo
typedef enum {
    FRACO,   
    FORTE    
} ForcaInimigo;

// Estrutura que representa um inimigo no jogo
typedef struct {
    Entidade base;        // Vida e escudo ficam aqui
    ForcaInimigo forca;   // Tipo do inimigo (FRACO ou FORTE)
    int numAcoes;         // Quantidade total de ações da IA
    Carta acoes[3];       // Lista de ações (máximo 3 para inimigos fortes)
    int acaoAtual;        // Índice da próxima ação a ser executada
    int vidaMax;
} Inimigo;

int gerarVidaInimigo(Inimigo inimigo);
ForcaInimigo gerarForcaInimigo();
int gerarNumeroDeAcoes(Inimigo inimigo);
void gerarVetorAcoes(Carta acoes[], int numAcoes, ForcaInimigo forca);
Inimigo gerarInimigoAleatorio();

#endif