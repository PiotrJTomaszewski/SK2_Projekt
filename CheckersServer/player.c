#include <malloc.h>
#include "player.h"

void player_assign_room(struct PLAYER *player, struct ROOM *room) {
    player->room = room;
    player->is_in_room = 1;
    if (room->number_of_players == 0) {
        room->player_one = player;
    } else {
        room->player_two = player;
    }
    room->number_of_players++;
}

void player_leave_room(struct PLAYER *player) {
    player->is_in_room = 0;
    if (player->room->player_one->file_descriptor == player->file_descriptor)
        player->room->player_one = NULL;
    else
        player->room->player_two = NULL;
    player->room->number_of_players--;
    player->room = NULL;
}

int player_is_in_free_room(struct PLAYER *player) {
    if (player->is_in_room) {
        return player->room->number_of_players == 1;
    }
    return 0;
}
