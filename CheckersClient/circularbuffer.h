#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#include <QByteArray>


class CircularBuffer
{
public:
    CircularBuffer(int size);
    ~CircularBuffer();

    // Read a byte from the buffer. Returns the read byte or -1 on error
    char readByte();

    // Writes a byte to the buffer. Returns number of bytes written (which is 1 or 0)
    int writeByte(char byte);

    // Writes bytes to the buffer. Returns number of bytes written
    int writeBytes(char *bytes, int number_of_bytes);

    // Writes bytes to the buffer. Returns number of bytes written
    int writeBytes(QByteArray bytes);

    // Clears the buffer
    void clear();

    // Returns a number of bytes that are currently available to read
    int getToRead();
private:
    int size;
    char *buffer;
    int head;
    int tail;
    // Number of bytes that are currently available to read
    int to_read;
};

#endif // CIRCULARBUFFER_H
