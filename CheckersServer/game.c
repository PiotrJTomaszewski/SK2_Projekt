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
    instance->game_state = STATE_LIGHT_TURN;
}

struct MOVE_RESULT
move_piece(struct GAME_INSTANCE *instance, int from_field, int to_field, enum GAME_PIECE_COLOR player_color) {
    struct MOVE_RESULT move_result;
    move_result.captured_piece_field = -1;
    move_result.end_tour = 0;
    move_result.move_error = ERROR_NO_ERROR;

    enum GAME_STATE state = instance->game_state;
    // Check if it's this player's turn
    if (state == STATE_LIGHT_TURN && player_color == COLOR_DARK) {
        move_result.move_error = ERROR_NOT_YOUR_TURN;
        return move_result;
    }
    if (state == STATE_DARK_TURN && player_color == COLOR_LIGHT) {
        move_result.move_error = ERROR_NOT_YOUR_TURN;
        return move_result;
    }
    int from_row = from_field / GAME_BOARD_WIDTH;
    int from_col = from_field % GAME_BOARD_WIDTH;
    int to_row = to_field / GAME_BOARD_WIDTH;
    int to_col = to_field % GAME_BOARD_WIDTH;

//    printf("fromRC %d,%d\n toRC %d,%d\n", from_row, from_col, to_row, to_col);
    int up_down = to_row > from_row ? 1 : -1;
    int num_of_fields = to_row > from_row ? to_row - from_row : from_row - to_row;
    int left_right;
    if (from_row % 2 == 0) {
        left_right = to_col > from_col ? 1 : -1;
    } else {
        left_right = to_col >= from_col ? 1 : -1;
    }

    enum GAME_PIECE_COLOR piece_color = _get_piece_color(instance, from_row, from_col);
    // Check if the move is correct
    if (from_field < 0 || from_field > GAME_BOARD_WIDTH * GAME_BOARD_HEIGHT)
        move_result.move_error = ERROR_CANT_MOVE_PIECE;
    // If the piece belongs to the other player
    if (instance->game_state == STATE_LIGHT_TURN && piece_color == COLOR_DARK)
        move_result.move_error = ERROR_CANT_MOVE_PIECE;
    else if (instance->game_state == STATE_DARK_TURN && piece_color == COLOR_LIGHT)
        move_result.move_error = ERROR_CANT_MOVE_PIECE;

    // If already there was an error there is no reason to proceed
    if (move_result.move_error != ERROR_NO_ERROR) return move_result;

    int through_row = from_row;
    int through_col = from_col;
    // Check if there is any piece on this field
    if (instance->board[from_row][from_col] == PIECE_NO_PIECE)
        move_result.move_error = ERROR_CANT_MOVE_PIECE;
    // Check if the piece is moving backward and if it is check if it's not a man
    if (up_down > 0 && instance->board[from_row][from_col] == PIECE_LIGHT_MAN)
        move_result.move_error = ERROR_NOT_KING;
    else if (up_down < 0 && instance->board[from_row][from_col] == PIECE_DARK_MAN)
        move_result.move_error = ERROR_NOT_KING;

    // Calculate the position of the piece that will be captured
    if (num_of_fields == 2) {
        if (from_row % 2 == 0) {
            // Pieces on even rows
            through_row += (up_down > 0) ? 1 : -1;
            through_col += (left_right > 0) ? 1 : 0;
        } else {
            // Pieces on odd rows
            through_row += (up_down > 0) ? 1 : -1;
            through_col += (left_right > 0) ? 0 : -1;
        }
    }
    if (num_of_fields > 2) {
        move_result.move_error = ERROR_ILLEGAL_MOVE;
    }
    // Check if the move can be done
    if (to_row < 0 || to_row > GAME_BOARD_HEIGHT)
        move_result.move_error = ERROR_ILLEGAL_MOVE;
    if (to_col < 0 || to_col > GAME_BOARD_WIDTH)
        move_result.move_error = ERROR_ILLEGAL_MOVE;
    // Those errors are critical so return if there is one
    if (move_result.move_error != ERROR_NO_ERROR)
        return move_result;

    if (instance->board[to_row][to_col] != PIECE_NO_PIECE)
        move_result.move_error = ERROR_FIELD_TAKEN;
    // If the player tried to capture his own piece
    if (num_of_fields == 2 && _get_piece_color(instance, through_row, through_col) == piece_color)
        move_result.move_error = ERROR_ILLEGAL_MOVE;
    if (move_result.move_error != ERROR_NO_ERROR)
        return move_result;

    // Moving the piece
    instance->board[to_row][to_col] = instance->board[from_row][from_col];
    instance->board[from_row][from_col] = PIECE_NO_PIECE;
    move_result.end_tour = 1;
    if (num_of_fields == 2) { // Capture
        instance->board[through_row][through_col] = PIECE_NO_PIECE;
        move_result.captured_piece_field = GAME_BOARD_WIDTH * through_row + through_col;
        move_result.end_tour = 0; // TODO: Only if still can move
    }
    return move_result;
}

int check_and_promote(struct GAME_INSTANCE *instance, int field) {
    int row = field / GAME_BOARD_WIDTH;
    int col = field % GAME_BOARD_WIDTH;
    if (instance->board[row][col] == PIECE_LIGHT_MAN && row == 0) {
        instance->board[row][col] = PIECE_LIGHT_KING;
        return 1;
    }
    if (instance->board[row][col] == PIECE_DARK_MAN && row == GAME_BOARD_HEIGHT - 1) {
        instance->board[row][col] = PIECE_DARK_KING;
        return 1;
    }
    return 0;
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
        for (int col = GAME_BOARD_WIDTH - 1; col >= 0; --col) {
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

void show_board2(struct GAME_INSTANCE *instance) {
    for (int row = 0; row < GAME_BOARD_HEIGHT; ++row) {
        if (row % 2 != 0) printf("  ");
        for (int col = 0; col < GAME_BOARD_WIDTH; --col) {
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
