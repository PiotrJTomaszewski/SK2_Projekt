#ifndef CHECKERSSERVER_ROOM_H
#define CHECKERSSERVER_ROOM_H

#include "game.h"
struct ROOM {
    int number_of_players;
    int player_one_fd;
    int player_two_fd;
    enum GAME_PIECE_COLOR player_one_color;
    enum GAME_PIECE_COLOR player_two_color;
    struct GAME_INSTANCE *game_instance;
};
#endif //CHECKERSSERVER_ROOM_H
