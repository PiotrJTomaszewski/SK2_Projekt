#include <malloc.h>
#include <unistd.h>
#include "player.h"

#define PLAYER_BUFFER_SIZE 64

struct PLAYER *player_create_new(int player_fd) {
    struct PLAYER *player = malloc(sizeof(struct PLAYER));
    if (player != NULL) {
        // Assign initial values to the player's fields
        struct CIRC_BUFFER *buffer = circ_buffer_create_new(PLAYER_BUFFER_SIZE);
        if (buffer != NULL) {
            player->file_descriptor = player_fd;
            player->player_color = COLOR_NO_COLOR;
            player->buffer = buffer;
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
