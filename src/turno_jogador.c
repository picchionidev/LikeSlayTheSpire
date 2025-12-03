#include <stdio.h>      
#include "turno_jogador.h"
#include "pilha.h"       
#include "cartas.h" 

void AjustarSelecaoInimigo(Combate* combate) {
    int atual = combate->alvoSelecionado;

    // Se ainda estiver dentro do range e vivo, mantém
    if (atual >= 0 && atual < NUMINIMIGOS &&
        combate->inimigos[atual].base.vida > 0)
        return;

    // Tenta achar um vivo à DIREITA
    for (int i = atual + 1; i < NUMINIMIGOS; i++) {
        if (combate->inimigos[i].base.vida > 0) {
            combate->alvoSelecionado = i;
            return;
        }
    }

    // Se não achou, tenta à ESQUERDA
    for (int i = atual - 1; i >= 0; i--) {
        if (combate->inimigos[i].base.vida > 0) {
            combate->alvoSelecionado = i;
            return;
        }
    }

    // Nenhum encontrado → sem alvo
    combate->alvoSelecionado = -1;
}

    
void comprar5Cartas(Pilha *pilhaCompras, Pilha *pilhaDescarte, Mao *mao) {
    int i = 0;

    // Se houver cartas suficientes na pilha de compras
    if (tamPilha(pilhaCompras) >= 5) {
        for (i = 0; i < 5; i++) {
            mao->cartas[i] = desempilhar(pilhaCompras);
            (mao->quantidade)++;
        }
    } 
    else {
        // Compra todas as cartas restantes na pilha de compras
        int restantes = tamPilha(pilhaCompras);
        for (i = 0; i < restantes; i++) {
            mao->cartas[i] = desempilhar(pilhaCompras);
            (mao->quantidade)++;
        }

        // Move a pilha de descarte para compras e embaralha
        moverPilha(pilhaDescarte, pilhaCompras);

        // Termina de comprar até completar 5 cartas
        for (; i < 5; i++) {
            mao->cartas[i] = desempilhar(pilhaCompras);
            (mao->quantidade)++;
        }
    }
}

void descartarMao(Combate *combate) {
    for (int i = 0; i < combate->mao.quantidade; i++) {
        empilharCarta(&combate->pilhaDescarte, combate->mao.cartas[i]);
    }
    combate->mao.quantidade = 0;
}

void iniciarTurnoJogador(Combate *combate){
    combate->cartaSelecionada = 0;
    combate->estadoSelecao = SELECIONANDO_CARTA;
    combate->energia = 3;
	combate->mao.quantidade = 0; // zera a mão antes de comprar novas cartas
	comprar5Cartas(&(combate->pilhaCompras),&(combate->pilhaDescarte),&(combate->mao));
    combate->jogador.base.escudo=0;//Zera escudo entre turnos ---> Regra Do enunciado
}
int jogarCarta(Combate *combate, int indiceCarta, int alvo) {
    // Pega a carta escolhida da mão do jogador
    Carta cartaEscolhida = combate->mao.cartas[indiceCarta];
	
	// Verifica se a carta escolhida é do tipo ESPECIAL
	if (cartaEscolhida.tipo == ESPECIAL) {
		// Descarta todas as cartas da mão do jogador, movendo para a pilha de descarte
		descartarMao(combate);

		// Compra 5 novas cartas da pilha de compras
		comprar5Cartas(&(combate->pilhaCompras), &(combate->pilhaDescarte), &combate->mao);

		// Retorna 1 para indicar que a carta foi jogada com sucesso
		return 1;
}

    // Verifica se o jogador tem energia suficiente para jogar a carta
    if (combate->energia >= cartaEscolhida.custo) {

        // Aplica o efeito da carta: ataque ou defesa
        // Usa os campos base de Entidade tanto do jogador quanto do inimigo
        executarAcao(cartaEscolhida, &(combate->jogador).base, &(combate->inimigos[alvo]).base);
    
        // Primeiro verifica se o inimigo morreu
        int inimigoMorreu = (combate->inimigos[alvo].base.vida == 0);

        
        
        // Se o inimigo morrer, volta para seleção de carta  e ajusta a seleção do inimigo
            if (cartaEscolhida.tipo == ATAQUE && inimigoMorreu) {
                // Ajusta a seleção (muda 'alvo' se estiver morto)
                AjustarSelecaoInimigo(combate);
                combate->estadoSelecao = SELECIONANDO_CARTA;
            }
        

        // Deduz o custo da carta da energia do jogador
        combate->energia -= cartaEscolhida.custo;
        // Move a carta jogada para a pilha de descarte do jogador
        empilharCarta(&combate->pilhaDescarte, cartaEscolhida);

        // Remove a carta da mão e reorganiza as cartas restantes
        for (int i = indiceCarta; i < combate->mao.quantidade - 1; i++) {
            combate->mao.cartas[i] = combate->mao.cartas[i + 1];
        }

        // Atualiza a quantidade de cartas na mão após a remoção
        combate->mao.quantidade--;

        // Retorna 1 indicando que a carta foi jogada com sucesso
        return 1;
    } else {
        // Energia insuficiente, carta não pode ser jogada
        return 0;
    }
}
void encerrarTurnoJogador(Combate *combate) {
    // Mover todas as cartas da mão para a pilha de descarte
    descartarMao(combate);
	

    // Zerar a quantidade de cartas na mão
    combate->mao.quantidade = 0;
	
	// Troca para o turno dos inimigos
    combate->turno = TURNO_INIMIGOS;
}