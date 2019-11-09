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

enum GAME_ERROR
move_piece(struct GAME_INSTANCE *instance, int from_field, int up_down, int left_right, int num_of_fields) {
    int piece_row = from_field / GAME_BOARD_WIDTH;
    int piece_col = from_field % GAME_BOARD_WIDTH;
    enum GAME_PIECE_COLOR piece_color = _get_piece_color(instance, piece_row, piece_col);
    // Check if the move is correct
    enum GAME_ERROR move_error = ERROR_NO_ERROR;
    if (from_field < 0 || from_field > GAME_BOARD_WIDTH * GAME_BOARD_HEIGHT)
        move_error = ERROR_CANT_MOVE_PIECE;
    // If the piece belongs to the other player
    if (instance->game_state == STATE_LIGHT_TURN && piece_color == COLOR_DARK)
        move_error = ERROR_CANT_MOVE_PIECE;
    else if (instance->game_state == STATE_DARK_TURN && piece_color == COLOR_LIGHT)
        move_error = ERROR_CANT_MOVE_PIECE;

    // If already there was an error there is no reason to proceed
    if (move_error != ERROR_NO_ERROR) return move_error;

    int dest_row = piece_row;
    int dest_col = piece_col;
    int through_row = piece_row;
    int through_col = piece_col;
    // Check if the piece is moving backward and if it is check if it's not a man
    if (up_down > 0 && instance->board[piece_row][piece_col] == PIECE_LIGHT_MAN)
        move_error = ERROR_NOT_KING;
    else if (up_down < 0 && instance->board[piece_row][piece_col] == PIECE_DARK_MAN)
        move_error = ERROR_NO_ERROR;
    switch (num_of_fields) {
        case 1:
            // Calculate the destination field
            dest_row += (up_down > 0) ? 1 : -1;
            dest_col += (left_right > 0) ? 1 : 0;
            break;
        case 2:
            // Calculate the destination field
            dest_row += (up_down > 0) ? 2 : -2;
            dest_col += (left_right > 0) ? 1 : -1;

            // Calulate the position of the piece that will be captured
            if (piece_row % 2 == 0) {
                // Pieces on even rows
                through_row += (up_down > 0) ? 2 : -2;
                through_col += (left_right > 0) ? 1 : 0;
            } else {
                // Pieces on odd rows
                through_row += (up_down > 0) ? 2 : -2;
                through_col += (left_right > 0) ? 0 : -1;
            }
            break;
        default:
            move_error = ERROR_ILLEGAL_MOVE;
            break;
    }
    // Check if the move can be done
    if (dest_row < 0 || dest_row > GAME_BOARD_HEIGHT)
        move_error = ERROR_ILLEGAL_MOVE;
    if (dest_col < 0 || dest_col > GAME_BOARD_WIDTH)
        move_error = ERROR_ILLEGAL_MOVE;
    // Those errors are critical so return if there is one
    if (move_error != ERROR_NO_ERROR)
        return move_error;

    if (instance->board[dest_row][dest_col] != PIECE_NO_PIECE)
        move_error = ERROR_FIELD_TAKEN;
    // If the player tried to capture his own piece
    if (num_of_fields == 2 && _get_piece_color(instance, through_row, through_col) == piece_color)
        move_error = ERROR_ILLEGAL_MOVE;
    if (move_error != ERROR_NO_ERROR)
        return move_error;

    // Moving the piece
    instance->board[dest_row][dest_col] = instance->board[piece_row][piece_col];
    instance->board[piece_row][piece_col] = PIECE_NO_PIECE;
    if (num_of_fields == 2) // Capture
        instance->board[piece_row][piece_col] = PIECE_NO_PIECE;
    return move_error;
}

enum GAME_PIECE_COLOR _get_piece_color(struct GAME_INSTANCE *instance, int row, int col) {
    enum GAME_PIECE_COLOR color = COLOR_NO_COLOR;
    if ((row < 0 || row > GAME_BOARD_HEIGHT) || (col < 0 || col > GAME_BOARD_WIDTH))
        color = COLOR_NO_COLOR;
    else if (instance->board[row][col] > 0)
        color = COLOR_DARK;
    else if (instance->board[row][col] < 0)
        color = COLOR_LIGHT;
    return color;
}

void show_board(struct GAME_INSTANCE *instance) {
    for (int row = GAME_BOARD_HEIGHT - 1; row >= 0; --row) {
        if (row % 2 != 0) printf("  ");
        for (int col = GAME_BOARD_WIDTH-1; col >= 0; --col) {
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
