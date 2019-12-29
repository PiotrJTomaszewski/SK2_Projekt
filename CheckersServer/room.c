#include <malloc.h>
#include "room.h"

void room_init(struct ROOM *room) {
    room->number_of_players = 0;
    room->player_one = NULL;
    room->player_two = NULL;
}

void room_free_memory(struct ROOM *room) {
    free(room->game_instance);
}

//void room_start_game(struct ROOM *room) {
//    room->game_instance = malloc(sizeof(struct GAME_INSTANCE));
//}
