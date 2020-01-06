/**
* @file messages.h
* @brief Message codes that make the communication protocol
*
* @author Piotr J. Tomaszewski
*/

#ifndef CHECKERSSERVER_MESSAGES_H
#define CHECKERSSERVER_MESSAGES_H

#define SCMSG_MESSAGE_LENGTH 9

enum SERVER_CLIENT_MESSAGE {
    // Messages that are send by the server
    // Parameters that should come with the messages are shown in comments

    // Messages that are send by server
    SCMSG_WAITING_FOR_OPPONENT,
    SCMSG_GAME_START,  // param - player color
    SCMSG_GAME_ERROR, // param - error code
    SCMSG_NEW_TURN,  // param - color
    SCMSG_PIECE_MOVED, // param - from, to
    SCMSG_PIECE_CAPTURED, // param - which
    SCMSG_PROMOTE, // param - which
    SCMSG_GAME_END, // param color
    SCMSG_OPPONENT_LEFT,
    SCMSG_CRITICAL_ERROR,

    // Messages that are send by a client
    SCMSG_MOVE_PIECE, // param -from, to
    SCMSG_GOODBYE

};


#endif //CHECKERSSERVER_MESSAGES_H
