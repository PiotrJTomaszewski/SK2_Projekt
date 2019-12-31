#include <malloc.h>
#include "player.h"

void player_init(struct PLAYER *player) {
    player->buffer = malloc(sizeof(struct CIRC_BUFFER));
    circ_buffer_init(player->buffer, 128);
}

void player_free_memory(struct PLAYER *player) {
    circ_buffer_free_memory(player->buffer);
    free(player->buffer);
}

void player_assign_room(struct PLAYER *player, struct ROOM *room) {
    player->room = room;
    if (room->number_of_players == 0) {
        room->player_one = player;
    } else {
        room->player_two = player;
    }
    room->number_of_players++;
}

void player_leave_room(struct PLAYER *player) {
    if (player->room != NULL) {
        if (player->room->player_one != NULL && player->room->player_one->file_descriptor == player->file_descriptor)
            player->room->player_one = NULL;
        else
            player->room->player_two = NULL;
        player->room->number_of_players--;
        if (player->room->number_of_players == 0) {
            room_free_memory(player->room);
        }
        player->room = NULL;
    }
}

int player_is_in_free_room(struct PLAYER *player) {
    if (player->room != NULL) {
        return player->room->number_of_players == 1;
    }
    return 0;
}
