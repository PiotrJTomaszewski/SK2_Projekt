#include <malloc.h>
#include <unistd.h>
#include "player.h"

#define PLAYER_BUFFER_SIZE 128

struct PLAYER *player_create_new(int player_fd) {
    struct PLAYER *player = malloc(sizeof(struct PLAYER));
    if (player != NULL) {
        // Assign initial values to the player's fields
        struct CIRC_BUFFER *buffer = circ_buffer_create_new(PLAYER_BUFFER_SIZE);
        if (buffer != NULL) {
            player->file_descriptor = player_fd;
            player->room = NULL;
            player->player_color = COLOR_NO_COLOR;
            player->buffer = buffer;
            if (pthread_mutex_init(&player->fd_lock, NULL) != 0)
            {
                perror("Error init player fd mutex");
            }
        } else {
            printf("Error while creating new player\n");
            return NULL;
        }
    } else {
        perror("Error while allocating memory for the new player\n");
    }
    return player;
}

void player_delete(struct PLAYER *player) {
    printf("Deleting player %d\n", player->file_descriptor);
    close(player->file_descriptor);
    circ_buffer_delete(player->buffer);
    free(player);
}

void player_assign_room(struct PLAYER *player, struct ROOM *room) {
    player->room = room;
    room_assign_player(room, player);
}

void player_leave_room(struct PLAYER *player) {
    // TODO: Change this
    if (player != NULL && player->room != NULL) {
        pthread_mutex_lock(&player->room->room_lock);
        if (player->room->player_one != NULL && player->room->player_one->file_descriptor == player->file_descriptor)
            player->room->player_one = NULL;
        else
            player->room->player_two = NULL;
        player->room->number_of_players--;
        if (player->room->number_of_players == 0) {
            room_delete(player->room);
        }
        pthread_mutex_unlock(&player->room->room_lock);
        player->room = NULL;
    }
}

int player_is_in_free_room(struct PLAYER *player) {
    if (player->room != NULL) {
        return player->room->number_of_players == 1;
    }
    return 0;
}
