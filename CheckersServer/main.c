#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main() {
    struct GAME_INSTANCE *instance;
    instance = malloc(sizeof(struct GAME_INSTANCE));
    instance->game_state = STATE_DARK_TURN;
    place_pieces(instance);
    show_board(instance);
    enum GAME_ERROR error;
    error = move_piece(instance, 9, 1, -1, 1);
    printf("%d\n", error);
    show_board(instance);
    free(instance);
    return 0;
}
