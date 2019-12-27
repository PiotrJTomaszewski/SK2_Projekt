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
#include "server.h"
#include "clientsList.h"
#include "roomList.h"
#include "room.h"
#include "game.h"
#include "messages.h"

#define BACKLOG_SIZE 1024

struct CLIENTS_LIST clients_list;
struct ROOMS_LIST rooms_list;

struct CLIENT_THREAD_DATA {
    int client_fd;
};


//void *_handle_connection(void *client_thread_data_param) {
//    pthread_detach(pthread_self());
//    struct CLIENT_THREAD_DATA *client_data = (struct CLIENT_THREAD_DATA *) (client_thread_data_param);
//    printf("DEBUG: New client connected\n");
//    printf("Welcome %d\n", ntohl(client_data->client_fd));
//    free(client_data);
//    return 0;
//}

void send_message(int client_fd, char *message) {
    // TODO: Make sure the whole message was sent
    size_t message_length = strlen(message);
//    size_t sent_bytes = 0;
    ssize_t n;
//    while (sent_bytes < message_length) {
    n = send(client_fd, message, message_length, 0);
//        sent_bytes += n; // TODO: What if n == 0 or n < 0
//    }
//    printf("Sent %d\n", (int)sent_bytes);
}

void start_game(int room_id) {
    struct ROOM *room = &rooms_list.rooms[room_id];
    room->game_instance = malloc(sizeof(struct GAME_INSTANCE));
    place_pieces(room->game_instance);
    // TODO: Move that
    room->game_instance->game_state = STATE_LIGHT_TURN;
    // TODO: Randomize colors
    room->player_one_color = COLOR_LIGHT;
    room->player_two_color = COLOR_DARK;
    send_message(room->player_one_fd, MSG_YOU_PLAY_LIGHT);
    send_message(room->player_two_fd, MSG_YOU_PLAY_DARK);
}

void *_handle_new_connection(void *client_thread_data_param) {
    pthread_detach(pthread_self());
    struct CLIENT_THREAD_DATA *client_data = (struct CLIENT_THREAD_DATA *) (client_thread_data_param);
    printf("New client connected\n");
    int client_fd = client_data->client_fd;
    printf("Client fd %d\n", client_fd);
    int client_id = client_list_add(&clients_list, client_fd);
    int room_id = rooms_list_find_free_room(&rooms_list);
    if (room_id == -1) { // No free room was found, so create a new one
        room_id = rooms_list_add(&rooms_list, client_fd);
        send_message(client_fd, MSG_WAITING_FOR_OPPONENT);
    } else { // A free room was found
        // Join the room
        rooms_list.rooms[room_id].player_two_fd = client_fd;
        rooms_list.rooms[room_id].number_of_players = 2;
        send_message(client_fd, MSG_ROOM_FOUND);
        start_game(room_id);
    }
    // Mark room id in the client
    clients_list.rooms_id[client_id] = room_id;
    free(client_data);
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
    int run_flag = 1;
    int server_socket_fd = _setup_socket(port);
    printf("DEBUG: Server starting\n");

    client_list_init(&clients_list);
    rooms_list_init(&rooms_list);
    // Add listener to clients list
    client_list_add(&clients_list, server_socket_fd);
    while (run_flag) {
        int poll_count = poll(clients_list.clients_fd, clients_list.number_of_clients, 1000);
        if (poll_count < 0) {
            perror("Poll error.");
            run_flag = 0;
            continue;
        }
        // Check if some client is ready
        for (unsigned i = 0; i < clients_list.number_of_clients; ++i) {
            if (clients_list.clients_fd[i].revents & POLLIN) {
                if (clients_list.clients_fd[i].fd == server_socket_fd) {
                    // Handle a new connection
                    int client_fd = accept(server_socket_fd, NULL, NULL);
                    pthread_t thread;
                    struct CLIENT_THREAD_DATA *thread_data = malloc(sizeof(struct CLIENT_THREAD_DATA));
                    thread_data->client_fd = client_fd;
                    pthread_create(&thread, NULL, _handle_new_connection, thread_data);
//                    client_list_add(&clients_list, client_fd);
                } else {
                    int client_fd = clients_list.clients_fd[i].fd;
                    // Handle existing connection
                    printf("DEBUG: Existing connection\n");
                    char buf[128];
                    ssize_t n = recv(client_fd, buf, sizeof buf, 0);
                    if (n > 0) {
                        printf("%s", buf);
                    } else if (n == 0) {
                        printf("DEBUG: Connection closed by the client\n");
                        close(client_fd);
                    } else {
                        perror("Error recv.");
                    } // if (n > 0)
                } // if (clients_list.clients_fd[i].fd == server_socket_fd)
            } // if (clients_list.clients_fd[i].revents & POLLIN)
        } // Poll loop
    } // Main loop
    close(server_socket_fd);
}
