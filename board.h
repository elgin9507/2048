#ifndef __BOARD_H
#define __BOARD_H

#include <stdbool.h>
#include <stdlib.h>

#define BOARD_SIZE      4
#define CELL_EMPTY      0
#define PFOUR           0.1
#define WON_CELL        2048

typedef unsigned short cell_value_t;
typedef unsigned short board_score_t;
typedef cell_value_t cells_t[BOARD_SIZE][BOARD_SIZE];
typedef struct board {
    cells_t cells;
    bool won;
    board_score_t score;
} board;
typedef enum BOARD_DIRECTION {
    BOARD_DIR_LEFT   = 0,
    BOARD_DIR_UP     = 1,
    BOARD_DIR_RIGHT  = 2,
    BOARD_DIR_DOWN   = 3
} board_direction_t;

static bool board_cell_is_empty(board, int, int);
static bool move_left(board*);
static void rotate_left(board*);
static void nrotate_left(board*, int n);

void board_init(board*);
void board_fill_random(board*);
bool board_move(board*, board_direction_t);
bool board_won(board);
bool board_lost(board);

#endif
