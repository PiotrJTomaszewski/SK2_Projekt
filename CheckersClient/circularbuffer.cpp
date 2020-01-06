#include "circularbuffer.h"

CircularBuffer::CircularBuffer(int size) {
    this->buffer = new char[size];
    this->head = 0;
    this->tail = 0;
    this->to_read = 0;
    this->size = size;
}

CircularBuffer::~CircularBuffer() {
    delete this->buffer;
}

char CircularBuffer::readByte() {
    char read_byte = -1;
    if (this->to_read > 0) {
        read_byte = this->buffer[this->tail];
        this->tail = ((this->tail + 1) % this->size);
        this->to_read--;
    }
    return read_byte;
}

int CircularBuffer::writeByte(char byte) {
    int bytes_written = 0;
    // If there is a free space left in the buffer
    if (this->to_read + 1 < this->size) {
        this->buffer[this->head] = byte;
        this->head = ((this->head + 1) % this->size);
        this->to_read++;
        bytes_written = 1;
    }
    return bytes_written;
}

int CircularBuffer::writeBytes(char *bytes, int number_of_bytes) {
    int bytes_written = 0;
    int n;
    for (int i = 0; i < number_of_bytes; ++i) {
        n = writeByte(bytes[i]);
        bytes_written += n;
        // If there was an error (the byte wasn't written)
        if (n == 0) break;
    }
    return bytes_written;
}

int CircularBuffer::writeBytes(QByteArray bytes) {
    int bytes_written = 0;
    int n;
    for (int i = 0; i < bytes.size(); ++i) {
        n = writeByte(bytes[i]);
        bytes_written += n;
        if (n == 0) break;
    }
    return bytes_written;
}

void CircularBuffer::clear() {
    this->head = 0;
    this->tail = 0;
    this->to_read = 0;
}

int CircularBuffer::getToRead() {
    return this->to_read;
}
