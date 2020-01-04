#ifndef CHECKERSSERVER_ROOM_H
#define CHECKERSSERVER_ROOM_H

#include <pthread.h>
#include "game.h"

struct ROOM {
    struct PLAYER *player_one;
    struct PLAYER *player_two;
    struct GAME_INSTANCE *game_instance;
    pthread_mutex_t room_lock;
};

struct ROOM *room_create_new(struct PLAYER *player_one, struct PLAYER *player_two);

void room_delete(struct ROOM *room);

#endif //CHECKERSSERVER_ROOM_H
