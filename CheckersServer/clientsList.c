#include <malloc.h>
#include "clientsList.h"
#include "circuralBuffer.h"

#define CLIENTS_LIST_INITIAL_SIZE 10
#define CIRC_BUFFER_SIZE 128


void client_list_init(struct CLIENTS_LIST *list) {
    list->current_clients_array_size = CLIENTS_LIST_INITIAL_SIZE;
    list->clients_fd = malloc(CLIENTS_LIST_INITIAL_SIZE * sizeof(struct pollfd)); // TODO: Check for error
    list->number_of_clients = 0;
//    list->marked_for_delete = malloc(CLIENTS_LIST_INITIAL_SIZE * sizeof(bool));
//    list->circular_buffers = malloc(CLIENTS_LIST_INITIAL_SIZE * sizeof(struct CIRC_BUFFER));
}

int resize_clients_array(struct CLIENTS_LIST *list) {
    printf("Allocating more memory for the clients list\n");
    // If the list is too small, allocate twice as much memory
    unsigned new_size = 2 * list->current_clients_array_size;
    list->clients_fd = realloc(list->clients_fd, new_size * sizeof(struct pollfd));
//    list->marked_for_delete = realloc(list->marked_for_delete, new_size * sizeof(bool));
//    list->circular_buffers = realloc(list->circular_buffers, new_size * sizeof(struct CIRC_BUFFER));
    if (list->clients_fd == NULL) {
        printf("Error while allocating memory\n");
        return -1;
    }
    list->current_clients_array_size = new_size;
    return 0;
}

int client_list_add(struct CLIENTS_LIST *list, int client_fd) {
    if (list->number_of_clients >= list->current_clients_array_size) {
        resize_clients_array(list);
    }
    list->clients_fd[list->number_of_clients].fd = client_fd;
    list->clients_fd[list->number_of_clients].events = POLLIN;
//    list->marked_for_delete[list->number_of_clients] = false;
    list->number_of_clients++;
//    circ_buffer_init(&list->circular_buffers[list->number_of_clients], 128);
    return list->number_of_clients - 1;
}

void client_list_delete(struct CLIENTS_LIST *list, unsigned index) {
    // Put the last client in the place of the one we're deleting
    list->clients_fd[index] = list->clients_fd[list->number_of_clients - 1];
//    list->circular_buffers[index] = list->circular_buffers[list->number_of_clients - 1];
    list->number_of_clients--;
}

void client_list_delete_by_fd(struct CLIENTS_LIST *list, int fd) {
    int index = client_list_find_by_fd(list, fd);
    client_list_delete(list, (unsigned)index);
}

int client_list_find_by_fd(struct CLIENTS_LIST *list, int fd) {
    for (unsigned i = 0; i < list->number_of_clients; ++i) {
        if (list->clients_fd[i].fd == fd) {
            return i;
        }
    }
    return -1;
}

void client_list_free_memory(struct CLIENTS_LIST *list) {
//    for (unsigned i = 0; i < list->number_of_clients; ++i) {
//        circ_buffer_free_memory(&list->circular_buffers[i]);
//    }
    free(list->clients_fd);
}
