#ifndef CHECKERSSERVER_ROOM_H
#define CHECKERSSERVER_ROOM_H

#include "game.h"
struct ROOM {
    int number_of_players;
    struct PLAYER *player_one;
    struct PLAYER *player_two;
    struct GAME_INSTANCE *game_instance;
};

void room_init(struct ROOM *room);

void room_free_memory(struct ROOM *room);

//void room_start_game(struct ROOM *room);
#endif //CHECKERSSERVER_ROOM_H
