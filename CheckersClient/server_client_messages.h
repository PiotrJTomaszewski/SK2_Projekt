#ifndef SERVER_CLIENT_MESSAGES_H
#define SERVER_CLIENT_MESSAGES_H

#define SCMSG_MESSAGE_LENGTH 9

enum SERVER_CLIENT_MESSAGE {
    // Messages that are send by the server
    SCMSG_WELCOME,
    SCMSG_WAITING_FOR_OPPONENT,
    SCMSG_YOU_PLAY_LIGHT,
    SCMSG_YOU_PLAY_DARK,
    SCMSG_GAME_ERROR, // error code
    SCMSG_LIGHT_TURN,
    SCMSG_DARK_TURN,
    SCMSG_PIECE_MOVED, // from, to
    SCMSG_PIECE_CAPTURED, // which
    SCMSG_PROMOTE, // which
    SCMSG_LIGHT_WON,
    SCMSG_DARK_WON,
    SCMSG_OPPONENT_LEFT,

    // Messages that are send by a client
    SCMSG_MOVE_PIECE, // from, to
    SCMSG_GOODBYE

};

#endif // SERVER_CLIENT_MESSAGES_H
