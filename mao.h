#include "cartas.h"
// Struct para armazenar a mão do jogador
typedef struct {
    Carta cartas[MAX_MAO];  // cartas na mão
    int quantidade;         // quantas cartas estão atualmente na mão
} Mao;