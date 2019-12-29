#ifndef CHECKERSSERVER_CLIENT_H
#define CHECKERSSERVER_CLIENT_H
#include "room.h"

struct PLAYER {
    int file_descriptor;
    int is_in_room;
    struct ROOM *room;
    enum GAME_PIECE_COLOR player_color;
};

void player_assign_room(struct PLAYER *player, struct ROOM *room);

void player_leave_room(struct PLAYER *player);


/**
 * Checks if the player is in a free room.
 * @param player
 * @return
 */
int player_is_in_free_room(struct PLAYER *player);

#endif //CHECKERSSERVER_CLIENT_H
