#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "test.h"
#include "server.h"

#include "circuralBuffer.h"
#include <string.h>

int main() {
//    struct GAME_INSTANCE *instance;
//    instance = malloc(sizeof(struct GAME_INSTANCE));
//    place_pieces(instance);
//    show_board(instance);
//    enum GAME_ERROR error;
//    error = move_piece(instance, 12, 1, 1, 1);
//    printf("%d\n", error);
//    show_board(instance);
//    free(instance);
//    run_all_tests();
    server_run(1234);

//    struct CIRC_BUFFER *buffer;
//    buffer = malloc(sizeof(struct CIRC_BUFFER));
//    circ_buffer_init(buffer, 128);
//    char message[] = "TEst1234";
//    for (unsigned long i=0; i < strlen(message); ++i) {
//        circ_buffer_write_byte(buffer, message[i]);
//    }
//    while (buffer->to_read != 0) {
//        struct CIRC_BUFFER_RESULT result;
//        result = circ_buffer_read_byte(buffer);
//        printf("%c\n", result.byte);
//    }
//    circ_buffer_free_memory(buffer);
//    free(buffer);

    return 0;
}
