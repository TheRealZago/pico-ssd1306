#ifndef OLED_FRAMEBUFFER_H
#define OLED_FRAMEBUFFER_H

#include <cstdint>
#include <cstring>
#include <memory>

/// \brief Framebuffer class contains a pointer to buffer and functions for interacting with it
class FrameBuffer {
    size_t bufferSize { 0 };
    std::unique_ptr<uint8_t[]> buffer { nullptr };

public:
    /// Constructs frame buffer and allocates memory for buffer
    explicit FrameBuffer(const size_t buffSz);

    inline size_t GetBufferSize() const { return bufferSize; }

    /// \brief Performs OR logical operation on selected and provided byte
    ///
    /// ex. if byte in buffer at position n is 0b10001111 and provided byte is 0b11110000 the buffer at position n becomes 0b11111111
    /// \param n - byte offset in buffer array to work on
    /// \param byte - provided byte to make operation
    void byteOR(size_t n, uint8_t byte);

    /// \brief Performs AND logical operation on selected and provided byte
    ///
    /// ex. if byte in buffer at position n is 0b10001111 and provided byte is 0b11110000 the buffer at position n becomes 0b10000000
    /// \param n - byte offset in buffer array to work on
    /// \param byte - provided byte to make operation
    void byteAND(size_t n, uint8_t byte);

    /// \brief Performs XOR logical operation on selected and provided byte
    ///
    /// ex. if byte in buffer at position n is 0b10001111 and provided byte is 0b11110000 the buffer at position n becomes 0b0111111
    /// \param n - byte offset in buffer array to work on
    /// \param byte - provided byte to make operation
    void byteXOR(size_t n, uint8_t byte);

    /// Replaces pointer with one pointing to a different buffer
    void setBuffer(const uint8_t* new_buffer, size_t newBuffSz);

    /// Zeroes out the buffer aka set buffer to all 0
    void clear();

    /// Returns a pointer to the buffer
    uint8_t* get();
};

#endif // OLED_FRAMEBUFFER_H
