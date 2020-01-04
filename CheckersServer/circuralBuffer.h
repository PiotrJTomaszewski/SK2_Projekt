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

//void circ_buffer_init(struct CIRC_BUFFER *circ_buffer, size_t size);

/**
 * Allocates memory and creates a new instance of the circular buffer.
 * @param size A size of the buffer.
 * @return A new circular buffer instance or NULL on error.
 */
struct CIRC_BUFFER *circ_buffer_create_new(size_t size);

/**
 * Deletes the buffer freeing it's memory.
 * @param circ_buffer A buffer to delete.
 */
void circ_buffer_delete(struct CIRC_BUFFER *circ_buffer);


void circ_buffer_free_memory(struct CIRC_BUFFER *circ_buffer);

struct CIRC_BUFFER_RESULT circ_buffer_read_byte(struct CIRC_BUFFER *circ_buffer);

struct CIRC_BUFFER_RESULT circ_buffer_peek_byte(struct CIRC_BUFFER *circ_buffer);

struct CIRC_BUFFER_RESULT circ_buffer_peek_head_byte(struct CIRC_BUFFER *circ_buffer);

int circ_buffer_write_byte(struct CIRC_BUFFER *circ_buffer, char byte);

/**
 * Writes given number of bytes to the buffer.
 * @param circ_buffer
 * @param bytes
 * @param number_of_bytes
 * @return 0 on success, -1 on error
 */
int circ_buffer_write_bytes(struct CIRC_BUFFER *circ_buffer, char *bytes, unsigned number_of_bytes);

size_t circ_buffer_get_free_space(struct CIRC_BUFFER *circ_buffer);

#endif //CHECKERSSERVER_CIRCURALBUFFER_H
