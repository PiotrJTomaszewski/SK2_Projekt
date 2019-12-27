#ifndef CHECKERSSERVER_CLIENTSLIST_H
#define CHECKERSSERVER_CLIENTSLIST_H

struct CLIENTS_LIST {
    struct pollfd *clients_fd;
    unsigned number_of_clients;
    unsigned clients_fd_size;
    int *rooms_id; // Id of the room the player (client) is in
};

void client_list_init(struct CLIENTS_LIST *list);

int client_list_add(struct CLIENTS_LIST *list, int client_fd);

void client_list_delete(struct CLIENTS_LIST *list, unsigned index);

int client_list_find_by_fd(struct CLIENTS_LIST *list, int fd);

#endif //CHECKERSSERVER_CLIENTSLIST_H
