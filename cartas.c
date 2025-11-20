#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "cartas.h"
#include "entidade.h"
#include "inimigo.h"
#include "personagem.h"

// Função para gerar número aleatório entre min e max (inclusive)
int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}     

// Função que calcula o efeito de uma carta com base no custo de energia
int calcularEfeitoCarta(int c) {
    int min[] = {1, 5, 10, 15};     // Valor mínimo do efeito por custo
    int max[] = {5, 10, 15, 30};    // Valor máximo do efeito por custo
    return randInt(min[c], max[c]);
}

// FUNÇÕES DE CRIAÇÃO E CONFIGURAÇÃO DO BARALHO

void embaralharCartas(Carta baralho[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        int j = rand() % tamanho; // Escolhe uma posição
        Carta temp = baralho[i];
        baralho[i] = baralho[j];
        baralho[j] = temp;
        // Troca a carta de posição i com a carta de posição j
        // Todas as cartas são trocadas pelo menos uma vez de i até tamanho
    }
}

void adicionarCartas(Carta baralho[], int *indexBaralho, TipoCarta tipo, int quantidade, int minCusto, int maxCusto) {
    // Função que adiciona múltiplas cartas do mesmo tipo ao baralho
    // Pode ser de custo fixo (minCusto == maxCusto) ou aleatório (minCusto != maxCusto)
    for (int i = 0; i < quantidade; i++) {
        int custo;
        if (minCusto == maxCusto) {
            custo = minCusto; // custo fixo
        } else {
            custo = randInt(minCusto, maxCusto); // custo aleatório dentro do intervalo
        }
        if (*indexBaralho >= MAX_CARTAS) break;
        baralho[*indexBaralho].tipo = tipo;       // define o tipo da carta
        baralho[*indexBaralho].custo = custo;     // define o custo da carta
        baralho[*indexBaralho].efeito = calcularEfeitoCarta(custo); // define o efeito da carta
        (*indexBaralho)++; if (*indexBaralho > MAX_CARTAS) *indexBaralho = MAX_CARTAS; // incrementa o índice para a próxima carta
    }
}

void criarBaralhoInicial(Carta baralho[], int tam_baralho) {
    int indexBaralho = 0; // posição da carta no baralho

    // Adiciona cartas do tipo ESPECIAL (2 cartas, custo fixo 0)
    adicionarCartas(baralho, &indexBaralho, ESPECIAL, 2, 0, 0);

    // Adiciona cartas do tipo ATAQUE
    // Cartas fixas: 1 de custo 0, 3 de custo 1, 1 de custo 2, 1 de custo 3
    adicionarCartas(baralho, &indexBaralho, ATAQUE, 1, 0, 0);
    adicionarCartas(baralho, &indexBaralho, ATAQUE, 3, 1, 1);
    adicionarCartas(baralho, &indexBaralho, ATAQUE, 1, 2, 2);
    adicionarCartas(baralho, &indexBaralho, ATAQUE, 1, 3, 3);

    // Cartas restantes de ATAQUE, custo aleatório entre 0 e 3
    adicionarCartas(baralho, &indexBaralho, ATAQUE, 4, 0, 3);

    // Adiciona cartas do tipo DEFESA
    // Cartas fixas: 1 de custo 0, 3 de custo 1, 1 de custo 2, 1 de custo 3
    adicionarCartas(baralho, &indexBaralho, DEFESA, 1, 0, 0);
    adicionarCartas(baralho, &indexBaralho, DEFESA, 3, 1, 1);
    adicionarCartas(baralho, &indexBaralho, DEFESA, 1, 2, 2);
    adicionarCartas(baralho, &indexBaralho, DEFESA, 1, 3, 3);

    // Cartas restantes de DEFESA, custo aleatório entre 0 e 2
    adicionarCartas(baralho, &indexBaralho, DEFESA, 4, 0, 2);

    // Embaralha todas as cartas do baralho
    embaralharCartas(baralho, tam_baralho);
}

// Função que aplica o efeito da carta sobre atacante (+EFEITO) ou alvo (-EFEITO)
// Pode ser utilizada tanto Por Personagem Quanto Por Inimigos
void executarAcao(Carta carta, Entidade *atacante, Entidade *alvo) {
    switch(carta.tipo) {
        case ATAQUE:
            if (alvo->escudo > 0) {
                // Calcula quanto dano "sobra" depois de atingir o escudo
                int danoRestante = carta.efeito - alvo->escudo;

                // Reduz o escudo do inimigo pelo valor do efeito da carta
                alvo->escudo -= carta.efeito;

                // Se o escudo ficou negativo, zera (não pode haver escudo negativo)
                if (alvo->escudo< 0) alvo->escudo = 0;

                // Se ainda restou dano após quebrar o escudo, aplica o restante na vida
                if (danoRestante > 0) alvo->vida -= danoRestante;
            } else {
                // Se o inimigo não tem escudo, o dano vai direto na vida
                alvo->vida -= carta.efeito;
            }

            // Garante que a vida do inimigo não fique negativa
            if (alvo->vida < 0) alvo->vida = 0;
            break;

        case DEFESA:
            atacante->escudo += carta.efeito; // Aumenta o escudo do personagem
            break;
        
        case ESPECIAL:
            break;

    }
}