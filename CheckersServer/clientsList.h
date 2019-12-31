#ifndef CHECKERSSERVER_CLIENTSLIST_H
#define CHECKERSSERVER_CLIENTSLIST_H

#include <poll.h>

struct CLIENTS_LIST {
    struct pollfd *clients_fd;
    struct CIRC_BUFFER *circular_buffers;
    unsigned number_of_clients;
    unsigned current_clients_array_size;
};

void client_list_init(struct CLIENTS_LIST *list);

int client_list_add(struct CLIENTS_LIST *list, int client_fd);

void client_list_delete(struct CLIENTS_LIST *list, unsigned index);

void client_list_delete_by_fd(struct CLIENTS_LIST *list, int fd);

int client_list_find_by_fd(struct CLIENTS_LIST *list, int fd);

void client_list_free_memory(struct CLIENTS_LIST *list);

#endif //CHECKERSSERVER_CLIENTSLIST_H
