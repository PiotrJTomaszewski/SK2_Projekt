#ifndef CHECKERSSERVER_CIRCURALBUFFER_H
#define CHECKERSSERVER_CIRCURALBUFFER_H
#include <malloc.h>
#include <stdbool.h>

struct CIRC_BUFFER {
    size_t size;
    char *buffer;
    int head;  // First byte in the buffer (writer position)
    int tail; // The last byte in the buffer (reader position)
    int to_read; // Number of bytes to read
};

struct CIRC_BUFFER_RESULT {
    char byte;
    bool error;
};

void circ_buffer_init(struct CIRC_BUFFER *circ_buffer, size_t size);

void circ_buffer_free_memory(struct CIRC_BUFFER *circ_buffer);

struct CIRC_BUFFER_RESULT circ_buffer_read_byte(struct CIRC_BUFFER *circ_buffer);

struct CIRC_BUFFER_RESULT circ_buffer_peek_byte(struct CIRC_BUFFER *circ_buffer);

struct CIRC_BUFFER_RESULT circ_buffer_peek_head_byte(struct CIRC_BUFFER *circ_buffer);

bool circ_buffer_write_byte(struct CIRC_BUFFER *circ_buffer, char byte);

size_t circ_buffer_get_free_space(struct CIRC_BUFFER *circ_buffer);

#endif //CHECKERSSERVER_CIRCURALBUFFER_H
