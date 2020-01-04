#include <stdio.h>
#include <stdlib.h>
#include "serverGame.h"
#include "serverClientCommunication.h"
#include "game.h"
#include "player.h"
#include "messages.h"

void _server_game_end_tour(struct ROOM *room) {
    struct GAME_INSTANCE *instance = room->game_instance;
    if (instance->game_state == STATE_DARK_TURN) {
        instance->game_state = STATE_LIGHT_TURN;
        ser_cli_com_send_message(room->player_one, SCMSG_LIGHT_TURN, 0, 0);
        ser_cli_com_send_message(room->player_two, SCMSG_LIGHT_TURN, 0, 0);
    } else if (instance->game_state == STATE_LIGHT_TURN) {
        instance->game_state = STATE_DARK_TURN;
        ser_cli_com_send_message(room->player_one, SCMSG_DARK_TURN, 0, 0);
        ser_cli_com_send_message(room->player_two, SCMSG_DARK_TURN, 0, 0);
    }
}

void _server_game_check_promote_piece(struct ROOM *room, int field) {
    // Promote piece to king
    if (check_and_promote(room->game_instance, field) == 1) {
        ser_cli_com_send_message(room->player_one, SCMSG_PROMOTE, field, 0);
        ser_cli_com_send_message(room->player_two, SCMSG_PROMOTE, field, 0);
    }
}

bool server_game_move_piece(struct ROOM *room, struct PLAYER *current_player, int from_field, int to_field) {
    struct GAME_INSTANCE *instance = room->game_instance;
    // Move the piece
//    pthread_mutex_lock(&room->room_lock);
    struct MOVE_RESULT move_result = move_piece(instance, from_field, to_field, current_player->player_color);
//    pthread_mutex_unlock(&room->room_lock);
    if (move_result.end_tour == 1) { // If the tour should end
        _server_game_end_tour(room);
    }
    // Send the move error code to the player
    ser_cli_com_send_message(current_player, SCMSG_GAME_ERROR, move_result.move_error, 0);
    show_board(instance);
    if (move_result.move_error == ERROR_NO_ERROR) { // If there was no error, inform both players about the move
        ser_cli_com_send_message(room->player_one, SCMSG_PIECE_MOVED, from_field, to_field);
        ser_cli_com_send_message(room->player_two, SCMSG_PIECE_MOVED, from_field, to_field);
        if (move_result.captured_piece_field != -1) { // Inform players if a piece was captured
            ser_cli_com_send_message(room->player_one, SCMSG_PIECE_CAPTURED, move_result.captured_piece_field, 0);
            ser_cli_com_send_message(room->player_two, SCMSG_PIECE_CAPTURED, move_result.captured_piece_field, 0);
        }
        _server_game_check_promote_piece(room, to_field);
        return true;
    }
    return false;
}

void server_game_start_game(struct ROOM *room) {
    printf("DEBUG: Game starting\n");
    place_pieces(room->game_instance);
    if (rand() % 2) {
        room->player_one->player_color = COLOR_LIGHT;
        room->player_two->player_color = COLOR_DARK;
        ser_cli_com_send_message(room->player_one, SCMSG_GAME_START_YOU_PLAY_LIGHT, 0, 0);
        ser_cli_com_send_message(room->player_two, SCMSG_GAME_START_YOU_PLAY_DARK, 0, 0);
    } else {
        room->player_one->player_color = COLOR_DARK;
        room->player_two->player_color = COLOR_LIGHT;
        ser_cli_com_send_message(room->player_one, SCMSG_GAME_START_YOU_PLAY_DARK, 0, 0);
        ser_cli_com_send_message(room->player_two, SCMSG_GAME_START_YOU_PLAY_LIGHT, 0, 0);
    }
    ser_cli_com_send_message(room->player_one, SCMSG_LIGHT_TURN, 0, 0);
    ser_cli_com_send_message(room->player_two, SCMSG_LIGHT_TURN, 0, 0);
}

bool server_game_check_if_the_game_has_ended(struct ROOM *room) {
    if (is_game_end(room->game_instance)) {
        switch (room->game_instance->game_state) {
            case STATE_LIGHT_WON:
                ser_cli_com_send_message(room->player_one, SCMSG_GAME_END_LIGHT_WON, 0, 0);
                ser_cli_com_send_message(room->player_two, SCMSG_GAME_END_LIGHT_WON, 0, 0);
                break;
            case STATE_DARK_WON:
                ser_cli_com_send_message(room->player_one, SCMSG_GAME_END_DARK_WON, 0, 0);
                ser_cli_com_send_message(room->player_two, SCMSG_GAME_END_DARK_WON, 0, 0);
                break;
            case STATE_TIE:
                ser_cli_com_send_message(room->player_one, SCMSG_GAME_END_TIE, 0, 0);
                ser_cli_com_send_message(room->player_two, SCMSG_GAME_END_TIE, 0, 0);
                break;
            default:
                break;
        }
        return true;
    } else return false;
}
