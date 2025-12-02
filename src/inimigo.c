#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "inimigo.h"
#include "cartas.h"

// Gera a vida do inimigo com base em sua força
int gerarVidaInimigo(Inimigo inimigo) {
    if (inimigo.forca == FRACO)
        return randInt(10, 30);   // Inimigos fracos: vida entre 10 e 30
    else
        return randInt(40, 100);  // Inimigos fortes: vida entre 40 e 100
}

// Define aleatoriamente se o inimigo será fraco ou forte
ForcaInimigo gerarForcaInimigo() {
    int chance = randInt(1, 100);
    if (chance <= 95)
        return FRACO;  // 95% de chance de ser fraco
    else
        return FORTE;  // 5% de chance de ser forte
}

// Define quantas ações o inimigo terá com base na força
int gerarNumeroDeAcoes(Inimigo inimigo) {
    if (inimigo.forca == FRACO)
        return randInt(1, 2);  // Inimigo fraco: 1 ou 2 ações
    else if (inimigo.forca == FORTE){
        return randInt(2, 3);  // Inimigo forte: 2 ou 3 ações
    }
        return 1;
}


// Função que gera o vetor de ações (ataques e defesas) de um inimigo
// As ações variam conforme o tipo de inimigo (fraco ou forte)
void gerarVetorAcoes(Carta acoes[], int numAcoes, ForcaInimigo forca) {

    // Decide quantas ações serão de ataque e quantas de defesa
    int qtdAtaques = randInt(1, numAcoes); // Garante pelo menos 1 ataque
    int indice;

    // Preenche o início do vetor com ações de ataque
    for (indice = 0; indice < qtdAtaques; indice++)
        acoes[indice].tipo = ATAQUE;

    // Preenche o restante do vetor com ações de defesa
    for (indice = qtdAtaques; indice < numAcoes; indice++)
        acoes[indice].tipo = DEFESA;
    
    
    // Configuração para inimigo fraco
    if (forca == FRACO) {   
        for (int i = 0; i < numAcoes; i++) {
            // Inimigos fracos usam cartas com custo entre 0 e 1
            acoes[i].custo = randInt(0, 1);

            // Calcula o efeito da carta com base no custo
            acoes[i].efeito = calcularEfeitoCarta(acoes[i].custo);
        }
    }
    
    // Configuração para inimigo forte
    else if (forca == FORTE) {
        int custo1Usado = 0; // Controle: só pode haver UMA carta com custo 1
        int custo = 0;

        for (int i = 0; i < numAcoes; i++) {
            if (custo1Usado == 1) {
                // Se já foi usada uma carta de custo 1,
                // as próximas devem ser de custo 2 ou 3
                custo = randInt(2, 3);
            } else {
                // Caso contrário, ainda pode sair custo 1, 2 ou 3
                custo = randInt(1, 3);
            }

            // Define o custo da carta
            acoes[i].custo = custo;

            // Calcula o efeito da carta conforme o custo
            acoes[i].efeito = calcularEfeitoCarta(acoes[i].custo);

            // Marca que já houve uma carta de custo 1, se for o caso
            if (acoes[i].custo == 1)
                custo1Usado = 1;
        }
    }
    // Embaralha as ações no vetor(ataques e defesas aleatórios)--> Ações funcionam como cartas
    embaralharCartas(acoes,numAcoes);
    
}


// Cria um inimigo aleatório completo
Inimigo gerarInimigoAleatorio() {
    Inimigo inimigo;

    // Define características básicas
    inimigo.forca = gerarForcaInimigo();
    inimigo.base.vida = gerarVidaInimigo(inimigo);
    inimigo.base.escudo = 0;
    inimigo.numAcoes = gerarNumeroDeAcoes(inimigo);
    inimigo.acaoAtual = 0;
    inimigo.vidaMax = inimigo.base.vida;

    // Gera as ações (ataque/defesa) e as aplica ao inimigo gerado
    gerarVetorAcoes(inimigo.acoes, inimigo.numAcoes,inimigo.forca);

    return inimigo;
}
