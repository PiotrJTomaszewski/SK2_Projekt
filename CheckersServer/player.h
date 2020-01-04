#ifndef CHECKERSSERVER_CLIENT_H
#define CHECKERSSERVER_CLIENT_H
#include <pthread.h>
#include "circuralBuffer.h"
#include "room.h"

/**
 * A structure representing a player
 */
struct PLAYER {
    int file_descriptor;
    enum GAME_PIECE_COLOR player_color;
    struct CIRC_BUFFER *buffer;
//    pthread_mutex_t fd_lock;
};

/**
 * Allocates memory and creates a new instance of the player.
 * @param player_fd A file descriptor of the player.
 * @return A new player instance or NULL on error.
 */
struct PLAYER *player_create_new(int player_fd);

/**
 * Closes player's file descriptor and frees the allocated memory.
 * @param player
 */
void player_delete(struct PLAYER *player);

#endif //CHECKERSSERVER_CLIENT_H
