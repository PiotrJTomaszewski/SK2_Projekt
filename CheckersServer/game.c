#include <string.h>
#include <stdio.h>
#include "game.h"

void place_pieces(struct GAME_INSTANCE *instance) {
    for (int col = 0; col < GAME_BOARD_WIDTH; ++col) {
        // Place dark pieces
        for (int row = 0; row < 3; ++row)
            instance->board[row][col] = PIECE_DARK_MAN;
        // Make sure there is nothing on the empty fields
        for (int row = 3; row < 5; ++row)
            instance->board[row][col] = PIECE_NO_PIECE;
        // Place light pieces
        for (int row = 5; row < 8; ++row)
            instance->board[row][col] = PIECE_LIGHT_MAN;
    }
}

void show_board(struct GAME_INSTANCE *instance) {
    for (int row = GAME_BOARD_HEIGHT - 1; row >= 0; --row) {
        if (row % 2 != 0) printf("  ");
        for (int col = 0; col < GAME_BOARD_WIDTH; ++col) {
            switch (instance->board[row][col]) {
                case PIECE_LIGHT_MAN:
                    printf("LM");
                    break;
                case PIECE_LIGHT_KING:
                    printf("LK");
                    break;
                case PIECE_DARK_MAN:
                    printf("DM");
                    break;
                case PIECE_DARK_KING:
                    printf("DK");
                    break;
                case PIECE_NO_PIECE:
                    printf("..");
                    break;
                default:
                    break;
            }
            printf("  ");
        }
        printf("\n");
    }
}
