#ifndef CHECKERSSERVER_PLAYERS_LIST_H
#define CHECKERSSERVER_PLAYERS_LIST_H
#include "player.h"

struct PLAYERS_LIST {
    struct PLAYER **players;
    unsigned number_of_players;
    unsigned current_players_array_size;
};

void player_list_init(struct PLAYERS_LIST *list);

struct PLAYER *player_list_add(struct PLAYERS_LIST *list, int player_fd);

struct ROOM *player_list_get_free_room(struct PLAYERS_LIST *list);

struct PLAYER *player_get_by_fd(struct PLAYERS_LIST *list, int fd);
#endif //CHECKERSSERVER_PLAYERS_LIST_H
