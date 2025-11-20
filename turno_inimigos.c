#include "turno_inimigos.h"
#include "cartas.h"  
#include "pilha.h"  
#include <stdio.h> 
#include "constants.h"  

void turnoInimigos(Combate *combate) {
    // No início do turno, todos os inimigos perdem seu escudo acumulado
    for (int i = 0; i < NUMINIMIGOS; i++) {
        combate->inimigos[i].base.escudo = 0;
    }

    // Cada inimigo realiza sua ação da vez
    for (int i = 0; i < NUMINIMIGOS; i++) {
        // Executa a ação atual do inimigo contra o jogador
        executarAcao(
            combate->inimigos[i].acoes[combate->inimigos[i].acaoAtual],
            &combate->inimigos[i].base,
            &combate->jogador.base
        );

        // Avança para a próxima ação do inimigo
        combate->inimigos[i].acaoAtual++;

        // Se chegou ao final da lista de ações, volta para a primeira ação (ciclo contínuo)
        if (combate->inimigos[i].acaoAtual >= combate->inimigos[i].numAcoes)
            combate->inimigos[i].acaoAtual = 0;
    }
		
		// Troca para o turno do jogador
		combate->turno = TURNO_JOGADOR;
}