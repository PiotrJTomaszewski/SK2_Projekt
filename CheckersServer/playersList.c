#include "playersList.h"
#include <malloc.h>
#include <poll.h>

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
    player_init(player);
    player->file_descriptor = player_fd;
    list->players[list->number_of_players] = player;
    list->number_of_players++;
    return player;
}

void player_list_delete_by_fd(struct PLAYERS_LIST *list, int player_fd) {
    int id = player_get_index_by_fd(list, player_fd);
    // Mark that fact in the room info also
    player_leave_room(list->players[id]);
    player_free_memory(list->players[id]);
    list->players[id] = list->players[list->number_of_players - 1];
    list->number_of_players--;
}

struct ROOM *player_list_get_free_room(struct PLAYERS_LIST *list) {
    for (unsigned i = 0; i < list->number_of_players; ++i) {
        if (player_is_in_free_room(list->players[i])) {
            return list->players[i]->room;
        }
    }
    return NULL;
}

struct PLAYER *player_list_get_by_fd(struct PLAYERS_LIST *list, int fd) {
    for (unsigned i = 0; i < list->number_of_players; ++i) {
        if (list->players[i]->file_descriptor == fd) {
            return list->players[i];
        }
    }
    return NULL;
}

int player_get_index_by_fd(struct PLAYERS_LIST *list, int fd) {
    for (unsigned i = 0; i < list->number_of_players; ++i) {
        if (list->players[i]->file_descriptor == fd) {
            return i;
        }
    }
    return -1;
}

struct pollfd *player_list_get_pollfds(struct PLAYERS_LIST *list, int server_fd, unsigned *number_of_clients) {
    struct pollfd *pollfds = malloc((list->number_of_players + 1) * sizeof(struct pollfd));
    for (unsigned i = 0; i < list->number_of_players; ++i) {
        pollfds[i].fd = list->players[i]->file_descriptor;
        pollfds[i].events = POLLIN;
    }
    pollfds[list->number_of_players].fd = server_fd;
    pollfds[list->number_of_players].events = POLLIN;
    *number_of_clients = list->number_of_players + 1;
    return pollfds;
}
