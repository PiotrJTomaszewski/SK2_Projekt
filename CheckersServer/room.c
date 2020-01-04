#include <malloc.h>
#include "room.h"
#include "player.h"

//struct ROOM *room_create_new() {
//    struct ROOM *room = malloc(sizeof(struct ROOM));
//    if (room != NULL) {
//        room->game_instance = malloc(sizeof(struct GAME_INSTANCE));
//        if (room->game_instance != NULL) {
//            room->number_of_players = 0;
//            room->player_one = NULL;
//            room->player_two = NULL;
//            room->game_instance->game_state = STATE_NO_GAME;
//            if (pthread_mutex_init(&room->room_lock, NULL) != 0)
//            {
//                perror("Error init room mutex");
//                return NULL;
//            }
//        } else {
//            perror("Error while allocating memory for the room");
//            free(room);
//            return NULL;
//        }
//    } else {
//        perror("Error while allocating memory for the room");
//    }
//    return room;
//}

struct ROOM *room_create_new(struct PLAYER *player_one, struct PLAYER *player_two) {
    struct ROOM *room = malloc(sizeof(struct ROOM));
    if (room != NULL) {
        room->game_instance = malloc(sizeof(struct GAME_INSTANCE));
        if (room->game_instance != NULL) {
            room->number_of_players = 2;
            room->player_one = player_one;
            room->player_two = player_two;
            room->game_instance->game_state = STATE_NO_GAME;
            if (pthread_mutex_init(&room->room_lock, NULL) != 0)
            {
                free(room->game_instance);
                free(room);
                perror("Error init room mutex");
                return NULL;
            }
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

int room_assign_player(struct ROOM *room, struct PLAYER *player) {
    int was_player_assigned = -1;
    pthread_mutex_lock(&room->room_lock);
    if (room->player_one == NULL) {
        room->player_one = player;
        room->number_of_players++;
        was_player_assigned = 0;
    } else if (room->player_two == NULL) {
        room->player_two = player;
        room->number_of_players++;
        was_player_assigned = 0;
    }
    pthread_mutex_unlock(&room->room_lock);
    return was_player_assigned;
}

struct PLAYER *room_get_other_player(struct ROOM *room, struct PLAYER *player) {
    if (room == NULL)
        return NULL;
    if (player == room->player_one)
        return room->player_two;
    return room->player_two;
}
