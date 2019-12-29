#include "playersList.h"
#include <malloc.h>

#define PLAYERS_LIST_INITIAL_SIZE 10


void player_list_init(struct PLAYERS_LIST *list) {
    list->current_players_array_size = PLAYERS_LIST_INITIAL_SIZE;
    list->players = malloc(PLAYERS_LIST_INITIAL_SIZE * sizeof(struct PLAYER *)); // TODO: Check for error
    list->number_of_players = 0;
}

int resize_player_array(struct PLAYERS_LIST *list) {
    printf("Allocating more memory for the clients list\n");
    // If the list is too small, allocate twice as much memory
    unsigned new_size = 2 * list->current_players_array_size;
    list->players = realloc(list->players, new_size * sizeof(struct PLAYER *));
    if (list->players == NULL) {
        printf("Error while allocating memory\n");
        return -1;
    }
    list->current_players_array_size = new_size;
    return 0;
}

struct PLAYER *player_list_add(struct PLAYERS_LIST *list, int player_fd) {
    if (list->number_of_players >= list->current_players_array_size) {
        resize_player_array(list);
    }
    // Allocate memory for the new client
    struct PLAYER *player = malloc(sizeof(struct PLAYER));
    player->file_descriptor = player_fd;
    list->players[list->number_of_players] = player;
    list->number_of_players++;
    return player;
}

struct ROOM *player_list_get_free_room(struct PLAYERS_LIST *list) {
    for (unsigned i = 0; i < list->number_of_players; ++i) {
        if (player_is_in_free_room(list->players[i])) {
            return list->players[i]->room;
        }
    }
    return NULL;
}
