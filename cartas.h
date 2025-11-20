#ifndef CARTAS_H
#define CARTAS_H
#define MAX_CARTAS 20
#define MAX_MAO 5

#include "entidade.h"

// Enum para tipos de cartas
typedef enum {
    ATAQUE,
    DEFESA,
    ESPECIAL   // Carta especial (lógica a implementar)
} TipoCarta;

// Struct que representa uma carta, contendo tipo e custo
typedef struct {
    TipoCarta tipo;
    int custo;
    int efeito;
} Carta;



// Protótipos de funções
int randInt(int min, int max);
int calcularEfeitoCarta(int c);
void executarAcao(Carta carta, Entidade *atacante, Entidade *alvo);
void embaralharCartas(Carta baralho[], int tamanho);
void adicionarCartas(Carta baralho[], int *indexBaralho, TipoCarta tipo, int quantidade, int minCusto, int maxCusto);
void criarBaralhoInicial(Carta baralho[], int tam_baralho);

#endif