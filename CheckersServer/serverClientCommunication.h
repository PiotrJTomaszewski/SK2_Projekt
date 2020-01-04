#ifndef CHECKERSSERVER_SERVERCLIENTCOMMUNICATION_H
#define CHECKERSSERVER_SERVERCLIENTCOMMUNICATION_H

#include <stdbool.h>
#include <pthread.h>
#include "player.h"
#include "messages.h"

/**
 * A structure representing the result of communication
 */
enum SER_CLI_COM_RESULT {
    SER_CLI_COM_NO_ERROR,
    SER_CLI_COM_SOCKET_CLOSED,
    SER_CLI_COM_OTHER_ERROR,
};

/**
 * A structure representing parsed message
 */
struct PARSED_MESSAGE_STRUCT {
    int error;
    enum SERVER_CLIENT_MESSAGE message_code;
    int param1;
    int param2;
};


/**
 * Receives all available bytes from client and puts it into the buffer.
 * @param player A player to receive message from.
 * @return A number representing the error code. 0 means success
 * @see SER_CLI_COM_RESULT
 */
enum SER_CLI_COM_RESULT ser_cli_com_receive(struct PLAYER *player);

/**
 * Parses the next message from buffer.
 * @param player A player whose buffer shuld be used
 * @return A structure representing the result
 * @see PARSED_MESSAGE_STRUCT
 */
struct PARSED_MESSAGE_STRUCT ser_cli_com_parse_next(struct PLAYER *player);

/**
 * Sends the message to the player.
 * @param player The player that should receive the message
 * @param message_code A message to send
 * @param param1 The first parameter of the message
 * @param param2 The second parameter of the message
 * @return 0 on success, -1 on error
 */
int ser_cli_com_send_message(struct PLAYER *player, enum SERVER_CLIENT_MESSAGE message_code, int param1, int param2);

#endif //CHECKERSSERVER_SERVERCLIENTCOMMUNICATION_H
