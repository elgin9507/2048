#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "board.h"


void board_init(board *game_board) {
    int r, c;

    srand(time(NULL));

    for (r = 0; r < BOARD_SIZE; ++r)
        for (c = 0; c < BOARD_SIZE; ++c)
            game_board->cells[r][c] = CELL_EMPTY;

    game_board->won = false;
    game_board->score = 0;
    board_fill_random(game_board);
    board_fill_random(game_board);
}

static bool board_cell_is_empty(board game_board, int row, int col) {
    return game_board.cells[row][col] == CELL_EMPTY;
}

void board_fill_random(board *game_board) {
    int r, c, value;

    do {
        r = rand() % BOARD_SIZE;
        c = rand() % BOARD_SIZE;
    } while (!board_cell_is_empty(*game_board, r, c));
    value = rand() < RAND_MAX * PFOUR ? 4 : 2;
    game_board->cells[r][c] = value;
}

static bool move_left(board *game_board) {
    bool has_changed = false;
    int row, cell, i;
    cell_value_t new_row[BOARD_SIZE];

    for (row = 0; row < BOARD_SIZE; ++row) {
        // clear empty cells
        i = 0;
        for (cell = 0; cell < BOARD_SIZE; ++cell) {
            if (!board_cell_is_empty(*game_board, row, cell)) {
                if (cell != i)
                    has_changed = true;
                new_row[i++] = game_board->cells[row][cell];
            }
        }
        for (; i < BOARD_SIZE; ++i)
            new_row[i] = CELL_EMPTY;

        // sum up equal cells
        for (i = 0; i < BOARD_SIZE-1; ++i) {
            if (new_row[i] == CELL_EMPTY) continue;

            if (new_row[i] == new_row[i+1]) {
                has_changed = true;
                new_row[i] *= 2;
                new_row[i+1] = CELL_EMPTY;
                game_board->score += new_row[i];
                if (new_row[i] == WON_CELL)
                    game_board->won = true;
                i++;
            }
        }

        // set board's new row
        i = 0;
        for (cell = 0; cell < BOARD_SIZE; ++cell)
            if (new_row[cell] != CELL_EMPTY)
                game_board->cells[row][i++] = new_row[cell];
        for (; i < BOARD_SIZE; ++i)
            game_board->cells[row][i] = CELL_EMPTY;
    }

    return has_changed;
}

static void rotate_left(board* game_board) {
    cells_t cells_copy;
    int row, col;

    memcpy(cells_copy, game_board->cells, sizeof(cells_copy));

    for (row = 0; row < BOARD_SIZE; ++row)
        for (col = 0; col < BOARD_SIZE; ++col)
            game_board->cells[row][col] = cells_copy[col][BOARD_SIZE-row-1];
}

static void nrotate_left(board* game_board, int n) {
    int i;
    n = n % 4;

    for (i = 0; i < n; ++i)
        rotate_left(game_board);
}

bool board_move(board *game_board, board_direction_t direction) {
    bool has_changed;

    nrotate_left(game_board, direction);
    has_changed = move_left(game_board);
    nrotate_left(game_board, 4 - direction);

    if (has_changed)
        board_fill_random(game_board);

    return has_changed;
}

bool board_won(board game_board) {
    cell_value_t cell;
    int row, col;

    for (row = 0; row < BOARD_SIZE; ++row)
        for (col = 0; col < BOARD_SIZE; ++col)
            if (game_board.cells[row][col] == WON_CELL)
                return true;

    return false;
}

bool board_lost(board game_board) {
    int row, col;
    cell_value_t up, down, left, right, cur_cell;

    for (row = 0; row < BOARD_SIZE; ++row)
        for (col = 0; col < BOARD_SIZE; ++col) {
            cur_cell = game_board.cells[row][col];

            if (cur_cell == CELL_EMPTY) return false;
            if (row > 0) {
                up = game_board.cells[row-1][col];
                if (up == cur_cell) return false;
            }
            if (row < BOARD_SIZE - 1) {
                down = game_board.cells[row+1][col];
                if (down == cur_cell) return false;
            }
            if (col > 0) {
                left = game_board.cells[row][col-1];
                if (left == cur_cell) return false;
            }
            if (col < BOARD_SIZE -1) {
                right = game_board.cells[row][col+1];
                if (right == cur_cell) return false;
            }
        }

    return true;
}
