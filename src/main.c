#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "renderer.h"
#include "utils.h"
#include "cartas.h"
#include "combate.h"
#include "turno_inimigos.h"
#include "turno_jogador.h"

// Main do jogo - controle completo via teclado e turnos

int main() {

    // --------------------------------------------------------------------
    // Setup do Allegro
    // --------------------------------------------------------------------
    srand(time(NULL));
    must_init(al_init(), "allegro");
    must_init(al_init_image_addon(), "allegro");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    unsigned char keyboard_keys[ALLEGRO_KEY_MAX];
    ClearKeyboardKeys(keyboard_keys);

    ALLEGRO_EVENT event;

    Renderer renderer;
    FillRenderer(&renderer);
    al_register_event_source(queue,
                             al_get_display_event_source(renderer.display));

    // --------------------------------------------------------------------
    // Componentes do Jogo
    // --------------------------------------------------------------------

    int vitorias = 0;
    const int VITORIAS_PARA_VENCER = 10;

    // Jogador inicial (estado persistente entre combates)
    Personagem jogador_inicial;
    jogador_inicial.base.vida = 100;
    jogador_inicial.base.escudo = 0; // escudo não persiste entre combates

    // Baralho inicial (aleatório)
    Carta baralho_inicial[MAX_CARTAS];
    criarBaralhoInicial(baralho_inicial, MAX_CARTAS);

    // Combate atual
    Combate combate = {0};

    // --------------------------------------------------------------------
    // Inicia o primeiro combate
    // --------------------------------------------------------------------
    combate.jogador = jogador_inicial; // combate.jogador é estado temporário de um combate
    iniciarCombate(&combate, baralho_inicial, MAX_CARTAS);
    iniciarTurnoJogador(&combate); // reseta energia, escudo, cartas na mão e seleção

    // --------------------------------------------------------------------
    // Loop Principal
    // --------------------------------------------------------------------
    al_start_timer(timer);

    while (1) {

        al_wait_for_event(queue, &event);

        // Flags de controle
        int done = 0, redraw = 0;

        
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                redraw = 1;
                if (keyboard_keys[ALLEGRO_KEY_Q]) {
                    done = 1; 
                    break;
                }
                
                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    keyboard_keys[i] &= ~GAME_KEY_SEEN;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (!(keyboard_keys[event.keyboard.keycode] & GAME_KEY_DOWN)) {
                    keyboard_keys[event.keyboard.keycode] |= GAME_KEY_DOWN;
                    keyboard_keys[event.keyboard.keycode] |= GAME_KEY_SEEN; 
                }
                break;

            case ALLEGRO_EVENT_KEY_UP:
                keyboard_keys[event.keyboard.keycode] &= ~GAME_KEY_DOWN;
                keyboard_keys[event.keyboard.keycode] &= ~GAME_KEY_SEEN; 
                break;


            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = 1;
                break;
        }

        if (done) break;

        // =====================================================
        // CONTROLES DO JOGADOR (CLIQUE ÚNICO)
        // =====================================================

        // SPACE: matar todos os inimigos
        if (keyboard_keys[ALLEGRO_KEY_SPACE] & GAME_KEY_SEEN) {
            keyboard_keys[ALLEGRO_KEY_SPACE] &= ~GAME_KEY_SEEN;

            for (int i = 0; i < NUMINIMIGOS; i++)
                combate.inimigos[i].base.vida = 0;
        }

        // X: setar vida do jogador para 1
        if (keyboard_keys[ALLEGRO_KEY_X] & GAME_KEY_SEEN) {
            keyboard_keys[ALLEGRO_KEY_X] &= ~GAME_KEY_SEEN;
            combate.jogador.base.vida = 1;
        }

        // ESC: encerrar turno
        if (keyboard_keys[ALLEGRO_KEY_ESCAPE] & GAME_KEY_SEEN) {
            keyboard_keys[ALLEGRO_KEY_ESCAPE] &= ~GAME_KEY_SEEN;

            encerrarTurnoJogador(&combate);
            turnoInimigos(&combate);
            iniciarTurnoJogador(&combate);
        }

        // =====================================================
        // NAVEGAÇÃO COM SETAS (CLIQUES ÚNICOS)
        // =====================================================

        // ESQUERDA
        if (keyboard_keys[ALLEGRO_KEY_LEFT] & GAME_KEY_SEEN) {
            keyboard_keys[ALLEGRO_KEY_LEFT] &= ~GAME_KEY_SEEN;

            if (combate.estadoSelecao == SELECIONANDO_CARTA && combate.cartaSelecionada > 0)
                combate.cartaSelecionada--;

            else if (combate.estadoSelecao == SELECIONANDO_INIMIGO) {
                //Lógica pra não selecionar inimigos mortos
                int novo = combate.alvoSelecionado - 1;

                while (novo >= 0 && combate.inimigos[novo].base.vida <= 0)
                    novo--;

                if (novo >= 0) //Se não tiver nenhum nuvo será = -1 e o alvo nao sera alterado
                    combate.alvoSelecionado = novo;
        }

        }

        // DIREITA
        if (keyboard_keys[ALLEGRO_KEY_RIGHT] & GAME_KEY_SEEN) {
            keyboard_keys[ALLEGRO_KEY_RIGHT] &= ~GAME_KEY_SEEN;

            if (combate.estadoSelecao == SELECIONANDO_CARTA && combate.cartaSelecionada < combate.mao.quantidade - 1)
                combate.cartaSelecionada++;

            else if (combate.estadoSelecao == SELECIONANDO_INIMIGO) {
                //Lógica pra não selecionar inimigos mortos
                int novo = combate.alvoSelecionado + 1;

                // pular mortos
                while (novo < NUMINIMIGOS && combate.inimigos[novo].base.vida <= 0)
                    novo++;

                // só muda se encontrou vivo
                if (novo < NUMINIMIGOS)//Se não tiver nenhum novo será = NUMINIMIGOS e o alvo nao sera alterado
                    combate.alvoSelecionado = novo;
            }

        }

        // =====================================================
        // ENTER – confirmar seleção (apenas 1 vez por clique)
        // =====================================================
        if (keyboard_keys[ALLEGRO_KEY_ENTER] & GAME_KEY_SEEN) {
            keyboard_keys[ALLEGRO_KEY_ENTER] &= ~GAME_KEY_SEEN;

            if (combate.mao.quantidade > 0) {

                Carta cartaAtual = combate.mao.cartas[combate.cartaSelecionada];

                // Primeiro estado: selecionando carta
                if (combate.estadoSelecao == SELECIONANDO_CARTA) {

                    if (cartaAtual.tipo == ATAQUE) {

                        // Checa a quantidade de energia restante
                        if (combate.energia < cartaAtual.custo) {
                            //ignora o ENTER
                            continue;
                        }
                        
                        // Precisa escolher inimigo 
                        combate.estadoSelecao = SELECIONANDO_INIMIGO;
                        
                        // Ajusta imediatamente para um inimigo vivo antes de se iinicar a seleção deles
                        AjustarSelecaoInimigo(&combate);
                         
                    }
                    else {
                        // DEFESA ou ESPECIAL -> joga direto
                        jogarCarta(&combate, combate.cartaSelecionada, 0);
                        combate.estadoSelecao = SELECIONANDO_CARTA;
                        // Garante que o índice da carta selecionada ainda seja válido após a remoção da carta(pelo JogarCarta)
                        if (combate.cartaSelecionada >= combate.mao.quantidade)
                            combate.cartaSelecionada = combate.mao.quantidade - 1;
                        
                    }
                }
                else {// Segundo estado: selecionando inimigo
                    //Garante pela segunda vez que inimigos mortos não serao atacados
                    if (combate.inimigos[combate.alvoSelecionado].base.vida <= 0) {
                        // volta para escolher carta
                        combate.estadoSelecao = SELECIONANDO_CARTA;
                        combate.alvoSelecionado = 0;
                        continue; // sai do ENTER e volta a esperar tecla
                    }
                    jogarCarta(&combate, combate.cartaSelecionada, combate.alvoSelecionado);
                    combate.estadoSelecao = SELECIONANDO_CARTA;
                    combate.alvoSelecionado = 0;

                    // Garante que o índice da carta selecionada ainda seja válido após a remoção da carta(pelo JogarCarta)
                    if (combate.mao.quantidade == 0)
                        combate.cartaSelecionada = 0;
                    else if (combate.cartaSelecionada >= combate.mao.quantidade)
                        combate.cartaSelecionada = combate.mao.quantidade - 1;
                }
            }
        }

        // =====================================================
        // FIM DO COMBATE
        // =====================================================
        int status = combateTerminado(&combate); // -1 perde, 1 vence, 0 continua
        if (status != 0) {

            // ------------------ DERROTA ------------------
            if (status == -1) {
                vitorias = 0;
                // Mostra tela de Game Over
                RenderGameOverScreen(&renderer);

                // Espera qualquer tecla
                ALLEGRO_EVENT e;
                int esperando = 1;
                while (esperando) {
                    al_wait_for_event(queue, &e);
                    if (e.type == ALLEGRO_EVENT_KEY_DOWN ||
                        e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    {
                        esperando = 0;
                    }
                }

                // Restaura jogador ao estado inicial
                jogador_inicial.base.vida = 100;
                jogador_inicial.base.escudo = 0;

                // Recria baralho inicial aleatório
                criarBaralhoInicial(baralho_inicial, MAX_CARTAS);

                // Reinicia combate com jogador restaurado
                combate.jogador = jogador_inicial;
                iniciarCombate(&combate, baralho_inicial, MAX_CARTAS);
                iniciarTurnoJogador(&combate);
            }

        // ------------------ VITÓRIA ------------------
        else if (status == 1) { 
            vitorias++;
            // -------------- VITÓRIA FINAL --------------
            if (vitorias >= VITORIAS_PARA_VENCER) {

                RenderVictoryScreen(&renderer, vitorias);

                ALLEGRO_EVENT e;
                int esperando = 1;

                while (esperando) {
                    al_wait_for_event(queue, &e);
                    if (e.type == ALLEGRO_EVENT_KEY_DOWN ||
                        e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    {
                        esperando = 0;
                    }
                }

                done = 1;
                break;
            }

            // -------------- VITÓRIA NORMAL --------------
            RenderCombatVictory(&renderer);

            ALLEGRO_EVENT e;
            int esperando = 1;

            while (esperando) {
                al_wait_for_event(queue, &e);
                if (e.type == ALLEGRO_EVENT_KEY_DOWN ||
                    e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    esperando = 0;
                }
            }

            // Atualiza vida persistente
            jogador_inicial.base.vida = combate.jogador.base.vida;
            jogador_inicial.base.escudo = 0;
            // Inicia próximo combate
            combate.jogador = jogador_inicial;
            iniciarCombate(&combate, baralho_inicial, MAX_CARTAS);
            iniciarTurnoJogador(&combate);
        }
        }

        // =====================================================
        // RENDER
        // =====================================================
        if (redraw) {
            Render(&renderer,&combate);
            redraw = 0;
        }
    }
    
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    ClearRenderer(&renderer);

    return 0;
}
