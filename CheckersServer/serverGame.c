#include <stdio.h>
#include <stdlib.h>
#include "serverGame.h"
#include "serverClientCommunication.h"
#include "game.h"
#include "player.h"
#include "messages.h"


void server_game_move_piece(struct PLAYER *current_player, int from_field, int to_field) {
    struct ROOM *room = current_player->room;
    struct GAME_INSTANCE *instance = current_player->room->game_instance;
    struct MOVE_RESULT move_result = move_piece(instance, from_field, to_field, current_player->player_color);
    if (move_result.end_tour == 1) {
        server_game_end_tour(current_player->room);
    }
    printf("Error %d\n", (int) move_result.move_error);
    ser_cli_com_send_message(current_player->file_descriptor, SCMSG_GAME_ERROR, move_result.move_error, 0);
    show_board(instance);
    if (move_result.move_error == ERROR_NO_ERROR) { // If there was no error, inform the other player about the move
//        // Get fd of the other player
//        int other_player_fd;
//        if (current_player->file_descriptor == current_player->room->player_one->file_descriptor)
//            other_player_fd = current_player->room->player_two->file_descriptor;
//        else
//            other_player_fd = current_player->room->player_one->file_descriptor;
//        ser_cli_com_send_message(other_player_fd, SCMSG_PIECE_MOVED, from_field, to_field);
        ser_cli_com_send_message(room->player_one->file_descriptor, SCMSG_PIECE_MOVED, from_field,
                                 to_field);
        ser_cli_com_send_message(room->player_two->file_descriptor, SCMSG_PIECE_MOVED, from_field,
                                 to_field);
        if (move_result.captured_piece_field != -1) { // Inform players that the piece was captured
            ser_cli_com_send_message(room->player_one->file_descriptor, SCMSG_PIECE_CAPTURED,
                                     move_result.captured_piece_field, 0);
            ser_cli_com_send_message(room->player_two->file_descriptor, SCMSG_PIECE_CAPTURED,
                                     move_result.captured_piece_field, 0);
        }
        server_game_check_promote_piece(current_player->room, to_field);
    }
}

void server_game_end_tour(struct ROOM *room) {
    struct GAME_INSTANCE *instance = room->game_instance;
    if (instance->game_state == STATE_DARK_TURN) {
        instance->game_state = STATE_LIGHT_TURN;
        ser_cli_com_send_message(room->player_one->file_descriptor, SCMSG_LIGHT_TURN, 0, 0);
        ser_cli_com_send_message(room->player_two->file_descriptor, SCMSG_LIGHT_TURN, 0, 0);
    } else if (instance->game_state == STATE_LIGHT_TURN) {
        instance->game_state = STATE_DARK_TURN;
        ser_cli_com_send_message(room->player_one->file_descriptor, SCMSG_DARK_TURN, 0, 0);
        ser_cli_com_send_message(room->player_two->file_descriptor, SCMSG_DARK_TURN, 0, 0);
    }
}

void server_game_check_promote_piece(struct ROOM *room, int field) {
    // Promote piece to king
    if (check_and_promote(room->game_instance, field) == 1) {
        ser_cli_com_send_message(room->player_one->file_descriptor, SCMSG_PROMOTE, field, 0);
        ser_cli_com_send_message(room->player_two->file_descriptor, SCMSG_PROMOTE, field, 0);
    }
}

void server_game_start_game(struct ROOM *room) {
    srand((unsigned) time(NULL));
    printf("DEBUG: Game starting\n");
    place_pieces(room->game_instance);
    if (rand() % 2) {
        room->player_one->player_color = COLOR_LIGHT;
        room->player_two->player_color = COLOR_DARK;
        ser_cli_com_send_message(room->player_one->file_descriptor, SCMSG_YOU_PLAY_LIGHT, 0, 0);
        ser_cli_com_send_message(room->player_two->file_descriptor, SCMSG_YOU_PLAY_DARK, 0, 0);
    } else {
        room->player_one->player_color = COLOR_DARK;
        room->player_two->player_color = COLOR_LIGHT;
        ser_cli_com_send_message(room->player_one->file_descriptor, SCMSG_YOU_PLAY_DARK, 0, 0);
        ser_cli_com_send_message(room->player_two->file_descriptor, SCMSG_YOU_PLAY_LIGHT, 0, 0);
    }
}
