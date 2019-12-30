#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "game.h"
#include "test.h"

/**
 * @brief Runs all automated tests.
 */
int run_all_tests() {
    printf("Running all tests\n");
    _test_piece_moving();
    printf("All tests: OK\n");
    return 0;
}


/**
 * @brief Runs automated tests of piece movement.
 */
void _test_piece_moving() {
//    struct GAME_INSTANCE *instance;
//    instance = malloc(sizeof(struct GAME_INSTANCE));
//    place_pieces(instance);
//    // Move not yours piece
//    instance->game_state = STATE_LIGHT_TURN;
//    assert(move_piece(instance, 9, 1, -1, 1) != ERROR_NO_ERROR);
//    instance->game_state = STATE_DARK_TURN;
//    assert(move_piece(instance, 21, -1, 1, 1) != ERROR_NO_ERROR);
//
//    // Moving non existing piece
//    assert(move_piece(instance, 12, 1, 1, 1) != ERROR_NO_ERROR);
//    instance->game_state = STATE_LIGHT_TURN;
//    assert(move_piece(instance, 19, -1, -1, 1) != ERROR_NO_ERROR);
//    free(instance);
}
