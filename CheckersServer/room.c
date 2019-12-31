#include <malloc.h>
#include "room.h"
#include "player.h"

void room_init(struct ROOM *room) {
    room->game_instance = malloc(sizeof(struct GAME_INSTANCE));
    room->number_of_players = 0;
    room->player_one = NULL;
    room->player_two = NULL;
    room->game_instance->game_state = STATE_NO_GAME;
}

void room_free_memory(struct ROOM *room) {
    printf("Freeing room memory\n");
    free(room->game_instance);
}

int room_get_player_one_fd(struct ROOM *room) {
    if (room == NULL)
        return -1;
    if (room->player_one != NULL)
        return room->player_one->file_descriptor;
    else
        return -1;
}

int room_get_player_two_fd(struct ROOM *room) {
    if (room == NULL)
        return -1;
    if (room->player_one != NULL)
        return room->player_two->file_descriptor;
    else
        return -1;
}

int room_get_other_player_fd(struct ROOM *room, int current_player_fd) {
    if (room == NULL)
        return -1;
    if (current_player_fd == -1)
        return -1;
    if (room->player_one != NULL)
        if (room->player_one->file_descriptor != current_player_fd)
            return room->player_one->file_descriptor;
    if (room->player_two != NULL)
        if (room->player_two->file_descriptor != current_player_fd)
            return room->player_two->file_descriptor;
    return -1;
}

//void room_start_game(struct ROOM *room) {
//    room->game_instance = malloc(sizeof(struct GAME_INSTANCE));
//}
