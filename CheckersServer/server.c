#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "server.h"
#include "room.h"
#include "messages.h"
#include "serverClientCommunication.h"
#include "serverGame.h"

#define BACKLOG_SIZE 1024

volatile int run_flag = true;

struct ROOM_THREAD_DATA {
    struct PLAYER *player_one;
    struct PLAYER *player_two;
};


void interrupt_signal_handler() {
    printf("CTRL+C interrupt\n");
    run_flag = false;
}


void *_room_thread(void *room_thread_data_param) {
    pthread_detach(pthread_self());
    // Retrieve players data
    struct ROOM_THREAD_DATA *room_thread_data = (struct ROOM_THREAD_DATA *) (room_thread_data_param);
    struct PLAYER *players[2];
    players[0] = room_thread_data->player_one;
    players[1] = room_thread_data->player_two;
    free(room_thread_data); // We won't need that anymore

    // Create a room for them
    struct ROOM *room = room_create_new(players[0], players[1]);
    if (room == NULL) { // Terminate thread if there was an error while allocating memory
        // Inform players about the error
        ser_cli_com_send_message(players[0], SCMSG_CRITICAL_ERROR, 0, 0);
        ser_cli_com_send_message(players[1], SCMSG_CRITICAL_ERROR, 0, 0);
        printf("Critical error. Game room thread terminating\n");
        room_delete(room);
        player_delete(players[0]);
        player_delete(players[1]);
        return 0;
    }

    // Start a game and inform players about that fact
    server_game_start_game(room);

    // Initialize polling
    struct pollfd poll_clients[2];
    poll_clients[0].fd = players[0]->file_descriptor;
    poll_clients[0].events = POLLIN;
    poll_clients[1].fd = players[1]->file_descriptor;
    poll_clients[1].events = POLLIN;

    bool run_flag = true;
    while (run_flag) { // Main loop
        int poll_count = poll(poll_clients, 2, 1000);
        if (poll_count < 0) {
            perror("Poll error. Terminating thread");
            run_flag = false;
        } else {
            for (unsigned i = 0; i < 2; ++i) {
                if (poll_clients[i].revents & POLLIN) {
                    // Receive all available bytes from the player and put it into the buffer
                    enum SER_CLI_COM_RESULT result = ser_cli_com_receive(players[i]);
                    struct PARSED_MESSAGE_STRUCT parsed_message;
                    if (result == SER_CLI_COM_NO_ERROR) {
                        // Parse all available messages
                        do {
                            parsed_message = ser_cli_com_parse_next(players[i]);
                            if (parsed_message.error == 0) { // A message was received
                                switch (parsed_message.message_code) {
                                    case SCMSG_MOVE_PIECE:
                                        server_game_move_piece(room, players[i], parsed_message.param1,
                                                               parsed_message.param2);
                                        if (server_game_check_if_the_game_has_ended(room)) { // The game has ended
                                            run_flag = false;
                                        }
                                        break;
                                    case SCMSG_GOODBYE:  // Player wants to disconnect. Inform his opponent
                                        printf("Player %d says goodbye\n", players[i]->file_descriptor);
                                        ser_cli_com_send_message(players[1 - i], SCMSG_OPPONENT_LEFT, 0, 0);
                                        run_flag = false;
                                        break;
                                    default:
                                        break;
                                } // switch
                            } // Check if message was received
                        } while (parsed_message.error == 0);
                    } else if (result == SER_CLI_COM_SOCKET_CLOSED) {  // Player has left the server
                        run_flag = false;
                        // Inform the other player about the fact
                        ser_cli_com_send_message(players[1 - i], SCMSG_OPPONENT_LEFT, 0, 0);
                    }
                }
            }
        }
    }
    printf("Game room thread terminating\n");
    room_delete(room);
    player_delete(players[0]);
    player_delete(players[1]);
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

    struct PLAYER *waiting_player = NULL;
    struct PLAYER *player = NULL;
    int client_fd;

    // Main server loop
    while (run_flag) {
        client_fd = accept(server_socket_fd, NULL, NULL);
        if (client_fd > 0) {
            player = player_create_new(client_fd);
            // Send the player a nice welcoming message
            printf("Welcoming a new player %d\n", client_fd);
            ser_cli_com_send_message(player, SCMSG_WELCOME, 0, 0);
            // If there is no player waiting for a room, make this player wait
            if (waiting_player == NULL) {
                printf("He'll have to wait though\n");
                waiting_player = player;
                // Inform the player that he is waiting for an opponent
                ser_cli_com_send_message(waiting_player, SCMSG_WAITING_FOR_OPPONENT, 0, 0);
            } else { // If there is another player waiting for an opponent
                printf("I've found a friend for him\n");
                pthread_t room_thread;
                struct ROOM_THREAD_DATA *room_thread_data = malloc(sizeof(struct ROOM_THREAD_DATA));
                room_thread_data->player_one = waiting_player;
                room_thread_data->player_two = player;
                waiting_player = NULL;
                pthread_create(&room_thread, NULL, _room_thread, room_thread_data);
            } // If there is a player waiting
        } // Accept check
    } // Main loop
    printf("Closing server\n");
    close(server_socket_fd);
}

