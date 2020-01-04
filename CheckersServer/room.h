#ifndef CHECKERSSERVER_ROOM_H
#define CHECKERSSERVER_ROOM_H

#include <pthread.h>
#include "game.h"

/**
 * A structure representing a game room
 */
struct ROOM {
    struct PLAYER *player_one;
    struct PLAYER *player_two;
    struct GAME_INSTANCE *game_instance;
//    pthread_mutex_t room_lock;
};

/**
 * Allocates the memory and creates a new room instance.
 * @param player_one A pointer to one of the players that will be in that room
 * @param player_two A pointer to the other player
 * @return A new instance of room or NULL on error
 */
struct ROOM *room_create_new(struct PLAYER *player_one, struct PLAYER *player_two);

/**
 * Deletes the room instance, freeing memory
 * @param room A pointer to the room that should be deleted
 */
void room_delete(struct ROOM *room);

#endif //CHECKERSSERVER_ROOM_H
