#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// =====================================================================
// GAME KEYS
// =====================================================================
#define GAME_KEY_SEEN 1
#define GAME_KEY_DOWN 2

// =====================================================================
// DISPLAY
// =====================================================================
#define DISPLAY_BUFFER_WIDTH 1920.0
#define DISPLAY_BUFFER_HEIGHT 1080.0

#define DISPLAY_SCALE 1.0
#define DISPLAY_WIDTH (DISPLAY_SCALE * DISPLAY_BUFFER_WIDTH)
#define DISPLAY_HEIGHT (DISPLAY_SCALE * DISPLAY_BUFFER_HEIGHT)


// =====================================================================
// DECKS
// =====================================================================
#define DRAW_DECK_X 30
#define DRAW_DECK_Y 650
#define DECK_WIDTH 80
#define DECK_HEIGHT 80


// =====================================================================
// PLAYER
// =====================================================================
#define PLAYER_BEGIN_X 550
#define PLAYER_BEGIN_Y 530
#define PLAYER_RADIUS   100

#define PLAYER_SPRITE_WIDTH  260
#define PLAYER_SPRITE_HEIGHT 340


// =====================================================================
// HEALTH / SHIELD BARS
// =====================================================================
#define HEALTH_BAR_HEIGHT 20
#define HEALTH_BAR_RX     0
#define HEALTH_BAR_RY     0


// =====================================================================
// CARDS (PLAYER HAND)
// =====================================================================
#define HAND_BEGIN_Y 750

#define CARD_WIDTH  280
#define CARD_HEIGHT 420

// spacing entre cartas
#define CARDS_SPACING 20

#define MAXCARDS 5


// =====================================================================
// ENERGY UI
// =====================================================================
#define ENERGY_X 120
#define ENERGY_Y 120
#define ENERGY_RADIUS 70
#define PLAYER_MAX_ENERGY 3


// =====================================================================
// ENEMIES
// =====================================================================
#define NUMINIMIGOS 2

#define ENEMY_BEGIN_X 900
#define ENEMY_BEGIN_Y 700 

#define ENEMY_RADIUS  90
#define ENEMY_SPACING 170

#define ENEMY_SPRITE_WIDTH  200
#define ENEMY_SPRITE_HEIGHT 260
#define ENEMY_SELECTION_RADIUS 100


// =====================================================================
// AUXILIAR ENUMS(NAO USADO)
// =====================================================================
typedef enum {
    MOVE_LEFT = 0,
    MOVE_RIGHT = 1,
} CursorMovementDirection;

#endif // _CONSTANTS_H_
