#include "renderer.h"

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "combate.h"
#include "turno_inimigos.h"
#include "turno_jogador.h"

#include "constants.h"
#include "utils.h"

//
// ========================================================
//                  FUNÇÕES DE TEXTO
// ========================================================
//
void DrawScaledText(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x, float y,
                    float xscale, float yscale, int alignment,
                    const char* text) {
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, xscale, yscale);
    al_use_transform(&transform);

    al_draw_text(font, color, x, y, alignment, text);

    al_identity_transform(&transform);
    al_use_transform(&transform);
}

void DrawCenteredScaledText(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x,
                            float y, float xscale, float yscale, char* text) {
    DrawScaledText(font, color, x, y, xscale, yscale, ALLEGRO_ALIGN_CENTRE, text);
}

//
// ========================================================
//         CARREGAMENTO DE IMAGENS E INICIALIZAÇÃO
// ========================================================
//

void FillImagesProcedure(Renderer *renderer)
{
    renderer->img_bg = al_load_bitmap("assets/forest_bg.png");

    renderer->img_card_ataque   = al_load_bitmap("assets/carta_ataque.png");
    renderer->img_card_defesa   = al_load_bitmap("assets/carta_defesa.png");
    renderer->img_card_especial = al_load_bitmap("assets/carta_especial.png");

    renderer->img_enemy  = al_load_bitmap("assets/enemy.png");
    renderer->img_weak_enemy = al_load_bitmap("assets/weak_enemy.png");
    renderer->img_player = al_load_bitmap("assets/player.png");

    renderer->img_energy_ring = al_load_bitmap("assets/energy_ring.png");

    renderer->img_health_bar = al_load_bitmap("assets/health_bar_translucid.png");
    renderer->img_shield_bar = al_load_bitmap("assets/shield_bar_translucid.png");

    renderer->img_pile = al_load_bitmap("assets/pile.png");

    renderer->img_combat_screen  = al_load_bitmap("assets/combat_screen.png");
    renderer->img_game_over_screen = al_load_bitmap("assets/game_over_screen.png");
    renderer->img_victory_screen = al_load_bitmap("assets/victory_screen.png");

    must_init(renderer->img_bg != NULL, "forest background");

    must_init(renderer->img_card_ataque != NULL, "card ataque");
    must_init(renderer->img_card_defesa != NULL, "card defesa");
    must_init(renderer->img_card_especial != NULL, "card especial");

    must_init(renderer->img_enemy != NULL, "enemy sprite");
    must_init(renderer->img_weak_enemy != NULL, "weak enemy sprite");
    must_init(renderer->img_player != NULL, "player sprite");

    must_init(renderer->img_energy_ring != NULL, "energy ring");

    must_init(renderer->img_health_bar != NULL, "health bar");
    must_init(renderer->img_shield_bar != NULL, "shield bar");

    must_init(renderer->img_pile != NULL, "deck pile sprite");

    must_init(renderer->img_combat_screen != NULL, "combat screen");
    must_init(renderer->img_game_over_screen != NULL, "game over screen");
    must_init(renderer->img_victory_screen != NULL, "victory screen");
}
void FillRenderer(Renderer* renderer) {
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    renderer->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    must_init(renderer->display != NULL, "display");

    al_set_target_backbuffer(renderer->display);

    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    renderer->display_buffer =
        al_create_bitmap(DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT);
    must_init(renderer->display_buffer != NULL, "display buffer");

    renderer->font = al_create_builtin_font();
    must_init(renderer->font != NULL, "font");

    FillImagesProcedure(renderer);
}

// ========================================================
//      RENDERIZAÇÕES SIMPLES / TELAS ESTÁTICAS
// ========================================================
void RenderBackground(Renderer* render) {
    al_draw_scaled_bitmap(
        render->img_bg,
        0, 0,
        al_get_bitmap_width(render->img_bg),
        al_get_bitmap_height(render->img_bg),
        0, 0,
        DISPLAY_BUFFER_WIDTH,
        DISPLAY_BUFFER_HEIGHT,
        0
    );
}

void RenderGameOverScreen(Renderer* render)
{
    al_set_target_backbuffer(render->display);

    al_draw_scaled_bitmap(
        render->img_game_over_screen,
        0, 0,
        al_get_bitmap_width(render->img_game_over_screen),
        al_get_bitmap_height(render->img_game_over_screen),
        0, 0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
        0
    );

    al_flip_display();
}

void RenderVictoryScreen(Renderer* render, int totalVitorias)
{
    al_set_target_backbuffer(render->display);

    al_draw_scaled_bitmap(
        render->img_victory_screen,
        0, 0,
        al_get_bitmap_width(render->img_victory_screen),
        al_get_bitmap_height(render->img_victory_screen),
        0, 0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
        0
    );

    al_flip_display();
}

void RenderCombatVictory(Renderer* render)
{
    al_set_target_backbuffer(render->display);

    al_draw_scaled_bitmap(
        render->img_combat_screen,
        0, 0,
        al_get_bitmap_width(render->img_combat_screen),
        al_get_bitmap_height(render->img_combat_screen),
        0, 0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
        0
    );

    al_flip_display();
}


// ========================================================
//        ELEMENTOS PEQUENOS: DECK, BARRAS, ENERGIA
// ========================================================
void RenderDeck(Renderer* renderizador, int posX, int posY, int quantidadeCartas)
{
    ALLEGRO_BITMAP* bufferAnterior = al_get_target_bitmap();
    al_set_target_bitmap(renderizador->display_buffer);

    ALLEGRO_BITMAP* imagemBaralho = renderizador->img_pile;

    int larguraOriginal = al_get_bitmap_width(imagemBaralho);
    int alturaOriginal  = al_get_bitmap_height(imagemBaralho);

    float fatorEscala = 0.35;

    int larguraFinal = larguraOriginal * fatorEscala;
    int alturaFinal  = alturaOriginal  * fatorEscala;

    al_draw_scaled_bitmap(
        imagemBaralho,
        0, 0, larguraOriginal, alturaOriginal,
        posX, posY, larguraFinal, alturaFinal,
        0
    );

    float ajusteX = 0.43;
    float ajusteY = 0.40;

    float posTextoX = posX + larguraFinal * ajusteX;
    float posTextoY = posY + alturaFinal  * ajusteY;

    char textoQuantidade[16];
    snprintf(textoQuantidade, sizeof(textoQuantidade), "%d", quantidadeCartas);

    float escalaTexto = 2.0;

    DrawCenteredScaledText(
        renderizador->font,
        al_map_rgb(255, 255, 255),
        posTextoX / escalaTexto, posTextoY / escalaTexto,
        escalaTexto, escalaTexto,
        textoQuantidade
    );

    al_set_target_bitmap(bufferAnterior);
}

void RenderHealthBar(float barraInicioX, float barraFimX, float posicaoY,
                     int vidaAtual, int vidaMaxima, ALLEGRO_FONT* fonteTexto) {

    float proporcaoVida = (float)vidaAtual / (float)vidaMaxima;
    float barraVidaX = barraInicioX + (barraFimX - barraInicioX) * proporcaoVida;

    al_draw_filled_rounded_rectangle(
        barraInicioX,
        posicaoY - HEALTH_BAR_HEIGHT,
        barraVidaX,
        posicaoY,
        HEALTH_BAR_RX, HEALTH_BAR_RY,
        al_map_rgb(0, 255, 0)
    );

    char textoVida[20];
    snprintf(textoVida, sizeof(textoVida), "%d / %d", vidaAtual, vidaMaxima);

    float centroX = (barraInicioX + barraFimX) / 2.0;
    float centroY = posicaoY - HEALTH_BAR_HEIGHT / 2.0;

    DrawScaledText(fonteTexto, al_map_rgb(0,0,0),
                   centroX, centroY,
                   1.0, 1.0,
                   ALLEGRO_ALIGN_CENTRE,
                   textoVida);
}

void RenderShieldBar(float barraInicioX, float barraFimX, float posicaoY,
                     int valorEscudo, int escudoMaximo, ALLEGRO_FONT* fonteTexto)
{
    float larguraEscudo = (barraFimX - barraInicioX) *
                          ((float)valorEscudo / escudoMaximo);

    al_draw_filled_rounded_rectangle(
        barraInicioX,
        posicaoY - HEALTH_BAR_HEIGHT,
        barraInicioX + larguraEscudo,
        posicaoY,
        HEALTH_BAR_RX, HEALTH_BAR_RY,
        al_map_rgb(0, 128, 255)
    );

    char textoEscudo[16];
    snprintf(textoEscudo, sizeof(textoEscudo), "%d", valorEscudo);

    float centroY = posicaoY - HEALTH_BAR_HEIGHT / 2.0f;

    DrawScaledText(fonteTexto, al_map_rgb(255,255,255),
                   (barraInicioX + barraFimX) / 2.0f, centroY,
                   1.0f, 1.0f, ALLEGRO_ALIGN_CENTRE,
                   textoEscudo);
}

void RenderEnergy(Renderer* renderizador, int energiaAtual, int energiaMaxima)
{
    al_set_target_bitmap(renderizador->display_buffer);

    ALLEGRO_BITMAP* imagemEnergia = renderizador->img_energy_ring;

    float fatorEscala = 0.20;

    int larguraOriginal = al_get_bitmap_width(imagemEnergia);
    int alturaOriginal  = al_get_bitmap_height(imagemEnergia);

    int larguraFinal = larguraOriginal * fatorEscala;
    int alturaFinal  = alturaOriginal  * fatorEscala;

    al_draw_scaled_bitmap(
        imagemEnergia,
        0, 0, larguraOriginal, alturaOriginal,
        ENERGY_X - larguraFinal/2,
        ENERGY_Y - alturaFinal/2,
        larguraFinal, alturaFinal,
        0
    );

    char textoEnergia[16];
    snprintf(textoEnergia, sizeof(textoEnergia), "%d/%d",
             energiaAtual, energiaMaxima);

    float escalaTexto = 2.8f;

    DrawCenteredScaledText(
        renderizador->font,
        al_map_rgb(255,255,255),
        ENERGY_X / escalaTexto,
        (ENERGY_Y - al_get_font_line_height(renderizador->font)/2) / escalaTexto,
        escalaTexto, escalaTexto,
        textoEnergia
    );
}
//
// ========================================================
//                   RENDERIZAR JOGADOR
// ========================================================
//
void RenderPlayer(Renderer* renderer, Personagem* jogador)
{
    ALLEGRO_BITMAP* imagemJogador = renderer->img_player;

    int larguraOriginal = al_get_bitmap_width(imagemJogador);
    int alturaOriginal  = al_get_bitmap_height(imagemJogador);

    float escalaJogador = 0.25f;

    int larguraNova = larguraOriginal * escalaJogador;
    int alturaNova  = alturaOriginal  * escalaJogador;

    float posX = PLAYER_BEGIN_X - larguraNova / 2.0f;
    float posY = PLAYER_BEGIN_Y - alturaNova  / 2.0f;

    al_draw_scaled_bitmap(
        imagemJogador,
        0, 0, larguraOriginal, alturaOriginal,
        posX, posY,
        larguraNova, alturaNova,
        0
    );

    RenderHealthBar(
        posX,
        posX + larguraNova,
        posY + alturaNova + 10,
        jogador->base.vida,
        100,
        renderer->font
    );

    RenderShieldBar(
        posX,
        posX + larguraNova,
        posY + alturaNova + 35,
        jogador->base.escudo,
        100,
        renderer->font
    );
}

//
// ========================================================
//          RENDERIZAÇÃO DAS CARTAS / MÃO
// ========================================================
//
void RenderCard(const Renderer* render, Carta* carta, int posX, int posY, int estaSelecionada)
{
    ALLEGRO_BITMAP* alvoAnterior = al_get_target_bitmap();
    al_set_target_bitmap(render->display_buffer);

    ALLEGRO_BITMAP* imgCarta = NULL;

    if (carta->tipo == ATAQUE)
        imgCarta = render->img_card_ataque;
    else if (carta->tipo == DEFESA)
        imgCarta = render->img_card_defesa;
    else
        imgCarta = render->img_card_especial;

    float escala = 0.30f;

    int larguraOriginal = al_get_bitmap_width(imgCarta);
    int alturaOriginal  = al_get_bitmap_height(imgCarta);

    int larguraFinal = larguraOriginal * escala;
    int alturaFinal  = alturaOriginal  * escala;

    al_draw_scaled_bitmap(
        imgCarta,
        0, 0, larguraOriginal, alturaOriginal,
        posX, posY,
        larguraFinal, alturaFinal,
        0
    );

    char textoCusto[32];
    snprintf(textoCusto, sizeof(textoCusto), "Custo: %d", carta->custo);

    float escalaTexto = 1.7f;

    DrawCenteredScaledText(
        render->font,
        al_map_rgb(255,255,0),
        (posX + larguraFinal/2.0f) / escalaTexto,
        (posY + 30) / escalaTexto,
        escalaTexto,
        escalaTexto,
        textoCusto
    );

    char textoEfeito[32];
    snprintf(textoEfeito, sizeof(textoEfeito), "Efeito: %d", carta->efeito);

    DrawCenteredScaledText(
        render->font,
        al_map_rgb(255,255,255),
        (posX + larguraFinal/2.0f) / escalaTexto,
        (posY + alturaFinal - 30) / escalaTexto,
        escalaTexto,
        escalaTexto,
        textoEfeito
    );

    if (estaSelecionada)
    {
        al_draw_rectangle(
            posX + 3, posY + 3,
            posX + larguraFinal - 3,
            posY + alturaFinal - 3,
            al_map_rgb(255,255,0),
            4
        );
    }

    al_set_target_bitmap(alvoAnterior);
}


void RenderPlayerHand(Renderer* render, Mao *mao, Combate *combate)
{
    if (mao->quantidade == 0) return;

    ALLEGRO_BITMAP* imgTemp = render->img_card_ataque;

    int larguraOriginal = al_get_bitmap_width(imgTemp);
    float escala = 0.30f;
    int larguraFinal = larguraOriginal * escala;

    float larguraTotal = mao->quantidade * larguraFinal +
                         (mao->quantidade - 1) * CARDS_SPACING;

    float posicaoInicialX = (DISPLAY_WIDTH - larguraTotal) / 2.0f;

    for (int i = 0; i < mao->quantidade; i++)
    {
        float posX = posicaoInicialX + i * (larguraFinal + CARDS_SPACING);
        int estaSelecionada = (i == combate->cartaSelecionada);

        RenderCard(render, &mao->cartas[i], posX, HAND_BEGIN_Y, estaSelecionada);
    }
}


//
// ========================================================
//                  RENDERIZAÇÃO DOS INIMIGOS
// ========================================================
//
void RenderEnemiesIntents(Renderer* render, Combate* combate)
{
    int posX = ENEMY_BEGIN_X;

    for (int i = 0; i < NUMINIMIGOS; i++)
    {
        Inimigo* inimigo = &combate->inimigos[i];

        if (inimigo->base.vida <= 0)
            continue;

        ALLEGRO_BITMAP* imgInimigo = NULL;
        float escala = 1.0f;

        if (inimigo->forca == 1)
        {
            imgInimigo = render->img_enemy;
            escala = 0.30f;
        }
        else
        {
            imgInimigo = render->img_weak_enemy;
            escala = 0.25f;
        }

        int larguraOriginal = al_get_bitmap_width(imgInimigo);
        int alturaOriginal  = al_get_bitmap_height(imgInimigo);

        int larguraFinal = larguraOriginal * escala;
        int alturaFinal  = alturaOriginal  * escala;

        int posY = ENEMY_BEGIN_Y - alturaFinal;

        Carta acaoAtual = inimigo->acoes[inimigo->acaoAtual];

        char textoIntencao[64];

        if (acaoAtual.tipo == ATAQUE)
            snprintf(textoIntencao, sizeof(textoIntencao), "Ataque (%d)", acaoAtual.efeito);
        else if (acaoAtual.tipo == DEFESA)
            snprintf(textoIntencao, sizeof(textoIntencao), "Defesa (%d)", acaoAtual.efeito);
        else
            snprintf(textoIntencao, sizeof(textoIntencao), "Esp (%d)", acaoAtual.efeito);

        float textoX = posX + larguraFinal / 2.0f;
        float textoY = posY - 20;

        float escalaTexto = 1.6f;

        DrawScaledText(
            render->font,
            al_map_rgb(255,255,255),
            textoX / escalaTexto,
            textoY / escalaTexto,
            escalaTexto, escalaTexto,
            ALLEGRO_ALIGN_CENTRE,
            textoIntencao
        );

        posX += larguraFinal + ENEMY_SPACING;
    }
}


void RenderEnemies(Renderer* render, Combate* combate)
{
    int posX = ENEMY_BEGIN_X;

    for (int i = 0; i < NUMINIMIGOS; i++)
    {
        Inimigo* inimigo = &combate->inimigos[i];

        if (inimigo->base.vida <= 0)
            continue;

        ALLEGRO_BITMAP* imgInimigo = NULL;
        float escala = 1.0f;

        if (inimigo->forca == 1)
        {
            imgInimigo = render->img_enemy;
            escala = 0.30f;
        }
        else
        {
            imgInimigo = render->img_weak_enemy;
            escala = 0.25f;
        }

        int larguraOriginal = al_get_bitmap_width(imgInimigo);
        int alturaOriginal  = al_get_bitmap_height(imgInimigo);

        int larguraFinal = larguraOriginal * escala;
        int alturaFinal  = alturaOriginal  * escala;

        int posY = ENEMY_BEGIN_Y - alturaFinal;

        al_draw_scaled_bitmap(
            imgInimigo,
            0, 0, larguraOriginal, alturaOriginal,
            posX, posY,
            larguraFinal, alturaFinal,
            0
        );

        if (combate->alvoSelecionado == i &&
            combate->estadoSelecao == SELECIONANDO_INIMIGO)
        {
            al_draw_rectangle(
                posX - 4, posY - 4,
                posX + larguraFinal + 4, posY + alturaFinal + 4,
                al_map_rgb(255, 255, 0),
                4
            );
        }

        RenderHealthBar(
            posX,
            posX + larguraFinal,
            posY + alturaFinal + 10,
            inimigo->base.vida,
            inimigo->vidaMax,
            render->font
        );

        RenderShieldBar(
            posX,
            posX + larguraFinal,
            posY + alturaFinal + 35,
            inimigo->base.escudo,
            inimigo->vidaMax,
            render->font
        );

        posX += larguraFinal + ENEMY_SPACING;
    }

    RenderEnemiesIntents(render, combate);
}



//
// ========================================================
//                  FUNÇÃO PRINCIPAL DE RENDER
// ========================================================
//
void Render(Renderer* renderer, Combate *combate) {
    al_set_target_bitmap(renderer->display_buffer);
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    RenderBackground(renderer);

    RenderDeck(renderer, 100,790,combate->pilhaCompras.topo + 1);

    RenderDeck(renderer,DISPLAY_BUFFER_WIDTH - 250,790,combate->pilhaDescarte.topo + 1);

    RenderPlayer(renderer, &combate->jogador);
    RenderEnergy(renderer,combate->energia, 3);
    RenderEnemies(renderer, combate);
    RenderPlayerHand(renderer,&combate->mao,combate);

    al_set_target_backbuffer(renderer->display);

    al_draw_scaled_bitmap(renderer->display_buffer, 0, 0, DISPLAY_BUFFER_WIDTH,
                          DISPLAY_BUFFER_HEIGHT, 0, 0, DISPLAY_WIDTH,
                          DISPLAY_HEIGHT, 0);

    al_flip_display();
}

//
// ========================================================
//                        LIMPEZA
// ========================================================
//
void ClearRenderer(Renderer* renderer) {
    al_destroy_display(renderer->display);
    al_destroy_bitmap(renderer->display_buffer);
    al_destroy_font(renderer->font);
}
