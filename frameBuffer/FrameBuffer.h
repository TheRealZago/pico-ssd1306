#ifndef OLED_FRAMEBUFFER_H
#define OLED_FRAMEBUFFER_H

#include <string.h>

/// \brief Framebuffer class contains a pointer to buffer and functions for interacting with it
class FrameBuffer {
    size_t bufferSize { 0 };
    unsigned char* buffer { nullptr };

public:
    /// Constructs frame buffer and allocates memory for buffer
    FrameBuffer(const size_t buffSz);

    inline size_t GetBufferSize() const { return bufferSize; }

    /// \brief Performs OR logical operation on selected and provided byte
    ///
    /// ex. if byte in buffer at position n is 0b10001111 and provided byte is 0b11110000 the buffer at position n becomes 0b11111111
    /// \param n - byte offset in buffer array to work on
    /// \param byte - provided byte to make operation
    void byteOR(int n, unsigned char byte);

    /// \brief Performs AND logical operation on selected and provided byte
    ///
    /// ex. if byte in buffer at position n is 0b10001111 and provided byte is 0b11110000 the buffer at position n becomes 0b10000000
    /// \param n - byte offset in buffer array to work on
    /// \param byte - provided byte to make operation
    void byteAND(int n, unsigned char byte);

    /// \brief Performs XOR logical operation on selected and provided byte
    ///
    /// ex. if byte in buffer at position n is 0b10001111 and provided byte is 0b11110000 the buffer at position n becomes 0b0111111
    /// \param n - byte offset in buffer array to work on
    /// \param byte - provided byte to make operation
    void byteXOR(int n, unsigned char byte);

    /// Replaces pointer with one pointing to a different buffer
    void setBuffer(unsigned char* new_buffer);

    /// Zeroes out the buffer aka set buffer to all 0
    void clear();

    /// Returns a pointer to the buffer
    unsigned char* get();
};

#endif // OLED_FRAMEBUFFER_H
