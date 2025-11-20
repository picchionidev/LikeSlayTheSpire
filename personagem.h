#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include "entidade.h"
#include "cartas.h"

// Struct para armazenar informações do personagem
typedef struct {
    Entidade base;        // Vida e escudo ficam aqui
} Personagem;

#endif