#include "ringBuffer.h"

struct CIRC_BUFFER *circ_buffer_create_new(size_t size) {
    struct CIRC_BUFFER *buffer = malloc(sizeof(struct CIRC_BUFFER));
    if (buffer != NULL) {
        buffer->buffer = malloc(size);
        if (buffer->buffer != NULL) {
            buffer->size = size;
            buffer->head = 0;
            buffer->tail = 0;
            buffer->to_read = 0;
        } else {
            perror("Error while creating circular buffer");
            return NULL;
        }
    } else {
        perror("Error while creating circular buffer");
        return NULL;
    }
    return buffer;
}

void circ_buffer_delete(struct CIRC_BUFFER *circ_buffer) {
    free(circ_buffer->buffer);
    free(circ_buffer);
}

struct CIRC_BUFFER_RESULT circ_buffer_read_byte(struct CIRC_BUFFER *circ_buffer) {
    struct CIRC_BUFFER_RESULT result;
    if (circ_buffer->to_read > 0) {
        result.byte = circ_buffer->buffer[circ_buffer->tail];
        circ_buffer->tail = (int) ((circ_buffer->tail + 1) % circ_buffer->size);
        circ_buffer->to_read--;
        result.error = false;
    } else {
        result.byte = (char) 0;
        result.error = true;
    }
    return result;
}

int circ_buffer_write_byte(struct CIRC_BUFFER *circ_buffer, char byte) {
    if (circ_buffer->to_read + 1 < (int) circ_buffer->size) {
        circ_buffer->buffer[circ_buffer->head] = byte;
        circ_buffer->head = (int) ((circ_buffer->head + 1) % circ_buffer->size);
        circ_buffer->to_read++;
        return 0;
    }
    printf("Error while writing a byte to the circular buffer\n");
    return -1;
}

int circ_buffer_write_bytes(struct CIRC_BUFFER *circ_buffer, char *bytes, unsigned number_of_bytes) {
    for (unsigned i = 0; i < number_of_bytes; ++i) {
        if (circ_buffer_write_byte(circ_buffer, bytes[i]) < 0) {
            return -1;
        }
    }
    return 0;
}
