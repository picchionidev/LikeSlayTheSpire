#ifndef RENDERER_H
#define RENDERER_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "combate.h"


typedef struct {
    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP* display_buffer;
    ALLEGRO_FONT* font;

ALLEGRO_BITMAP *img_bg;

ALLEGRO_BITMAP *img_card_ataque;
ALLEGRO_BITMAP *img_card_defesa;
ALLEGRO_BITMAP *img_card_especial;

ALLEGRO_BITMAP *img_enemy;
ALLEGRO_BITMAP* img_weak_enemy;
ALLEGRO_BITMAP *img_player;

ALLEGRO_BITMAP *img_energy_ring;

ALLEGRO_BITMAP *img_pile;

ALLEGRO_BITMAP *img_health_bar;
ALLEGRO_BITMAP *img_shield_bar;

ALLEGRO_BITMAP *img_combat_screen;
ALLEGRO_BITMAP *img_game_over_screen;
ALLEGRO_BITMAP *img_victory_screen;

} Renderer;

// Funções públicas
void FillRenderer(Renderer* renderer);
void Render(Renderer* renderer, Combate *combate);
void ClearRenderer(Renderer* renderer);
void RenderVictoryScreen(Renderer* renderer, int totalVitorias);
void RenderGameOverScreen(Renderer* renderer);
void RenderCombatVictory(Renderer* renderer);
#endif
