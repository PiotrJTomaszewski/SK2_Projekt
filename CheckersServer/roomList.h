#ifndef CHECKERSSERVER_ROOMLIST_H
#define CHECKERSSERVER_ROOMLIST_H
#include "room.h"

struct ROOMS_LIST {
    struct ROOM *rooms;
    unsigned number_of_rooms;
    unsigned rooms_size;
};

void rooms_list_init(struct ROOMS_LIST *list);

int rooms_list_add(struct ROOMS_LIST *list, int player_one_fd);

void rooms_list_delete(struct ROOMS_LIST *list, unsigned index);

int rooms_list_find_free_room(struct ROOMS_LIST *list);

#endif //CHECKERSSERVER_ROOMLIST_H
