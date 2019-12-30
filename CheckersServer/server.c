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
#include "server.h"
#include "clientsList.h"
#include "room.h"
#include "game.h"
#include "messages.h"
#include "playersList.h"

#define BACKLOG_SIZE 1024

struct CLIENTS_LIST clients_list;
struct PLAYERS_LIST players_list;

struct CLIENT_THREAD_DATA {
    int client_fd;
};

void send_message(int client_fd, char *message) {
    // TODO: Make sure the whole message was sent
    ssize_t message_length = strlen(message);
//    size_t sent_bytes = 0;
    ssize_t n;
//    while (sent_bytes < message_length) {
    n = send(client_fd, message, message_length, 0);
//        sent_bytes += n; // TODO: What if n == 0 or n < 0
//    }
//    printf("Sent %d\n", (int)sent_bytes);
}

void *receive_message(int client_fd, char *buf, ssize_t buf_size) {
    ssize_t n;
    n = recv(client_fd, buf, buf_size, 0);
    if (n > 0)
        printf("%s\n", buf);
    // TODO: Add error handling
}

void end_tour(struct ROOM *room) {
    struct GAME_INSTANCE *instance = room->game_instance;
    if (instance->game_state == STATE_DARK_TURN) {
        instance->game_state = STATE_LIGHT_TURN;
        send_message(room->player_one->file_descriptor, MSG_LIGHT_TURN);
        send_message(room->player_two->file_descriptor, MSG_LIGHT_TURN);
    } else if (instance->game_state == STATE_LIGHT_TURN) {
        instance->game_state = STATE_DARK_TURN;
        send_message(room->player_one->file_descriptor, MSG_DARK_TURN);
        send_message(room->player_two->file_descriptor, MSG_DARK_TURN);
    }

}

void server_check_promote_piece(struct ROOM *room, int field) {
    // Promote piece to king
    if (check_and_promote(room->game_instance, field) == 1) {
        char *message = malloc(sizeof(MSG_PROMOTE) + 3);
        sprintf(message, "%s%02d;", MSG_PROMOTE, field);
        send_message(room->player_one->file_descriptor, message);
        send_message(room->player_two->file_descriptor, message);
        free(message);
    }
}

void server_move_piece(struct PLAYER *current_player, int from_field, int to_field) {
    struct GAME_INSTANCE *instance = current_player->room->game_instance;
    struct MOVE_RESULT move_result = move_piece(instance, from_field, to_field, current_player->player_color);
    if (move_result.end_tour == 1) {
        end_tour(current_player->room);
    }
    printf("Error %d\n", (int) move_result.move_error);
    char *error_message = malloc(sizeof(MSG_GAME_ERROR) + 2);
    sprintf(error_message, "%s%d;", MSG_GAME_ERROR, move_result.move_error);
    send_message(current_player->file_descriptor, error_message);
    free(error_message);
    show_board(instance);
    if (move_result.move_error == ERROR_NO_ERROR) { // If there was no error, inform the other player about the move
        char *message = malloc(sizeof(MSG_PIECE_MOVE) + 6);
        sprintf(message, "%s%02d_%02d;", MSG_PIECE_MOVE, from_field, to_field);
        int other_player_fd;
        if (current_player->file_descriptor == current_player->room->player_one->file_descriptor)
            other_player_fd = current_player->room->player_two->file_descriptor;
        else
            other_player_fd = current_player->room->player_one->file_descriptor;

        send_message(other_player_fd, message);
        free(message);
        if (move_result.captured_piece_field != -1) { // Inform players that the piece was captured
            message = malloc(sizeof(MSG_PIECE_MOVE) + 6);
            sprintf(message, "%s%02d_%02d;", MSG_PIECE_MOVE, move_result.captured_piece_field, -1);
            send_message(current_player->file_descriptor, message);
            send_message(other_player_fd, message);
            free(message);
        }
        server_check_promote_piece(current_player->room, to_field);
    }
}

void parse_message(struct PLAYER *player, char *message, int message_length) {
    if (player->player_color == COLOR_NO_COLOR) // If there is no game
        return;
    struct GAME_INSTANCE *instance = player->room->game_instance;
    if (instance->game_state == STATE_LIGHT_TURN || instance->game_state == STATE_DARK_TURN) {
        if (strlen(message) >= 16) {
            if (strncmp(message, "move_piece_", 11) == 0) {
                char number[2] = {message[11], message[12]};
                int from_field = atoi(number);
                number[0] = message[14];
                number[1] = message[15];
                int to_field = atoi(number);
                server_move_piece(player, from_field, to_field);
            }
        }
    }
}

struct ROOM *join_room(struct PLAYER *player) {
    struct ROOM *room = player_list_get_free_room(&players_list);
    if (room == NULL) {  // There is no free room
        // Create new room
        room = malloc(sizeof(struct ROOM));
        room_init(room);
        printf("New room created\n");
        send_message(player->file_descriptor, MSG_WAITING_FOR_OPPONENT);
    }
    player_assign_room(player, room);
    return room;
}

void start_game(struct ROOM *room) {
    printf("DEBUG: Game starting\n");
    place_pieces(room->game_instance);
    if (rand() % 2) {
        room->player_one->player_color = COLOR_LIGHT;
        room->player_two->player_color = COLOR_DARK;
        send_message(room->player_one->file_descriptor, MSG_YOU_PLAY_LIGHT);
        send_message(room->player_two->file_descriptor, MSG_YOU_PLAY_DARK);
    } else {
        room->player_one->player_color = COLOR_DARK;
        room->player_two->player_color = COLOR_LIGHT;
        send_message(room->player_one->file_descriptor, MSG_YOU_PLAY_DARK);
        send_message(room->player_two->file_descriptor, MSG_YOU_PLAY_LIGHT);
    }
}

void *_handle_new_connection(void *client_thread_data_param) {
    pthread_detach(pthread_self());
    struct CLIENT_THREAD_DATA *player_data = (struct CLIENT_THREAD_DATA *) (client_thread_data_param);
    printf("New client connected\n");
    int player_fd = player_data->client_fd;
    printf("Client fd %d\n", player_fd);
    send_message(player_fd, MSG_WELCOME);

    struct PLAYER *player = player_list_add(&players_list, player_fd);
    struct ROOM *room = join_room(player);
    if (room->number_of_players == 2)
        start_game(room);
    free(player_data);
    return 0;
}

void *_handle_existing_connection(void *client_thread_data_param) {
    pthread_detach(pthread_self());
    struct CLIENT_THREAD_DATA *player_data = (struct CLIENT_THREAD_DATA *) (client_thread_data_param);
    printf("DEBUG: Existing connection\n");
    char buf[256];
    int player_fd = player_data->client_fd;
    struct PLAYER *player = player_get_by_fd(&players_list, player_fd);
    receive_message(player_fd, buf, 256);
    parse_message(player, buf, strlen(buf));
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

    srand(132);
    client_list_init(&clients_list);
    player_list_init(&players_list);
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
                pthread_t thread;
                struct CLIENT_THREAD_DATA *thread_data = malloc(sizeof(struct CLIENT_THREAD_DATA));
                if (clients_list.clients_fd[i].fd == server_socket_fd) {
                    // Handle a new connection
                    int client_fd = accept(server_socket_fd, NULL, NULL);
                    thread_data->client_fd = client_fd;
                    // Add client to the list
                    client_list_add(&clients_list, client_fd);
                    pthread_create(&thread, NULL, _handle_new_connection, thread_data);
                } else {
                    int client_fd = clients_list.clients_fd[i].fd;
                    // Handle existing connection
                    thread_data->client_fd = client_fd;
                    pthread_create(&thread, NULL, _handle_existing_connection, thread_data);
                } // if (clients_list.clients_fd[i].fd == server_socket_fd)
            } // if (clients_list.clients_fd[i].revents & POLLIN)
        } // Poll loop
    } // Main loop
    close(server_socket_fd);
}
