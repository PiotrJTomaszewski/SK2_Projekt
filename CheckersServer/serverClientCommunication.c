#include <sys/socket.h>
#include <stdbool.h>
#include <string.h>
#include "serverClientCommunication.h"
#include "serverGame.h"
#include "circuralBuffer.h"
#include "messages.h"
#include "player.h"

void ser_cli_com_init() {
}

enum SER_CLI_COM_RESULT ser_cli_com_get_and_parse(struct PLAYER *player) {
    enum SER_CLI_COM_RESULT recv_result;
    recv_result = ser_cli_com_receive(player->file_descriptor, player->buffer);
    // TODO: Error check
    if (recv_result == SER_CLI_COM_SOCKET_CLOSED) {
        printf("Message receive: Socket closed\n");
        return SER_CLI_COM_SOCKET_CLOSED;
    }
    struct PARSED_MESSAGE_STRUCT parsed_message = ser_cli_com_parse(player->buffer);
    // If there was message received
    if (parsed_message.result == SER_CLI_COM_NO_ERROR) {
        ser_cli_com_take_action(player, &parsed_message);
    }
    return SER_CLI_COM_NO_ERROR;
}

enum SER_CLI_COM_RESULT ser_cli_com_receive(int fd, struct CIRC_BUFFER *buffer) {
    ssize_t n;
    enum SER_CLI_COM_RESULT result = SER_CLI_COM_NO_ERROR;
    bool error;
//    pthread_mutex_lock(mutex);
    char *local_buf = malloc(SCMSG_MESSAGE_LENGTH + 1);
    bool run_flag = true;
    while (run_flag) {
        if (circ_buffer_get_free_space(buffer) <= SCMSG_MESSAGE_LENGTH + 1) {
            result = SER_CLI_COM_FULL_BUFFER;
            printf("No space left in the buffer\n");
            run_flag = false;
            break;
        }
        n = recv(fd, local_buf, SCMSG_MESSAGE_LENGTH, MSG_DONTWAIT);  // TODO: Non blocking ?
        if (n > 0) {
            printf("Received %d bytes\n", (int) n);
            for (int i = 0; i < n; ++i) {
                error = circ_buffer_write_byte(buffer, local_buf[i]);
                if (error) {
                    printf("No space left in the buffer\n");
                    result = SER_CLI_COM_FULL_BUFFER;
                    run_flag = false;
                }
            }
        }
        if (n == 0) {
            // Socket closed
            printf("Socket closed\n");
            result = SER_CLI_COM_SOCKET_CLOSED;
            // TODO: Delete client
            run_flag = false;
        }
        if (n < 0) {
            // Error
            // TODO: This :)
            printf("Reading would block\n");
            //errno - EAGAIN lub EWOULDBLOCK
            result = SER_CLI_COM_OTHER_ERROR;
            run_flag = false;
        }
    }
//    pthread_mutex_unlock(mutex);
    return result;
}

struct PARSED_MESSAGE_STRUCT ser_cli_com_parse(struct CIRC_BUFFER *buffer) {
    char local_buf[SCMSG_MESSAGE_LENGTH + 1];
    int read_bytes = 0;
    bool message_read = false;
    struct PARSED_MESSAGE_STRUCT result;
    result.result = SER_CLI_NOT_ENOUGH_TO_PARSE;
    if (buffer->to_read >= SCMSG_MESSAGE_LENGTH) {
        // Read data into local buffer
        while (!message_read) {
            local_buf[read_bytes] = circ_buffer_read_byte(buffer).byte;
            printf("%d %c\n", local_buf[read_bytes], local_buf[read_bytes]);
            if (local_buf[read_bytes] == '\n')
                message_read = true;
            ++read_bytes;
        }
        // Parse data
        int tmp;
        sscanf(local_buf, "%02d %02d %02d\n", &tmp, &result.param1, &result.param2);
        result.message_code = (enum SERVER_CLIENT_MESSAGE) tmp;
        result.result = SER_CLI_COM_NO_ERROR;
        printf("%d %d %d\n", tmp, result.param1, result.param2);
    }
    return result;
}

enum SER_CLI_COM_RESULT
ser_cli_com_send_message(int client_fd, enum SERVER_CLIENT_MESSAGE message_code, int param1, int param2) {
    enum SER_CLI_COM_RESULT result;
    result = SER_CLI_COM_NO_ERROR;
    char msg_buf[SCMSG_MESSAGE_LENGTH + 1];
    sprintf(msg_buf, "%02d %02d %02d\n", message_code, param1, param2);
    send(client_fd, msg_buf, SCMSG_MESSAGE_LENGTH, 0); // TODO: Check if all was sent
    return result;
}

void ser_cli_com_take_action(struct PLAYER *player, struct PARSED_MESSAGE_STRUCT *parsed_message) {
    switch (parsed_message->message_code) {
        case SCMSG_MOVE_PIECE:
            server_game_move_piece(player, parsed_message->param1, parsed_message->param2);
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
