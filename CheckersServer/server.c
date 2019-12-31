#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/poll.h>
#include <unistd.h>
#include <tkPort.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "server.h"
#include "clientsList.h"
#include "room.h"
#include "messages.h"
#include "playersList.h"
#include "serverClientCommunication.h"
#include "serverGame.h"

#define BACKLOG_SIZE 1024

pthread_mutex_t players_list_mutex;
volatile int run_flag = true;

//struct CLIENTS_LIST clients_list;
struct PLAYERS_LIST players_list;

struct CLIENT_THREAD_DATA {
    int client_fd;
};

//void interrupt_signal_handler(int a) {
//    printf("CTRL+C interrupt\n");
//    run_flag = false;
//}

struct ROOM *join_room(struct PLAYER *player) {
    struct ROOM *room = player_list_get_free_room(&players_list);
    if (room == NULL) {  // There is no free room
        // Create new room
        room = malloc(sizeof(struct ROOM));
        room_init(room);
        printf("New room created\n");
        ser_cli_com_send_message(player->file_descriptor, SCMSG_WAITING_FOR_OPPONENT, 0, 0);
    }
    player_assign_room(player, room);
    return room;
}


void *_handle_new_connection(void *client_thread_data_param) {
    pthread_detach(pthread_self());
    struct CLIENT_THREAD_DATA *player_data = (struct CLIENT_THREAD_DATA *) (client_thread_data_param);
    printf("New client connected\n");
    int player_fd = player_data->client_fd;
    printf("Client fd %d\n", player_fd);
    ser_cli_com_send_message(player_fd, SCMSG_WELCOME, 0, 0);

    struct PLAYER *player = player_list_add(&players_list, player_fd);
    struct ROOM *room = join_room(player);
    if (room->number_of_players == 2)
        server_game_start_game(room);
    free(player_data);
    return 0;
}

void *_handle_existing_connection(void *client_thread_data_param) {
    pthread_detach(pthread_self());
    struct CLIENT_THREAD_DATA *player_data = (struct CLIENT_THREAD_DATA *) (client_thread_data_param);
    printf("DEBUG: Existing connection\n");
    int player_fd = player_data->client_fd;
    struct PLAYER *player = player_list_get_by_fd(&players_list, player_fd);
    if (player == NULL) {
        printf("Tried to handle dead client\n");
        return 0;
    }
    enum SER_CLI_COM_RESULT result = ser_cli_com_get_and_parse(player);
    if (result == SER_CLI_COM_SOCKET_CLOSED) {
        printf("Player exited\n");
        // If the player disconnected from server
        int other_player_fd = room_get_other_player_fd(player->room, player->file_descriptor);
        if (player->room != NULL) {
            // If player is in game inform the other player about the fact
            if (player->room->number_of_players == 2) {
                if (other_player_fd != -1)
                    ser_cli_com_send_message(other_player_fd, SCMSG_OPPONENT_LEFT, 0, 0);
            }
            player_list_delete_by_fd(&players_list, player_fd);
            if (other_player_fd != -1)
                player_list_delete_by_fd(&players_list, other_player_fd);
        }

    }
    free(player_data);
    return 0;
}

int _setup_socket(int port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Error while creating socket");
    }
    int nFoo = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &nFoo, sizeof(nFoo)) < 0) {
        perror("Error setsockopt.");
    }
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons((uint16_t) port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Error bind.");
    }
    if (listen(socket_fd, BACKLOG_SIZE) < 0) {
        perror("Error listen.");
    }
    return socket_fd;
}

void server_run(int port) {
    int server_socket_fd = _setup_socket(port);
    printf("DEBUG: Server starting\n");
//    signal(SIGINT, interrupt_signal_handler);

    srand((unsigned) time(NULL));
    player_list_init(&players_list);
    // Add listener to clients list
    while (run_flag) {
        unsigned number_of_clients = 0;
        struct pollfd *clients = player_list_get_pollfds(&players_list, server_socket_fd, &number_of_clients);
        int poll_count = poll(clients, number_of_clients, 1000);
        if (poll_count < 0) {
            perror("Poll error.");
            run_flag = 0;
            continue;
        }
        // Check if any client is ready
        for (unsigned i = 0; i < number_of_clients; ++i) {
            if (clients[i].revents & POLLIN) {
                pthread_t thread;
                struct CLIENT_THREAD_DATA *thread_data = malloc(sizeof(struct CLIENT_THREAD_DATA));
                if (clients[i].fd == server_socket_fd) {
                    // Handle a new connection
                    int client_fd = accept(server_socket_fd, NULL, NULL);
                    thread_data->client_fd = client_fd;
                    // Add client to the list
                    pthread_create(&thread, NULL, _handle_new_connection, thread_data);
                } else {
                    // Handle existing connection
                    thread_data->client_fd = clients[i].fd;
                    pthread_create(&thread, NULL, _handle_existing_connection, thread_data);
                } // if (clients_list.clients_fd[i].fd == server_socket_fd)
            } // if (clients_list.clients_fd[i].revents & POLLIN)
        } // Poll loop
        free(clients);
    } // Main loop
    printf("End\n");
}

