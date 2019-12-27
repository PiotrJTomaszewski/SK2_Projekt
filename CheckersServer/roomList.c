#include <malloc.h>
#include "roomList.h"

#define ROOMS_LIST_INITIAL_SIZE 10

void rooms_list_init(struct ROOMS_LIST *list) {
    list->rooms = malloc(ROOMS_LIST_INITIAL_SIZE * sizeof(int));
    list->number_of_rooms = 0;
    list->rooms_size = ROOMS_LIST_INITIAL_SIZE;
}

int rooms_list_add(struct ROOMS_LIST *list, int player_one_fd) {
    if (list->number_of_rooms >= list->rooms_size) {
        printf("Allocating more memory for the rooms list\n");
        // If the list is too small, allocate twice as much memory
        unsigned new_size = 2 * list->rooms_size;
        list->rooms = realloc(list->rooms, new_size * sizeof(int));
        if (list->rooms == NULL) {
            printf("Error while allocating memory\n");
        }
    }
    list->rooms[list->number_of_rooms].player_one_fd = player_one_fd;
    list->rooms[list->number_of_rooms].number_of_players = 1;
    list->number_of_rooms++;
    return list->number_of_rooms - 1;
}

void rooms_list_delete(struct ROOMS_LIST *list, unsigned index) {
    // Put the last room in the place of the one we're deleting
    free(list->rooms[index].game_instance);
    list->rooms[index] = list->rooms[list->number_of_rooms - 1];
    list->number_of_rooms--;
}

int rooms_list_find_free_room(struct ROOMS_LIST *list) {
    for (unsigned i = 0; i < list->number_of_rooms; ++i) {
        if (list->rooms[i].number_of_players == 1) {
            return i;
        }
    }
    // No free room found
    return -1;
}
