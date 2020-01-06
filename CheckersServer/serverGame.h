/**
* @file serverGame.h
* @brief A module joining game and server together.
*
* @author Piotr J. Tomaszewski
*/

#ifndef CHECKERSSERVER_SERVERGAME_H
#define CHECKERSSERVER_SERVERGAME_H

#include "player.h"
#include "room.h"

/**
 * Tries to move the piece to the given field. Informs the players about results
 * @param room A game room in which the game takes place
 * @param current_player A player that sent the move command
 * @param from_field A field on which the piece stands currently
 * @param to_field A field to which the player wanted to move the piece
 * @return true on successful move, false otherwise
 */
bool server_game_move_piece(struct ROOM *room, struct PLAYER *current_player, int from_field, int to_field);

/**
 * Starts a new checkers game. Informs the players about the fact.
 * @param room A room in which the game will take place
 */
void server_game_start_game(struct ROOM *room);

/**
 * Checks if the game should end. Informs the players if the game should end.
 * @param room A room in which the checking takes place
 * @return true if the game should end, false otherwise
 */
bool server_game_check_if_the_game_has_ended(struct ROOM *room);

#endif //CHECKERSSERVER_SERVERGAME_H
