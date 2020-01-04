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
#include "room.h"
#include "messages.h"
#include "playersList.h"
#include "serverClientCommunication.h"
#include "serverGame.h"

#define BACKLOG_SIZE 1024

pthread_mutex_t players_list_mutex;
volatile int run_flag = true;


struct PLAYERS_LIST players_list;


struct CLIENT_THREAD_DATA {
    int client_fd;
};


void interrupt_signal_handler() {
    printf("CTRL+C interrupt\n");
    run_flag = false;
}


void *_handle_new_connection(void *client_thread_data_param) {
    pthread_detach(pthread_self());

    // Retrieve client file descriptor
    struct CLIENT_THREAD_DATA *player_data = (struct CLIENT_THREAD_DATA *) (client_thread_data_param);
    int player_fd = player_data->client_fd;

    printf("New client connected\n");
    printf("Client fd %d\n", player_fd);

    // Add the new player to the list of players
    struct PLAYER *player = players_list_add_new(&players_list, player_fd);
    // Send the welcome message to the player
    ser_cli_com_send_message(player, SCMSG_WELCOME, 0, 0);
    // Find a game room for the player or create a new one
    server_game_join_room(player, &players_list);
    free(player_data);
    return 0;
}

void *_handle_existing_connection(void *client_thread_data_param) {
    pthread_detach(pthread_self());
//    printf("DEBUG: Existing connection\n");

    // Retrieve client file descriptor
    struct CLIENT_THREAD_DATA *player_data = (struct CLIENT_THREAD_DATA *) (client_thread_data_param);
    int player_fd = player_data->client_fd;
    // Find the player using file descriptor
    struct PLAYER *player = players_list_get_by_fd(&players_list, player_fd);
    if (player == NULL) {
        printf("Tried to handle dead client\n");
        return 0;
    }
    // Handle messages sent by the player (if there were any)
    enum SER_CLI_COM_RESULT result = ser_cli_com_get_and_parse(player);
    if (result == SER_CLI_COM_SOCKET_CLOSED) {
        printf("Player exited\n");
        // Yhe player disconnected from server
        struct PLAYER *other_player = room_get_other_player(player->room, player);
        if (player->room != NULL) {
            // If player is in game inform the other player about the disconnection
            if (player->room->number_of_players == 2) {
                if (other_player != NULL)
                    ser_cli_com_send_message(other_player, SCMSG_OPPONENT_LEFT, 0, 0);
            }
            // TODO: Change this
            players_list_remove_player(&players_list, player);
            if (other_player != NULL)
                players_list_remove_player(&players_list, other_player);
        }

    }
    free(player_data);
    return 0;
}

/**
 * Setups the server.
 * @param port A port on which the server should listen.
 * @return socket_fd A file descriptor of the server or -1 if any error occurred.
 */
int _setup_socket(int port) {
    bool error_occurred = false;
    int socket_fd;
    int nFoo = 1;
    struct sockaddr_in server_address;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) >= 0) {
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &nFoo, sizeof(nFoo)) == 0) {
            memset(&server_address, 0, sizeof(server_address));
            server_address.sin_family = AF_INET;
            server_address.sin_port = htons((uint16_t) port);
            server_address.sin_addr.s_addr = htonl(INADDR_ANY);
            if (bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == 0) {
                if (listen(socket_fd, BACKLOG_SIZE) < 0) {
                    perror("Server setup error while enabling accepting connections");
                    error_occurred = true;
                }
            } else { // bind
                perror("Server setup error while assigning address");
                error_occurred = true;
            }
        } else { // setsockopt
            perror("Server setup error while making the socket able to reuse address");
            error_occurred = true;
        }
    } else {  // socket
        perror("Server setup error while creating the socket");
        error_occurred = true;
    }
    if (error_occurred)
        return -1;
    else
        return socket_fd;
}

/**
 * Initializes the game server and starts the server loop.
 * @param port A port on which the server should listen.
 */
void server_run(int port) {
    // Initialize the server
    int server_socket_fd = _setup_socket(port);
    if (server_socket_fd < 0) {
        printf("An error occurred while setting up the server!\nThe program will now terminate.\n");
        exit(-1);
    }
    printf("Server starting\n");

    // Add a way to stop the server loop
    signal(SIGINT, interrupt_signal_handler);
    // For some reason SIGINT doesn't work for stop button in CLion IDE but it reacts to SIGTERM
    signal(SIGTERM, interrupt_signal_handler);

    // Initialize players list
    players_list_init(&players_list);

    // Main server loop
    while (run_flag) {
        // Get the file descriptors of all the clients connected to the server. Also add the server socket fd to this array.
        unsigned number_of_clients = 0;
        struct pollfd *clients = players_list_get_pollfds(&players_list, server_socket_fd, &number_of_clients);

        int poll_count = poll(clients, number_of_clients, 1000);
        if (poll_count < 0) {
            perror("Poll error.");
            run_flag = 0;
        } else {
            // Check if any client is ready
            for (unsigned i = 0; i < number_of_clients; ++i) {
                if (clients[i].revents & POLLIN) {
                    pthread_t thread;
                    struct CLIENT_THREAD_DATA *thread_data = malloc(sizeof(struct CLIENT_THREAD_DATA));
                    if (clients[i].fd == server_socket_fd) {
                        // Handle a new connection
                        int client_fd;
                        if ((client_fd = accept(server_socket_fd, NULL, NULL)) >= 0) {
                            thread_data->client_fd = client_fd;
                            pthread_create(&thread, NULL, _handle_new_connection, thread_data);
                        } else { // accept
                            perror("Error while accepting client connection!");
                            printf("Moving on\n");
                        }
                    } else {
                        // Handle existing connection
                        thread_data->client_fd = clients[i].fd;
                        pthread_create(&thread, NULL, _handle_existing_connection, thread_data);
                    } // New or existing connection
                } // Poll event check
            } // Clients poll loop
        } // poll_count check
        free(clients);
    } // Main loop
    printf("Closing server\n");
    close(server_socket_fd);
    // TODO: Close all file descriptors
}

