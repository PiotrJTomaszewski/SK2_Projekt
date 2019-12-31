#include "circuralBuffer.h"

void circ_buffer_init(struct CIRC_BUFFER *circ_buffer, size_t size) {
    circ_buffer->buffer = malloc(size);
    circ_buffer->size = size;
    circ_buffer->head = 0;
    circ_buffer->tail = 0;
    circ_buffer->to_read = 0;
//    return circ_buffer;
}

void circ_buffer_free_memory(struct CIRC_BUFFER *circ_buffer) {
    free(circ_buffer->buffer);
}

struct CIRC_BUFFER_RESULT circ_buffer_read_byte(struct CIRC_BUFFER *circ_buffer) {
    struct CIRC_BUFFER_RESULT result;
    if (circ_buffer->to_read > 0) {
        result.byte = circ_buffer->buffer[circ_buffer->tail];
        circ_buffer->tail = (int) ((circ_buffer->tail + 1) % circ_buffer->size);
        circ_buffer->to_read--;
        result.error = false;
    } else {
        result.byte = (char) NULL;
        result.error = true;
    }
    return result;
}

struct CIRC_BUFFER_RESULT circ_buffer_peek_byte(struct CIRC_BUFFER *circ_buffer) {
    struct CIRC_BUFFER_RESULT result;
    if (circ_buffer->to_read > 0) {
        result.byte = circ_buffer->buffer[circ_buffer->tail];
        result.error = false;
    } else {
        result.byte = (char) NULL;
        result.error = true;
    }
    return result;
}

struct CIRC_BUFFER_RESULT circ_buffer_peek_head_byte(struct CIRC_BUFFER *circ_buffer) {
    struct CIRC_BUFFER_RESULT result;
    if (circ_buffer->to_read > 0) {
        result.byte = circ_buffer->buffer[circ_buffer->head-1];
        result.error = false;
    } else {
        result.byte = (char) NULL;
        result.error = true;
    }
    return result;
}

bool circ_buffer_write_byte(struct CIRC_BUFFER *circ_buffer, char byte) {
    if (circ_buffer->to_read + 1 < (int) circ_buffer->size) {
        circ_buffer->buffer[circ_buffer->head] = byte;
        circ_buffer->head = (int) ((circ_buffer->head + 1) % circ_buffer->size);
        circ_buffer->to_read++;
        return false;
    }
    return true;
}

size_t circ_buffer_get_free_space(struct CIRC_BUFFER *circ_buffer) {
    return circ_buffer->size - circ_buffer->to_read - 1;
}
