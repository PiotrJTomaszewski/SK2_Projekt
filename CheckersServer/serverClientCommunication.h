#ifndef CHECKERSSERVER_SERVERCLIENTCOMMUNICATION_H
#define CHECKERSSERVER_SERVERCLIENTCOMMUNICATION_H

#include <stdbool.h>
#include <pthread.h>
#include "player.h"
#include "clientsList.h"
#include "messages.h"

enum SER_CLI_COM_RESULT {
    SER_CLI_COM_NO_ERROR,
    SER_CLI_COM_SOCKET_CLOSED,
    SER_CLI_COM_FULL_BUFFER,
    SER_CLI_COM_OTHER_ERROR,
    SER_CLI_NOT_ENOUGH_TO_PARSE
};

struct PARSED_MESSAGE_STRUCT {
    enum SER_CLI_COM_RESULT result;
    enum SERVER_CLIENT_MESSAGE message_code;
    int param1;
    int param2;
};

void ser_cli_com_init();

enum SER_CLI_COM_RESULT ser_cli_com_get_and_parse(struct PLAYER *player);

enum SER_CLI_COM_RESULT ser_cli_com_receive(int fd, struct CIRC_BUFFER *buffer);

struct PARSED_MESSAGE_STRUCT ser_cli_com_parse(struct CIRC_BUFFER *buffer);

enum SER_CLI_COM_RESULT
ser_cli_com_send_message(int client_fd, enum SERVER_CLIENT_MESSAGE message_code, int param1, int param2);

/*
 * Executes the action that was received from the client.
 */
void ser_cli_com_take_action(struct PLAYER *player, struct PARSED_MESSAGE_STRUCT *parsed_message);

#endif //CHECKERSSERVER_SERVERCLIENTCOMMUNICATION_H
