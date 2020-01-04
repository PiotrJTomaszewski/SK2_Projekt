#ifndef CHECKERSSERVER_PLAYERS_LIST_H
#define CHECKERSSERVER_PLAYERS_LIST_H
#include <pthread.h>
#include "player.h"

/**
 * A structure holding a dynamic list of players.
 */
struct PLAYERS_LIST {
    struct PLAYER **players;
    unsigned number_of_players;
    unsigned current_players_array_size;
    pthread_mutex_t players_list_lock;
};

/**
 * Initializes the dynamic list of players.
 * @param list A pointer to the players list structure.
 * @see PLAYERS_LIST
 */
void players_list_init(struct PLAYERS_LIST *list);

/**
 * Creates a new player instance using the file descriptor and adds this new player to the players list.
 * @param list A pointer to the players list structure.
 * @param player_fd The file descriptor of the client (player).
 */
struct PLAYER *players_list_add_new(struct PLAYERS_LIST *list, int player_fd);

void players_list_delete_by_fd(struct PLAYERS_LIST *list, int player_fd);

void players_list_remove_player(struct PLAYERS_LIST *list, struct PLAYER *player);

/**
 * Returns a pointer to the room that have only one player waiting for an opponent.
 * @param list
 * @return
 */
struct ROOM *players_list_get_free_room(struct PLAYERS_LIST *list);

struct PLAYER *players_list_get_by_fd(struct PLAYERS_LIST *list, int fd);

struct pollfd *players_list_get_pollfds(struct PLAYERS_LIST *list, int server_fd, unsigned *number_of_clients);
#endif //CHECKERSSERVER_PLAYERS_LIST_H

// TODO: Freeing all memory?