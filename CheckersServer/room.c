#include <malloc.h>
#include "room.h"
#include "player.h"

struct ROOM *room_create_new(struct PLAYER *player_one, struct PLAYER *player_two) {
    struct ROOM *room = malloc(sizeof(struct ROOM));
    if (room != NULL) {
        room->game_instance = malloc(sizeof(struct GAME_INSTANCE));
        if (room->game_instance != NULL) {
            room->player_one = player_one;
            room->player_two = player_two;
            room->game_instance->game_state = STATE_NO_GAME;
//            if (pthread_mutex_init(&room->room_lock, NULL) != 0)
//            {
//                free(room->game_instance);
//                free(room);
//                perror("Error init room mutex");
//                return NULL;
//            }
        } else {
            perror("Error while allocating memory for the room");
            free(room);
            return NULL;
        }
    } else {
        perror("Error while allocating memory for the room");
    }
    return room;
}


void room_delete(struct ROOM *room) {
    free(room->game_instance);
    free(room);
}
