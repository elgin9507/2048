#ifndef __RENDER_H
#define __RENDER_H

#include "board.h"

#define GRID_STARTY        1
#define GRID_STARTX        2
#define GRID_BLOCK_SIZE    BOARD_SIZE
#define GRID_BLOCK_HEIGHT  3
#define GRID_BLOCK_WIDTH   7
#define GRID_ROWS          GRID_BLOCK_SIZE * (GRID_BLOCK_HEIGHT + 1) + 1
#define GRID_COLS          GRID_BLOCK_SIZE * (GRID_BLOCK_WIDTH + 1) + 1
#define GRID_MARGIN_BELOW  1
#define GRID_MARGIN_RIGHT  1
#define SCORE_STARTY       GRID_STARTY + GRID_ROWS + GRID_MARGIN_BELOW
#define SCORE_STARTX       GRID_STARTX
#define SCORE_ROWS         1
#define SCORE_COLS         GRID_COLS
#define INFO_STARTY        GRID_STARTY
#define INFO_STARTX        GRID_STARTX + GRID_COLS + GRID_MARGIN_RIGHT
#define INFO_ROWS          8
#define INFO_COLS          20

#define COLOR_GREY_DARK   8
#define COLOR_GREY_LIGHT  9
#define COLOR_SCORE_FG    COLOR_BLACK
#define COLOR_SCORE_BG    COLOR_WHITE

#define COLOR_2_FG        COLOR_BLACK
#define COLOR_2_BG        COLOR_WHITE
#define COLOR_4_FG        COLOR_BLACK
#define COLOR_4_BG        10
#define COLOR_8_FG        COLOR_BLACK
#define COLOR_8_BG        11
#define COLOR_16_FG       COLOR_BLACK
#define COLOR_16_BG       12
#define COLOR_32_FG       COLOR_BLACK
#define COLOR_32_BG       13
#define COLOR_64_FG       COLOR_BLACK
#define COLOR_64_BG       14
#define COLOR_128_FG      COLOR_BLACK
#define COLOR_128_BG      15
#define COLOR_256_FG      COLOR_BLACK
#define COLOR_256_BG      16
#define COLOR_512_FG      COLOR_BLACK
#define COLOR_512_BG      17
#define COLOR_1024_FG     COLOR_BLACK
#define COLOR_1024_BG     18
#define COLOR_2048_FG     COLOR_BLACK
#define COLOR_2048_BG     19

#define COLOR_WHITE2_RGB        980, 973, 937
#define COLOR_GREY_DARK_RGB     467, 431, 396
#define COLOR_GREY_LIGHT_RGB    804, 757, 706
#define COLOR_4_BG_RGB          933, 882, 800
#define COLOR_8_BG_RGB          953, 698, 478
#define COLOR_16_BG_RGB         965, 588, 392
#define COLOR_32_BG_RGB         961, 482, 373
#define COLOR_64_BG_RGB         953, 369, 227
#define COLOR_128_BG_RGB        925, 816, 451
#define COLOR_256_BG_RGB        929, 800, 384
#define COLOR_512_BG_RGB        929, 788, 314
#define COLOR_1024_BG_RGB       929, 769, 247
#define COLOR_2048_BG_RGB       929, 761, 180

#define COLOR_PAIR_GRID_BORDER        1
#define COLOR_PAIR_EMPTY_CELL         2
#define COLOR_PAIR_SCORE              3
#define COLOR_PAIR_VALUE_2            4
#define COLOR_PAIR_VALUE_4            5
#define COLOR_PAIR_VALUE_8            6
#define COLOR_PAIR_VALUE_16           7
#define COLOR_PAIR_VALUE_32           8
#define COLOR_PAIR_VALUE_64           9
#define COLOR_PAIR_VALUE_128          10
#define COLOR_PAIR_VALUE_256          11
#define COLOR_PAIR_VALUE_512          12
#define COLOR_PAIR_VALUE_1024         13
#define COLOR_PAIR_VALUE_2048         14

#define COLOR_PAIR_FOR_VALUE(val) COLOR_PAIR(COLOR_PAIR_VALUE_ ## val)

#define INPUT_EXIT     'q'
#define INPUT_RESTART  'r'
#define INPUT_UP1      'w'
#define INPUT_UP2      'k'
#define INPUT_RIGHT1   'd'
#define INPUT_RIGHT2   'l'
#define INPUT_DOWN1    's'
#define INPUT_DOWN2    'j'
#define INPUT_LEFT1    'a'
#define INPUT_LEFT2    'h'

static void get_cell_rc(int, int, int*, int*);
static int get_color_by_cell_value(int);
static void draw_info(void);

void init_colors(void);
void render_init(board);
void draw_borders(void);
void draw_cells(cells_t);
void draw_score(board_score_t);
void render_end(void);
void draw_game_end(bool);
void play_game(board*);

#endif
