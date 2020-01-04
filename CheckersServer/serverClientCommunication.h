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
//    SER_CLI_NOT_ENOUGH_TO_PARSE
};

struct PARSED_MESSAGE_STRUCT {
    int error;
    enum SERVER_CLIENT_MESSAGE message_code;
    int param1;
    int param2;
};

//struct PARSED_MESSAGE_STRUCT ser_cli_com_recv_and_parse(struct PLAYER *player);

/**
 * Receives all available bytes from client and puts it into the buffer.
 * @param player
 * @return
 */
enum SER_CLI_COM_RESULT ser_cli_com_receive(struct PLAYER *player);

/**
 * Parses the next message from buffer.
 * @param player
 * @return
 */
struct PARSED_MESSAGE_STRUCT ser_cli_com_parse_next(struct PLAYER *player);

/**
 * Sends the message to the player.
 * @param player The player
 * @param message_code A message to send
 * @param param1 The first parameter of the message
 * @param param2 The second parameter of the message
 * @return
 */
int ser_cli_com_send_message(struct PLAYER *player, enum SERVER_CLIENT_MESSAGE message_code, int param1, int param2);

#endif //CHECKERSSERVER_SERVERCLIENTCOMMUNICATION_H
