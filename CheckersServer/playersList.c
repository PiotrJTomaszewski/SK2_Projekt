#include "playersList.h"
#include <malloc.h>
#include <poll.h>

#define PLAYERS_LIST_INITIAL_SIZE 10


void players_list_init(struct PLAYERS_LIST *list) {
    list->current_players_array_size = PLAYERS_LIST_INITIAL_SIZE;
    list->players = malloc(PLAYERS_LIST_INITIAL_SIZE * sizeof(struct PLAYER *));
    if (list->players == NULL) {
        perror("Critical error while creating players list");
    }
    list->number_of_players = 0;
    if (pthread_mutex_init(&list->players_list_lock, NULL) != 0)
    {
        perror("Error init players list mutex");
    }
}

/**
 * Resize the players list, doubling it's capacity
 * @param list A pointer to the players list structure
 * @return 0 on success, -1 on error
 */
int _players_list_resize(struct PLAYERS_LIST *list) {
    printf("Allocating more memory for the clients list\n");
    // Double the allocated memory
    unsigned new_size = 2 * list->current_players_array_size;
    list->players = realloc(list->players, new_size * sizeof(struct PLAYER *));
    if (list->players != NULL) {
        list->current_players_array_size = new_size;
        return 0;
    } else {
        perror("Critical error while expanding players list");
        return -1;
    }
}

struct PLAYER *players_list_add_new(struct PLAYERS_LIST *list, int player_fd) {
    // Create a new player instance
    struct PLAYER *player = player_create_new(player_fd);
    if (player != NULL) {
        // Add the new player to the list
        pthread_mutex_lock(&list->players_list_lock);
        // Check if there is free space to put the client. If not resize the array
        if (list->number_of_players >= list->current_players_array_size) {
            _players_list_resize(list);
        }
        list->players[list->number_of_players] = player;
        list->number_of_players++;
        pthread_mutex_unlock(&list->players_list_lock);
    } else {
        perror("Error while allocating memory for the new player");
    }
    return player;
}

struct ROOM *players_list_get_free_room(struct PLAYERS_LIST *list) {
    struct ROOM *free_room = NULL;
//    pthread_mutex_lock(&list->players_list_lock); // TODO: For now this conflicts with server_game_join_room
    for (unsigned i = 0; i < list->number_of_players; ++i) {
        if (player_is_in_free_room(list->players[i])) {
            free_room = list->players[i]->room;
            break;
        }
    }
//    pthread_mutex_unlock(&list->players_list_lock);
    return free_room;
}

struct PLAYER *players_list_get_by_fd(struct PLAYERS_LIST *list, int fd) {
    struct PLAYER *player = NULL;
    pthread_mutex_lock(&list->players_list_lock);
    for (unsigned i = 0; i < list->number_of_players; ++i) {
        if (list->players[i]->file_descriptor == fd) {
            player = list->players[i];
            break;
        }
    }
    pthread_mutex_unlock(&list->players_list_lock);
    return player;
}

int _players_list_get_player_index(struct PLAYERS_LIST *list, struct PLAYER *player) {
    for (unsigned i = 0; i < list->number_of_players; ++i) {
        if (list->players[i] == player) {
            return i;
        }
    }
    return -1;
}

void players_list_remove_player(struct PLAYERS_LIST *list, struct PLAYER *player) {
    pthread_mutex_lock(&list->players_list_lock);
    int id = _players_list_get_player_index(list, player);
    player_leave_room(list->players[id]);
    player_delete(player);
    list->players[id] = list->players[list->number_of_players - 1];
    list->number_of_players--;
    pthread_mutex_unlock(&list->players_list_lock);
}

struct pollfd *players_list_get_pollfds(struct PLAYERS_LIST *list, int server_fd, unsigned *number_of_clients) {
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
