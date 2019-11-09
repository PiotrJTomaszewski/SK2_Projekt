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
 * @brief A structure that holds all information about an instance of the game.
 */
struct GAME_INSTANCE {
    /// A game board array.
    enum GAME_PIECE_TYPE board[GAME_BOARD_HEIGHT][GAME_BOARD_WIDTH];
};

/**
 * @brief Place game pieces on the game board.
 * @param instance A pointer to the game instance structure.
 * @see GAME_INSTANCE
 */
void place_pieces(struct GAME_INSTANCE *instance);

// DEBUG ONLY FUNCTIONS
void show_board(struct GAME_INSTANCE *instance);

#endif //CHECKERSSERVER_GAME_H
