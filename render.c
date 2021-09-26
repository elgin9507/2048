#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

#include "board.h"
#include "render.h"

WINDOW *board_win;
WINDOW *score_win;
WINDOW *info_win;

void render_init(board game_board) {
    initscr();
    noecho();
    init_colors();
    keypad(stdscr, TRUE);

    board_win = newwin(GRID_ROWS, GRID_COLS, GRID_STARTY, GRID_STARTX);
    score_win = newwin(SCORE_ROWS, SCORE_COLS, SCORE_STARTY, SCORE_STARTX);
    info_win = newwin(INFO_ROWS, INFO_COLS, INFO_STARTY, INFO_STARTX);
    refresh();

    draw_borders();
    draw_cells(game_board.cells);
    draw_score(game_board.score);
    draw_info();

    wrefresh(board_win);
    wrefresh(score_win);
    wrefresh(info_win);
}

void init_colors() {
    if (!has_colors() || !can_change_color()) {
        render_end();
        fprintf(stderr, "this terminal doesn't support colors!\n");
        exit(EXIT_FAILURE);
    }
    start_color();

    init_color(COLOR_GREY_DARK, COLOR_GREY_DARK_RGB);
    init_color(COLOR_GREY_LIGHT, COLOR_GREY_LIGHT_RGB);
    init_color(COLOR_4_BG, COLOR_4_BG_RGB);
    init_color(COLOR_8_BG, COLOR_8_BG_RGB);
    init_color(COLOR_16_BG, COLOR_16_BG_RGB);
    init_color(COLOR_32_BG, COLOR_32_BG_RGB);
    init_color(COLOR_64_BG, COLOR_64_BG_RGB);
    init_color(COLOR_128_BG, COLOR_128_BG_RGB);
    init_color(COLOR_256_BG, COLOR_256_BG_RGB);
    init_color(COLOR_512_BG, COLOR_512_BG_RGB);
    init_color(COLOR_1024_BG, COLOR_1024_BG_RGB);
    init_color(COLOR_2048_BG, COLOR_2048_BG_RGB);

    init_pair(COLOR_PAIR_GRID_BORDER, COLOR_GREY_DARK, COLOR_GREY_DARK);
    init_pair(COLOR_PAIR_EMPTY_CELL, COLOR_GREY_LIGHT, COLOR_GREY_LIGHT);
    init_pair(COLOR_PAIR_SCORE, COLOR_SCORE_FG, COLOR_SCORE_BG);
    init_pair(COLOR_PAIR_VALUE_2, COLOR_2_FG, COLOR_2_BG);
    init_pair(COLOR_PAIR_VALUE_4, COLOR_4_FG, COLOR_4_BG);
    init_pair(COLOR_PAIR_VALUE_8, COLOR_8_FG, COLOR_8_BG);
    init_pair(COLOR_PAIR_VALUE_16, COLOR_16_FG, COLOR_16_BG);
    init_pair(COLOR_PAIR_VALUE_32, COLOR_32_FG, COLOR_32_BG);
    init_pair(COLOR_PAIR_VALUE_64, COLOR_64_FG, COLOR_64_BG);
    init_pair(COLOR_PAIR_VALUE_128, COLOR_128_FG, COLOR_128_BG);
    init_pair(COLOR_PAIR_VALUE_256, COLOR_256_FG, COLOR_256_BG);
    init_pair(COLOR_PAIR_VALUE_512, COLOR_512_FG, COLOR_512_BG);
    init_pair(COLOR_PAIR_VALUE_1024, COLOR_1024_FG, COLOR_1024_BG);
    init_pair(COLOR_PAIR_VALUE_2048, COLOR_2048_FG, COLOR_2048_BG);
}

void draw_borders() {
    int r, c;
    chtype ch;

    for (r = 0; r < GRID_ROWS; ++r) {
        for (c = 0; c < GRID_COLS; ++c) {
            if (r == 0 && c == 0)
                ch = ACS_ULCORNER;
            else if (r == 0 && c == (GRID_COLS-1))
                ch = ACS_URCORNER;
            else if (r == 0 && c % (GRID_BLOCK_WIDTH+1) == 0)
                ch = ACS_TTEE;
            else if (r == (GRID_ROWS-1) && c == 0)
                ch = ACS_LLCORNER;
            else if (r == (GRID_ROWS-1) && c == (GRID_COLS-1))
                ch = ACS_LRCORNER;
            else if (r % (GRID_BLOCK_HEIGHT+1) == 0 && c == 0)
                ch = ACS_LTEE;
            else if (r % (GRID_BLOCK_HEIGHT+1) == 0 && c == (GRID_COLS-1))
                ch = ACS_RTEE;
            else if (r == (GRID_ROWS-1) && c % (GRID_BLOCK_WIDTH+1) == 0)
                ch = ACS_BTEE;
            else if (r % (GRID_BLOCK_HEIGHT+1) == 0 && c % (GRID_BLOCK_WIDTH+1) == 0)
                ch = ACS_PLUS;
            else if (r % (GRID_BLOCK_HEIGHT+1) == 0)
                ch = ACS_HLINE;
            else if (c % (GRID_BLOCK_WIDTH+1) == 0)
                ch = ACS_VLINE;
            else
                continue;

            wattron(board_win, COLOR_PAIR(COLOR_PAIR_GRID_BORDER));
            mvwaddch(board_win, r, c, ch);
            wattroff(board_win, COLOR_PAIR(COLOR_PAIR_GRID_BORDER));
        }
    }
}

void draw_cells(cells_t cells) {
    int cell_row, cell_col, r, c, r_rel, c_rel;
    cell_value_t cell_val;
    char cell_val_str[5];
    size_t cell_val_len;
    int cell_num_row_rem, cell_num_col_start_rem, cell_num_col_end_rem, color;

    cell_num_row_rem = GRID_BLOCK_HEIGHT / 2 + 1;

    for (r = 0; r < GRID_ROWS; ++r) {

        for (c = 0; c < GRID_COLS; ++c) {
            r_rel = r % (GRID_BLOCK_HEIGHT+1);
            c_rel = c % (GRID_BLOCK_WIDTH+1);

            if (r_rel == 0 || c_rel == 0)
                continue;

            get_cell_rc(r, c, &cell_row, &cell_col);
            cell_val = cells[cell_row][cell_col];

            if (cell_val == CELL_EMPTY) {
                wattron(board_win, COLOR_PAIR(COLOR_PAIR_EMPTY_CELL));
                mvwaddch(board_win, r, c, ' ');
                wattroff(board_win, COLOR_PAIR(COLOR_PAIR_EMPTY_CELL));
            } else {
                color = get_color_by_cell_value(cell_val);

                wattron(board_win, color);

                if (r_rel == cell_num_row_rem) {
                    sprintf(cell_val_str, "%d", cell_val);
                    cell_val_len = strlen(cell_val_str);
                    cell_num_col_start_rem = (GRID_BLOCK_WIDTH - cell_val_len) / 2 + 1;
                    cell_num_col_end_rem = cell_num_col_start_rem + cell_val_len;
                    if (c_rel >= cell_num_col_start_rem && c_rel < cell_num_col_end_rem)
                        mvwaddch(board_win, r, c, cell_val_str[c_rel-cell_num_col_start_rem]);
                    else
                        mvwaddch(board_win, r, c, ' ');
                } else
                    mvwaddch(board_win, r, c, ' ');

                wattroff(board_win, color);
            }

        }
    }

    wrefresh(board_win);
}

static void get_cell_rc(int grid_row, int grid_col,  int *cell_row, int *cell_col) {
    *cell_row = grid_row / (GRID_BLOCK_HEIGHT+1);
    *cell_col = grid_col / (GRID_BLOCK_WIDTH+1);
}

static int get_color_by_cell_value(int value) {
    int color;

    switch (value) {
        case 2:
            color = COLOR_PAIR_FOR_VALUE(2);
            break;
        case 4:
            color = COLOR_PAIR_FOR_VALUE(4);
            break;
        case 8:
            color = COLOR_PAIR_FOR_VALUE(8);
            break;
        case 16:
            color = COLOR_PAIR_FOR_VALUE(16);
            break;
        case 32:
            color = COLOR_PAIR_FOR_VALUE(32);
            break;
        case 64:
            color = COLOR_PAIR_FOR_VALUE(64);
            break;
        case 128:
            color = COLOR_PAIR_FOR_VALUE(128);
            break;
        case 256:
            color = COLOR_PAIR_FOR_VALUE(256);
            break;
        case 512:
            color = COLOR_PAIR_FOR_VALUE(512);
            break;
        case 1024:
            color = COLOR_PAIR_FOR_VALUE(1024);
            break;
        case 2048:
            color = COLOR_PAIR_FOR_VALUE(2048);
            break;
        default:
            color = COLOR_PAIR(COLOR_PAIR_EMPTY_CELL);
            break;
    }

    color |= A_BOLD;

    return color;
}

void draw_score(board_score_t score) {
    wattron(score_win, COLOR_PAIR(COLOR_PAIR_SCORE));
    mvwprintw(score_win, 0, 0, "Score : %hu", score);
    wrefresh(score_win);
}

void draw_info() {
    wprintw(info_win, "Controls:\n\n");
    wprintw(info_win, "w / ");
    waddch(info_win, ACS_UARROW);
    wprintw(info_win, " - move up\n");
    wprintw(info_win, "s / ");
    waddch(info_win, ACS_DARROW);
    wprintw(info_win, " - move down\n");
    wprintw(info_win, "a / ");
    waddch(info_win, ACS_LARROW);
    wprintw(info_win, " - move left\n");
    wprintw(info_win, "d / ");
    waddch(info_win, ACS_RARROW);
    wprintw(info_win, " - move right\n");
    wprintw(info_win, "q - quit game\n");
    wprintw(info_win, "r - restart game\n");

    wrefresh(info_win);
}

void draw_game_end(bool won) {
    wattron(score_win, COLOR_PAIR(COLOR_PAIR_SCORE));

    if (won)
        mvwprintw(score_win, 0, 0, "Game ended. You won!");
    else
        mvwprintw(score_win, 0, 0, "Game ended. You lost!");

    wrefresh(score_win);

}

void render_end() {
    endwin();
}

void play_game(board *game_board) {
    int input;
    bool changed;

    board_init(game_board);
    render_init(*game_board);

    while ((input = getch()) != INPUT_EXIT) {
        switch (input) {
            case INPUT_UP:
            case KEY_UP:
                changed = board_move(game_board, BOARD_DIR_UP);
                break;
            case INPUT_DOWN:
            case KEY_DOWN:
                changed = board_move(game_board, BOARD_DIR_DOWN);
                break;
            case INPUT_RIGHT:
            case KEY_RIGHT:
                changed = board_move(game_board, BOARD_DIR_RIGHT);
                break;
            case INPUT_LEFT:
            case KEY_LEFT:
                changed = board_move(game_board, BOARD_DIR_LEFT);
                break;
            case INPUT_RESTART:
                goto exit_input_loop;
                break;
            default:
                changed = false;
                break;
        }

        if (game_board->won || board_lost(*game_board)) {
            draw_cells(game_board->cells);
            draw_game_end(game_board->won);
            getch();
            break;
        }

        if (changed) {
            draw_cells(game_board->cells);
            draw_score(game_board->score);
        }
    }
    exit_input_loop: ;

    if (input == INPUT_RESTART)
        play_game(game_board);

    render_end();
}
