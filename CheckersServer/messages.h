#ifndef CHECKERSSERVER_MESSAGES_H
#define CHECKERSSERVER_MESSAGES_H
#include <stdbool.h>

//#define MESSAGE_LENGTH 30

//#define MSG_WELCOME "welcome;"
//#define MSG_YOU_PLAY_LIGHT "play_light;"
//#define MSG_YOU_PLAY_DARK "play_dark;"
//#define MSG_WAITING_FOR_OPPONENT "waiting_for_opponent;"
//#define MSG_ROOM_FOUND "room_found;"
//#define MSG_GAME_ERROR "game_error_"
//#define MSG_LIGHT_TURN "light_turn;"
//#define MSG_DARK_TURN "dark_turn;"
//#define MSG_PIECE_MOVE "pieced_moved_"
//#define MSG_PROMOTE "promote_"

#define SCMSG_MESSAGE_LENGTH 9

enum SERVER_CLIENT_MESSAGE {
    // Messages that are send by the server
    SCMSG_WELCOME,
    SCMSG_WAITING_FOR_OPPONENT,
    SCMSG_GAME_START_YOU_PLAY_LIGHT,
    SCMSG_GAME_START_YOU_PLAY_DARK,
    SCMSG_GAME_ERROR, // error code
    SCMSG_LIGHT_TURN,
    SCMSG_DARK_TURN,
    SCMSG_PIECE_MOVED, // from, to
    SCMSG_PIECE_CAPTURED, // which
    SCMSG_PROMOTE, // which
    SCMSG_LIGHT_WON,
    SCMSG_DARK_WON,
    SCMSG_OPPONENT_LEFT,
    SCMSG_CRITICAL_ERROR,

    // Messages that are send by a client
    SCMSG_MOVE_PIECE, // from, to
    SCMSG_GOODBYE

};


#endif //CHECKERSSERVER_MESSAGES_H

