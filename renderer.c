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
//         CARREGAMENTO DE ASSETS E INICIALIZAÇÃO
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

//
// ========================================================
//      RENDERIZAÇÕES SIMPLES / TELAS ESTÁTICAS
// ========================================================
//
void RenderBackground(Renderer* renderer) {
    al_draw_scaled_bitmap(
        renderer->img_bg,
        0, 0,
        al_get_bitmap_width(renderer->img_bg),
        al_get_bitmap_height(renderer->img_bg),
        0, 0,
        DISPLAY_BUFFER_WIDTH,
        DISPLAY_BUFFER_HEIGHT,
        0
    );
}

void RenderGameOverScreen(Renderer* renderer)
{
    al_set_target_backbuffer(renderer->display);

    al_draw_scaled_bitmap(
        renderer->img_game_over_screen,
        0, 0,
        al_get_bitmap_width(renderer->img_game_over_screen),
        al_get_bitmap_height(renderer->img_game_over_screen),
        0, 0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
        0
    );

    al_flip_display();
}

void RenderVictoryScreen(Renderer* renderer, int vitorias)
{
    al_set_target_backbuffer(renderer->display);

    al_draw_scaled_bitmap(
        renderer->img_victory_screen,
        0, 0,
        al_get_bitmap_width(renderer->img_victory_screen),
        al_get_bitmap_height(renderer->img_victory_screen),
        0, 0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
        0
    );

    al_flip_display();
}

void RenderCombatVictory(Renderer* renderer)
{
    al_set_target_backbuffer(renderer->display);

    al_draw_scaled_bitmap(
        renderer->img_combat_screen,
        0, 0,
        al_get_bitmap_width(renderer->img_combat_screen),
        al_get_bitmap_height(renderer->img_combat_screen),
        0, 0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
        0
    );

    al_flip_display();
}

//
// ========================================================
//        ELEMENTOS PEQUENOS: DECK, BARRAS, ENERGIA
// ========================================================
//
void RenderDeck(Renderer* renderer, int x_left, int y_top, int quantidade)
{
    ALLEGRO_BITMAP* prev = al_get_target_bitmap();
    al_set_target_bitmap(renderer->display_buffer);

    ALLEGRO_BITMAP* img = renderer->img_pile;

    int original_w = al_get_bitmap_width(img);
    int original_h = al_get_bitmap_height(img);

    float escala = 0.35;

    int w = original_w * escala;
    int h = original_h * escala;

    al_draw_scaled_bitmap(
        img,
        0, 0, original_w, original_h,
        x_left, y_top, w, h,
        0
    );

    float ajusteX = 0.43f;
    float ajusteY = 0.40f;

    float posTextoX = x_left + w * ajusteX;
    float posTextoY = y_top  + h * ajusteY;

    char texto[16];
    snprintf(texto, sizeof(texto), "%d", quantidade);

    float scale = 2.0;

    DrawCenteredScaledText(
        renderer->font,
        al_map_rgb(255, 255, 255),
        posTextoX/scale, posTextoY/scale,
        scale,scale,
        texto
    );

    al_set_target_bitmap(prev);
}

void RenderHealthBar(float x_begin, float x_end, float y_down_left,
                     int vidaAtual, int vidaMax, ALLEGRO_FONT* font) {
    float vidaProp = (float)vidaAtual / (float)vidaMax;
    float x_bar_fim = x_begin + (x_end - x_begin) * vidaProp;

    al_draw_filled_rounded_rectangle(
        x_begin,
        y_down_left - HEALTH_BAR_HEIGHT,
        x_bar_fim,
        y_down_left,
        HEALTH_BAR_RX, HEALTH_BAR_RY, al_map_rgb(0, 255, 0));

    char text[20];
    snprintf(text, sizeof(text), "%d / %d", vidaAtual, vidaMax);
    float mid_x = (x_begin + x_end) / 2.0f;
    float mid_y = y_down_left - HEALTH_BAR_HEIGHT / 2.0f;

    DrawScaledText(font, al_map_rgb(0,0,0), mid_x, mid_y, 1.0f, 1.0f, ALLEGRO_ALIGN_CENTRE, text);
}

void RenderShieldBar(float x_begin, float x_end, float y_bottom, int shieldValue, int shieldMax, ALLEGRO_FONT* font) {

    float shieldWidth = (x_end - x_begin) * ((float)shieldValue / shieldMax);
    al_draw_filled_rounded_rectangle(
        x_begin,
        y_bottom - HEALTH_BAR_HEIGHT,
        x_begin + shieldWidth,
        y_bottom ,
        HEALTH_BAR_RX, HEALTH_BAR_RY, al_map_rgb(0, 128, 255)
    );

    char text[16];
    snprintf(text, sizeof(text), "%d", shieldValue);
    float mid_y = y_bottom - HEALTH_BAR_HEIGHT / 2.0;

    DrawScaledText(font, al_map_rgb(255, 255, 255),
                   (x_begin + x_end) / 2.0f, mid_y,
                   1.0f, 1.0f, ALLEGRO_ALIGN_CENTRE, text);
}

void RenderEnergy(Renderer* renderer, int energiaAtual, int energiaMax)
{
    al_set_target_bitmap(renderer->display_buffer);

    ALLEGRO_BITMAP* imagem = renderer->img_energy_ring;

    float escala = 0.20;

    int larguraOriginal = al_get_bitmap_width(imagem);
    int alturaOriginal  = al_get_bitmap_height(imagem);

    int larguraNova = larguraOriginal * escala;
    int alturaNova  = alturaOriginal  * escala;

    al_draw_scaled_bitmap(
        imagem,
        0, 0, larguraOriginal, alturaOriginal,
        ENERGY_X - larguraNova/2,
        ENERGY_Y - alturaNova/2,
        larguraNova, alturaNova,
        0
    );

    char texto[16];
    snprintf(texto, sizeof(texto), "%d/%d", energiaAtual, energiaMax);

    float scale =2.8;
    DrawCenteredScaledText(
    renderer->font,
    al_map_rgb(255,255,255),
    ENERGY_X/scale,
    (ENERGY_Y - al_get_font_line_height(renderer->font)/2) /scale,
    scale,
    scale,
    texto
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
void RenderCard(const Renderer* renderer, Carta* carta, int x_left, int y_top, int selecionada)
{
    ALLEGRO_BITMAP* anterior = al_get_target_bitmap();
    al_set_target_bitmap(renderer->display_buffer);

    ALLEGRO_BITMAP* imagem;

    if (carta->tipo == ATAQUE)
        imagem = renderer->img_card_ataque;
    else if (carta->tipo == DEFESA)
        imagem = renderer->img_card_defesa;
    else
        imagem = renderer->img_card_especial;

    float escala = 0.30f;

    int wOriginal = al_get_bitmap_width(imagem);
    int hOriginal = al_get_bitmap_height(imagem);

    int wNovo = wOriginal * escala;
    int hNovo = hOriginal * escala;

    al_draw_scaled_bitmap(
        imagem,
        0, 0,
        wOriginal, hOriginal,
        x_left,
        y_top,
        wNovo,
        hNovo,
        0
    );

    char textoCusto[32];
    snprintf(textoCusto, sizeof(textoCusto), "Custo: %d", carta->custo);

    float escalaTexto = 1.7f;

    DrawCenteredScaledText(
        renderer->font,
        al_map_rgb(255,255,0),
        (x_left + wNovo/2.0f) / escalaTexto,
        (y_top + 30) / escalaTexto,
        escalaTexto,
        escalaTexto,
        textoCusto
    );

    char textoEfeito[32];
    snprintf(textoEfeito, sizeof(textoEfeito), "Efeito: %d", carta->efeito);

    DrawCenteredScaledText(
        renderer->font,
        al_map_rgb(255,255,255),
        (x_left + wNovo/2.0f) / escalaTexto,
        (y_top + hNovo - 30) / escalaTexto,
        escalaTexto,
        escalaTexto,
        textoEfeito
    );

    if (selecionada)
    {
        al_draw_rectangle(
            x_left + 3, y_top + 3,
            x_left + wNovo - 3,
            y_top + hNovo - 3,
            al_map_rgb(255,255,0),
            4
        );
    }

    al_set_target_bitmap(anterior);
}

void RenderPlayerHand(Renderer* renderer, Mao *mao, Combate *combate)
{
    if (mao->quantidade == 0) return;

    ALLEGRO_BITMAP* img = renderer->img_card_ataque;

    int wOriginal = al_get_bitmap_width(img);
    float escala = 0.30f;
    int wNovo = wOriginal * escala;

    float totalWidth = mao->quantidade * wNovo +
                       (mao->quantidade - 1) * CARDS_SPACING;

    float startX = (DISPLAY_WIDTH - totalWidth) / 2.0f;

    for (int i = 0; i < mao->quantidade; i++)
    {
        float x = startX + i * (wNovo + CARDS_SPACING);
        int selecionada = (i == combate->cartaSelecionada);

        RenderCard(renderer, &mao->cartas[i], x, HAND_BEGIN_Y, selecionada);
    }
}

//
// ========================================================
//                  RENDERIZAÇÃO DOS INIMIGOS
// ========================================================
//
void RenderEnemiesIntents(Renderer* renderer, Combate* combate)
{
    int posXAtual = ENEMY_BEGIN_X;

    for (int i = 0; i < NUMINIMIGOS; i++)
    {
        Inimigo* inimigo = &combate->inimigos[i];

        if (inimigo->base.vida <= 0)
            continue;

        ALLEGRO_BITMAP* img;
        float escala;

        if (inimigo->forca == 1)
        {
            img = renderer->img_enemy;
            escala = 0.30f;
        }
        else
        {
            img = renderer->img_weak_enemy;
            escala = 0.25f;
        }

        int wOriginal = al_get_bitmap_width(img);
        int hOriginal = al_get_bitmap_height(img);

        int w = wOriginal * escala;
        int h = hOriginal * escala;

        int posY = ENEMY_BEGIN_Y - h;

        Carta acao = inimigo->acoes[inimigo->acaoAtual];

        char texto[64];

        if (acao.tipo == ATAQUE)
            snprintf(texto, sizeof(texto), "Ataque (%d)", acao.efeito);
        else if (acao.tipo == DEFESA)
            snprintf(texto, sizeof(texto), "Defesa (%d)", acao.efeito);
        else
            snprintf(texto, sizeof(texto), "Esp (%d)", acao.efeito);

        float textoX = posXAtual + w / 2.0f;
        float textoY = posY - 20;

        float esc = 1.6f;

        DrawScaledText(
            renderer->font,
            al_map_rgb(255,255,255),
            textoX / esc,
            textoY / esc,
            esc, esc,
            ALLEGRO_ALIGN_CENTRE,
            texto
        );

        posXAtual += w + ENEMY_SPACING;
    }
}

void RenderEnemies(Renderer* renderer, Combate* combate)
{
    int posXAtual = ENEMY_BEGIN_X;

    for (int i = 0; i < NUMINIMIGOS; i++)
    {
        Inimigo* inimigo = &combate->inimigos[i];

        if (inimigo->base.vida <= 0)
            continue;

        ALLEGRO_BITMAP* img;
        float escala;

        if (inimigo->forca == 1)
        {
            img = renderer->img_enemy;
            escala = 0.30f;
        }
        else
        {
            img = renderer->img_weak_enemy;
            escala = 0.25f;
        }

        int wOriginal = al_get_bitmap_width(img);
        int hOriginal = al_get_bitmap_height(img);

        int w = wOriginal * escala;
        int h = hOriginal * escala;

        int posY = ENEMY_BEGIN_Y - h;

        al_draw_scaled_bitmap(
            img,
            0, 0, wOriginal, hOriginal,
            posXAtual, posY,
            w, h,
            0
        );

        if (combate->alvoSelecionado == i &&
            combate->estadoSelecao == SELECIONANDO_INIMIGO)
        {
            al_draw_rectangle(
                posXAtual - 4, posY - 4,
                posXAtual + w + 4, posY + h + 4,
                al_map_rgb(255, 255, 0),
                4
            );
        }

        RenderHealthBar(
            posXAtual,
            posXAtual + w,
            posY + h + 10,
            inimigo->base.vida,
            inimigo->vidaMax,
            renderer->font
        );

        RenderShieldBar(
            posXAtual,
            posXAtual + w,
            posY + h + 35,
            inimigo->base.escudo,
            inimigo->vidaMax,
            renderer->font
        );

        posXAtual += w + ENEMY_SPACING;
    }

    RenderEnemiesIntents(renderer, combate);
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
