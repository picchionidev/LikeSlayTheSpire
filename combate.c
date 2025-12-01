#include <stdlib.h>
#include <stdio.h>

#include "combate.h"
#include "pilha.h"
#include "cartas.h"
#include "inimigo.h"
#include "personagem.h"
#include "turno_jogador.h"
#include "turno_inimigos.h"
#include "constants.h"

void iniciarCombate(Combate *combate, Carta baralho[], int tamBaralho) {
    
    //Gera Inimigos e Garante que só tenha um Forte
    int fortes = 0;
    for (int i = 0; i < NUMINIMIGOS; i++) {
        combate->inimigos[i] = gerarInimigoAleatorio();

        if (combate->inimigos[i].forca == FORTE) {
            if (fortes == 0) {
                fortes++; // Primeiro forte permitido
            } else {
                // Substituir por um inimigo que não seja forte
                while (combate->inimigos[i].forca == FORTE) {
                    combate->inimigos[i] = gerarInimigoAleatorio();
                }
            }
        }
    }


    // Criar pilha de compras do jogador a partir do baralho
    criarPilhaDeCompras(&combate->pilhaCompras, baralho, tamBaralho);

    // Inicializar pilha de descarte vazia
    inicializarPilha(&combate->pilhaDescarte);

    // Zerar a mão do jogador
    combate->mao.quantidade = 0;

    combate->alvoSelecionado = 0;  // Sempre começa selecionando o primeiro inimigo
	
	//Inicialização do turno
	combate->turno = TURNO_JOGADOR;

}

// Verifica se o combate terminou
// Retorna:-1(Jogador Perdeu),1(Jogador Ganhou),0(Combate em Andamento)
int combateTerminado(Combate *combate) {
    // Verifica se o jogador morreu
    if (combate->jogador.base.vida <= 0)
        return -1; // Jogador perdeu

    // Verifica se todos os inimigos morreram
    int todosMortos = 1;
    for (int i = 0; i < NUMINIMIGOS; i++) {
        if (combate->inimigos[i].base.vida > 0) {
            todosMortos = 0;
            break;
        }
    }
    if (todosMortos)
        return 1; // Jogador venceu

    return 0; // Combate continua
}
