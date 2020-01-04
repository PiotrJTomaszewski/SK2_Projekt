#include <sys/socket.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "serverClientCommunication.h"
#include "serverGame.h"
#include "circuralBuffer.h"
#include "messages.h"
#include "player.h"

void ser_cli_com_init() {
}

enum SER_CLI_COM_RESULT ser_cli_com_recv_and_parse(struct ROOM *room, struct PLAYER *player) {
    enum SER_CLI_COM_RESULT recv_result;
    recv_result = ser_cli_com_receive(player);
    if (recv_result == SER_CLI_COM_SOCKET_CLOSED) {
        printf("Message receive: Socket closed\n");
        return SER_CLI_COM_SOCKET_CLOSED;
    }
    struct PARSED_MESSAGE_STRUCT parsed_message = ser_cli_com_parse(player);
    // If there was message received
    if (parsed_message.result == SER_CLI_COM_NO_ERROR) {
        ser_cli_com_take_action(room, player, &parsed_message);
    }
    return SER_CLI_COM_NO_ERROR;
}

//enum SER_CLI_COM_RESULT ser_cli_com_receive(struct PLAYER *player, struct CIRC_BUFFER *buffer) {
//    ssize_t n;
//    enum SER_CLI_COM_RESULT result = SER_CLI_COM_NO_ERROR;
//    int error;
//    char *local_buf = malloc(SCMSG_MESSAGE_LENGTH + 1);
//    pthread_mutex_lock(&player->fd_lock);
//    bool run_flag = true;
//    while (run_flag) {
//        if (circ_buffer_get_free_space(buffer) <= SCMSG_MESSAGE_LENGTH + 1) {
//            result = SER_CLI_COM_FULL_BUFFER;
//            printf("Error! No space left in the buffer\n");
//            run_flag = false;
//            break;
//        }
//        n = recv(player->file_descriptor, local_buf, SCMSG_MESSAGE_LENGTH, MSG_DONTWAIT);
//        if (n > 0) {
//            printf("Received %d bytes\n", (int) n);
//            for (int i = 0; i < n; ++i) {
//                error = circ_buffer_write_byte(buffer, local_buf[i]);
//                if (error) {
//                    printf("Error! No space left in the buffer\n");
//                    result = SER_CLI_COM_FULL_BUFFER;
//                    run_flag = false;
//                }
//            }
//        }
//        if (n == 0) {
//            // Socket closed
//            printf("Socket closed\n");
//            result = SER_CLI_COM_SOCKET_CLOSED;
//            // TODO: Delete client
//            run_flag = false;
//        }
//        if (n < 0) {
//            // Error
//            // TODO: This :)
//            printf("Reading would block\n");
//            //errno - EAGAIN lub EWOULDBLOCK
//            result = SER_CLI_COM_OTHER_ERROR;
//            run_flag = false;
//        }
//    }
//    pthread_mutex_unlock(&player->fd_lock);
//    return result;
//}

enum SER_CLI_COM_RESULT ser_cli_com_receive(struct PLAYER *player) {
    ssize_t n;
    enum SER_CLI_COM_RESULT result = SER_CLI_COM_NO_ERROR;
    char *local_buf = malloc(SCMSG_MESSAGE_LENGTH + 1);
    pthread_mutex_lock(&player->fd_lock);
    bool run_flag = true;
    while (run_flag) {
        n = recv(player->file_descriptor, local_buf, SCMSG_MESSAGE_LENGTH, MSG_DONTWAIT);
        if (n > 0) {
            circ_buffer_write_bytes(player->buffer, local_buf, (unsigned) n);
        }
        if (n == 0) {  // Socket closed
            printf("Socket closed\n");
            result = SER_CLI_COM_SOCKET_CLOSED;
            run_flag = false;
        }
        if (n < 0) { // Error
            //errno - EAGAIN lub EWOULDBLOCK
            run_flag = false;
            if (errno != EAGAIN || errno != EWOULDBLOCK) {
                result = SER_CLI_COM_OTHER_ERROR;
            }
        }
    }
    pthread_mutex_unlock(&player->fd_lock);
    return result;
}

struct PARSED_MESSAGE_STRUCT ser_cli_com_parse(struct PLAYER *player) {
    char local_buf[SCMSG_MESSAGE_LENGTH + 1];
    int read_bytes = 0;
    bool message_read = false;
    struct PARSED_MESSAGE_STRUCT result;
    result.result = SER_CLI_NOT_ENOUGH_TO_PARSE;
    pthread_mutex_lock(&player->fd_lock);
    if (player->buffer->to_read >= SCMSG_MESSAGE_LENGTH) {
        // Read data into local buffer
        while (!message_read) {
            local_buf[read_bytes] = circ_buffer_read_byte(player->buffer).byte;
            printf("%d %c\n", local_buf[read_bytes], local_buf[read_bytes]);
            if (local_buf[read_bytes] == '\n')
                message_read = true;
            ++read_bytes;
        }
    }
    pthread_mutex_unlock(&player->fd_lock);
    if (read_bytes > 0) {
        // Parse data
        int tmp;
        sscanf(local_buf, "%02d %02d %02d\n", &tmp, &result.param1, &result.param2);
        result.message_code = (enum SERVER_CLIENT_MESSAGE) tmp;
        result.result = SER_CLI_COM_NO_ERROR;
    }
    return result;
}

int ser_cli_com_send_message(struct PLAYER *player, enum SERVER_CLIENT_MESSAGE message_code, int param1, int param2) {
    // Create a message
    int error_occured = 0;
    char message[SCMSG_MESSAGE_LENGTH + 1];
    memset(message, 0, SCMSG_MESSAGE_LENGTH + 1);
    sprintf(message, "%02d %02d %02d\n", message_code, param1, param2);
    // Create a buffer
    char buffer[SCMSG_MESSAGE_LENGTH + 1];
    strcpy(buffer, message);
    // Send the message
    size_t bytes_left_to_send = SCMSG_MESSAGE_LENGTH;
    int bytes_sent = 0;
    ssize_t n;
    pthread_mutex_lock(&player->fd_lock);
    while (bytes_left_to_send > 0) {
        n = send(player->file_descriptor, buffer, bytes_left_to_send, 0);
        if (n > 0) {
            bytes_left_to_send -= n;
            bytes_sent += n;
            if (bytes_left_to_send > 0) {
                // Put the remaining bytes to the buffer
                memset(buffer, 0, SCMSG_MESSAGE_LENGTH + 1);
                strncpy(buffer, message + bytes_sent, bytes_left_to_send);
            }
        } else if (errno != EAGAIN || errno != EWOULDBLOCK) {
            perror("Error while sending a message to player");
            error_occured = 1;
            break;
        }
    }
    pthread_mutex_unlock(&player->fd_lock);
    return error_occured;
}

void ser_cli_com_take_action(struct ROOM *room, struct PLAYER *player, struct PARSED_MESSAGE_STRUCT *parsed_message) {
    switch (parsed_message->message_code) {
        case SCMSG_MOVE_PIECE:
            server_game_move_piece(room, player, parsed_message->param1, parsed_message->param2);
            printf("Message received: Move piece from %d to %d\n", parsed_message->param1, parsed_message->param2);
            break;
        case SCMSG_GOODBYE:
            printf("Message received: Goodbye\n");
            break;
        default:
            printf("Message received: Unknown %d %d %d\n", parsed_message->message_code, parsed_message->param1,
                   parsed_message->param2);
            break;
    }
}
