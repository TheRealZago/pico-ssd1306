#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(const size_t buffSz)
    : bufferSize(buffSz)
{
    this->buffer = std::make_unique<uint8_t[]>(bufferSize);
}

void FrameBuffer::byteOR(size_t n, uint8_t byte)
{
    // return if index outside 0 - buffer length - 1
    if (n > (bufferSize - 1))
        return;
    this->buffer[n] |= byte;
}

void FrameBuffer::byteAND(size_t n, uint8_t byte)
{
    // return if index outside 0 - buffer length - 1
    if (n > (bufferSize - 1))
        return;
    this->buffer[n] &= byte;
}

void FrameBuffer::byteXOR(size_t n, uint8_t byte)
{
    // return if index outside 0 - buffer length - 1
    if (n > (bufferSize - 1))
        return;
    this->buffer[n] ^= byte;
}

void FrameBuffer::setBuffer(const uint8_t* new_buffer, size_t newBuffSz)
{
    memcpy(this->buffer.get(), new_buffer, std::min(bufferSize, newBuffSz));
}

void FrameBuffer::clear()
{
    // zeroes out the buffer via memset function from string library
    memset(this->buffer.get(), 0, bufferSize);
}

uint8_t* FrameBuffer::get()
{
    return this->buffer.get();
}
