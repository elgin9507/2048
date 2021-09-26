#include <stdio.h>

#include "board.h"
#include "render.h"


int main(int argc, char *argv[]) {
    board game_board;
    play_game(&game_board);

    return 0;
}
