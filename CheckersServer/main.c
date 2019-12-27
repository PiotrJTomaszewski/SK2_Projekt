#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "test.h"
#include "server.h"
int main() {
//    struct GAME_INSTANCE *instance;
//    instance = malloc(sizeof(struct GAME_INSTANCE));
//    place_pieces(instance);
//    show_board(instance);
//    enum GAME_ERROR error;
//    error = move_piece(instance, 12, 1, 1, 1);
//    printf("%d\n", error);
//    show_board(instance);
//    free(instance);
//    run_all_tests();
    server_run(1234);
    return 0;
}
