/**
* @file game.h
* @brief Main logic for the checkers game.
*
* @author Piotr J. Tomaszewski
*/

#ifndef CHECKERSSERVER_GAME_H
#define CHECKERSSERVER_GAME_H

/**
 * @brief Width of the game board.
 */
#define GAME_BOARD_WIDTH 4

/**
 * @brief Height of the game board.
 */
#define GAME_BOARD_HEIGHT 8

/**
 * @brief Available types of pieces that can be placed on the board.
 */
enum GAME_PIECE_TYPE {
    PIECE_LIGHT_KING = -2, PIECE_LIGHT_MAN, PIECE_NO_PIECE, PIECE_DARK_MAN, PIECE_DARK_KING
};

/**
 * @brief Available piece / player colors.
 */
enum GAME_PIECE_COLOR {
    COLOR_NO_COLOR, COLOR_LIGHT, COLOR_DARK
};

/**
 * @brief Errors that may be encountered in game.
 */
enum GAME_ERROR {
    /// There is no error.
            ERROR_NO_ERROR = 0,
    /// This piece don't exist or can't be moved by current player.
            ERROR_CANT_MOVE_PIECE,
    /// This move is illegal
            ERROR_ILLEGAL_MOVE,
    /// The destination field is taken
            ERROR_FIELD_TAKEN,
    /// Only king can move backward
            ERROR_NOT_KING,
    /// It's not this player's turn
            ERROR_NOT_YOUR_TURN
};

/**
 * @brief A state of the game.
 */
enum GAME_STATE {
    STATE_NO_GAME,
    /// Light colored player's turn.
            STATE_LIGHT_TURN,
    /// Dark colored player's turn.
            STATE_DARK_TURN
};
/**
 * @brief A structure that holds all information about an instance of the game.
 */
struct GAME_INSTANCE {
    /// A game board array.
    enum GAME_PIECE_TYPE board[GAME_BOARD_HEIGHT][GAME_BOARD_WIDTH];
    enum GAME_STATE game_state;
};

struct MOVE_RESULT {
    enum GAME_ERROR move_error;
    int end_tour;
    int captured_piece_field;
};

/**
 * @brief Place game pieces on the game board.
 * @param instance A pointer to the game instance structure.
 * @see GAME_INSTANCE
 */
void place_pieces(struct GAME_INSTANCE *instance);

/**
 * @brief Move a game piece on the board.
 * @param instance A pointer to the game instance structure.
 * @param from_field Number of the field on which the piece stands. A number in range [0,board_height*board_width).
 * @param up_down Greater than 0 - piece moving up, less or equal 0 moving down.
 * @param left_right Greater than 0 - piece moving left, less or equal 0 moving right.
 * @param num_of_fields Number of fields that the piece is moving.
 * @return A structure // TODO: Fill this
 * @see GAME_INSTANCE
 * @see GAME_ERROR
 */
struct MOVE_RESULT
move_piece(struct GAME_INSTANCE *instance, int from_field, int to_field, enum GAME_PIECE_COLOR player_color);

int check_and_promote(struct GAME_INSTANCE *instance, int field);

/**
 * @brief Get color of the given piece.
 * @param instance A pointer to the game instance structure.
 * @param row Piece's row.
 * @param col Piece's column.
 * @return An error code.
 * @see GAME_INSTANCE
 */
enum GAME_PIECE_COLOR _get_piece_color(struct GAME_INSTANCE *instance, int row, int col);

// DEBUG ONLY FUNCTIONS
void show_board(struct GAME_INSTANCE *instance);


#endif //CHECKERSSERVER_GAME_H
