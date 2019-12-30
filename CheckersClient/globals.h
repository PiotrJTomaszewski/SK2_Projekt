#ifndef GLOBALS_H
#define GLOBALS_H
namespace GLOBAL {
enum PIECE_TYPE {NO_PIECE, MAN, KING};
enum COLOR {LIGHT, DARK};
enum GAME_STATUS {TURN_LIGHT, TURN_DARK};
enum GAME_ERROR {
    // There was no error
    NO_ERROR,
    // This piece don't exist or can't be moved by current player.
    ERROR_CANT_MOVE_PIECE,
    // This move is illegal
    ERROR_ILLEGAL_MOVE,
    // The destination field is taken
    ERROR_FIELD_TAKEN,
    // Only king can move backward
    ERROR_NOT_KING,
    // It's not this player's turn
    ERROR_NOT_YOUR_TURN
};
}
#endif // GLOBALS_H
