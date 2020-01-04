#ifndef CHECKERSSERVER_SERVERGAME_H
#define CHECKERSSERVER_SERVERGAME_H

#include "player.h"
#include "room.h"

void server_game_move_piece(struct ROOM *room, struct PLAYER *current_player, int from_field, int to_field);

void server_game_end_tour(struct ROOM *room);

void server_game_check_promote_piece(struct ROOM *room, int field);

void server_game_start_game(struct ROOM *room);

#endif //CHECKERSSERVER_SERVERGAME_H
