#ifndef CHECKERSSERVER_CIRCURALBUFFER_H
#define CHECKERSSERVER_CIRCURALBUFFER_H

#include <malloc.h>
#include <stdbool.h>

/**
 * A structure representing a circular buffer.
 */
struct CIRC_BUFFER {
    size_t size;
    char *buffer;
    int head;  // First byte in the buffer (writer position)
    int tail; // The last byte in the buffer (reader position)
    int to_read; // Number of bytes to read
};

/**
 * A structure representing the result of read operation on a circular buffer.
 */
struct CIRC_BUFFER_RESULT {
    char byte;
    bool error;
};

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


/**
 * Read the next byte from the circular buffer.
 * @param circ_buffer
 * @return
 */
struct CIRC_BUFFER_RESULT circ_buffer_read_byte(struct CIRC_BUFFER *circ_buffer);

/**
 * Write a byte to the circular buffer
 * @param circ_buffer
 * @param byte
 * @return
 */
int circ_buffer_write_byte(struct CIRC_BUFFER *circ_buffer, char byte);

/**
 * Writes given number of bytes to the buffer.
 * @param circ_buffer
 * @param bytes
 * @param number_of_bytes
 * @return 0 on success, -1 on error
 */
int circ_buffer_write_bytes(struct CIRC_BUFFER *circ_buffer, char *bytes, unsigned number_of_bytes);


#endif //CHECKERSSERVER_CIRCURALBUFFER_H
