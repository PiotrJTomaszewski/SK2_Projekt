#ifndef CHECKERSSERVER_SERVERCLIENTCOMMUNICATION_H
#define CHECKERSSERVER_SERVERCLIENTCOMMUNICATION_H

#include <stdbool.h>
#include <pthread.h>
#include "player.h"
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

enum SER_CLI_COM_RESULT ser_cli_com_recv_and_parse(struct ROOM *room, struct PLAYER *player);

enum SER_CLI_COM_RESULT ser_cli_com_receive(struct PLAYER *player);

struct PARSED_MESSAGE_STRUCT ser_cli_com_parse(struct PLAYER *player);

/**
 * Sends the message to the player.
 * @param player The player
 * @param message_code A message to send
 * @param param1 The first parameter of the message
 * @param param2 The second parameter of the message
 * @return
 */
int ser_cli_com_send_message(struct PLAYER *player, enum SERVER_CLIENT_MESSAGE message_code, int param1, int param2);

/*
 * Executes the action that was received from the client.
 */
void ser_cli_com_take_action(struct ROOM *room, struct PLAYER *player, struct PARSED_MESSAGE_STRUCT *parsed_message);

#endif //CHECKERSSERVER_SERVERCLIENTCOMMUNICATION_H
