#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(const size_t buffSz)
{
    this->bufferSize = buffSz;
    this->buffer = new unsigned char[bufferSize];
}

void FrameBuffer::byteOR(int n, unsigned char byte)
{
    // return if index outside 0 - buffer length - 1
    if (n > (bufferSize - 1))
        return;
    this->buffer[n] |= byte;
}

void FrameBuffer::byteAND(int n, unsigned char byte)
{
    // return if index outside 0 - buffer length - 1
    if (n > (bufferSize - 1))
        return;
    this->buffer[n] &= byte;
}

void FrameBuffer::byteXOR(int n, unsigned char byte)
{
    // return if index outside 0 - buffer length - 1
    if (n > (bufferSize - 1))
        return;
    this->buffer[n] ^= byte;
}

void FrameBuffer::setBuffer(unsigned char* new_buffer)
{
    this->buffer = new_buffer;
}

void FrameBuffer::clear()
{
    // zeroes out the buffer via memset function from string library
    memset(this->buffer, 0, bufferSize);
}

unsigned char* FrameBuffer::get()
{
    return this->buffer;
}
