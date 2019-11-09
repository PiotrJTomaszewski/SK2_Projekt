#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main() {
    struct GAME_INSTANCE *instance;
    instance = malloc(sizeof(struct GAME_INSTANCE));
    place_pieces(instance);
    show_board(instance);
    free(instance);
    return 0;
}
