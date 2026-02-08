#pragma once

// Render params
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_FPS 60
#define SCREEN_OFFSET 20

// Font sizes
#define TITLE_FONT_SIZE 20
#define VALUE_FONT_SIZE 25
#define INDEX_FONT_SIZE 10

// Shape sizes
#define SQUARE_SIZE 75
#define TILE_SIZE 25
#define MAX_TILES_WIDTH (SCREEN_WIDTH/TILE_SIZE)
#define MAX_TILES_HEIGHT (SCREEN_HEIGHT/TILE_SIZE)
#define RECTANGLE_THICK 3
#define BUTTON_WIDTH 35
#define BUTTON_HEIGHT 25
#define BUTTON_OFFSET 2

// Custom colors
#define CELL_COLOR      (Color){247, 114, 69, 255}
#define BG_COLOR        (Color){238, 230, 212, 255}
