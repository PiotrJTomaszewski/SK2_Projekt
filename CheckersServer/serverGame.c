#include <stdio.h>
#include <stdlib.h>
#include "serverGame.h"
#include "serverClientCommunication.h"
#include "game.h"
#include "player.h"
#include "messages.h"


void server_game_move_piece(struct ROOM *room, struct PLAYER *current_player, int from_field, int to_field) {
    struct GAME_INSTANCE *instance = room->game_instance;
    // Move the piece
//    pthread_mutex_lock(&room->room_lock);
    struct MOVE_RESULT move_result = move_piece(instance, from_field, to_field, current_player->player_color);
//    pthread_mutex_unlock(&room->room_lock);
    if (move_result.end_tour == 1) { // If the tour should end
        server_game_end_tour(room);
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
        server_game_check_promote_piece(room, to_field);
    }
}

void server_game_end_tour(struct ROOM *room) {
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

void server_game_check_promote_piece(struct ROOM *room, int field) {
    // Promote piece to king
    if (check_and_promote(room->game_instance, field) == 1) {
        ser_cli_com_send_message(room->player_one, SCMSG_PROMOTE, field, 0);
        ser_cli_com_send_message(room->player_two, SCMSG_PROMOTE, field, 0);
    }
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

//struct ROOM *server_game_join_room(struct PLAYER *player, struct PLAYERS_LIST *players_list) {
//    pthread_mutex_lock(&players_list->players_list_lock); // TODO: Find a better lock for this job
//    struct ROOM *room = players_list_get_free_room(players_list);
//    if (room == NULL) {  // There is no free room
//        // Create new room
//        room = room_create_new();
//        printf("New room created\n");
//        ser_cli_com_send_message(player, SCMSG_WAITING_FOR_OPPONENT, 0, 0);
//    }
//    // Assign player to the room
//    player_assign_room(player, room);
//    pthread_mutex_unlock(&players_list->players_list_lock); // TODO: Find a better lock for this job
//    if (room->number_of_players == 2)
//        server_game_start_game(room);
//    return room;
//}
