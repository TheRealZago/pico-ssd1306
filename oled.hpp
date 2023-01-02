#ifndef OLED_IFACE_H
#define OLED_IFACE_H

#include "frameBuffer/FrameBuffer.h"
#include "hardware/i2c.h"
#include <cstring>
#include <cstdint>

namespace pico_oled {
/// \enum pico_oled::Type
enum class Type {
    /// Display type SSD1306
    SSD1306,
    /// Display type SH1106
    SH1106
};

/// \enum pico_oled::Size
enum class Size {
    /// Display size W128xH64
    W128xH64,
    /// Display size W128xH32
    W128xH32
};

/// \enum pico_oled::WriteMode
enum class WriteMode : const unsigned char {
    /// sets pixel on regardless of its state
    ADD = 0,
    /// sets pixel off regardless of its state
    SUBTRACT = 1,
    /// inverts pixel, so 1->0 or 0->1
    INVERT = 2,
};

/// \class OLED oled.hpp "pico-oled/oled.hpp"
/// \brief OLED class represents underlying i2c connection to display
class OLED {
protected:
    i2c_inst* i2CInst = nullptr;
    uint16_t address = 0x00;
    Type type;
    Size size;
    FrameBuffer frameBuffer;
    uint8_t width = 128;
    uint8_t height = 64;
    bool inverted = false;

    virtual void cmd(uint8_t command) = 0;

public:
    /// \brief Generic OLED constructor for property setting
    /// \param i2CInst - i2c instance. Either i2c0 or i2c1
    /// \param Address - display i2c address. usually for 128x32 0x3C and for 128x64 0x3D
    /// \param type - display type. Acceptable values SSD1306 or SH1106
    /// \param size - display size. Acceptable values W128xH32 or W128xH64
    explicit OLED(i2c_inst* i2CInst, uint16_t Address, Type type, Size size) {
        // Set class instanced variables
        this->i2CInst = i2CInst;
        this->address = Address;
        this->size = size;

        if (size == Size::W128xH32) {
            this->height = 32;
        }

        // create a frame buffer
        this->frameBuffer = FrameBuffer();
    }

    virtual bool IsConnected() = 0;

    /// \brief Set pixel operates frame buffer
    /// x is the x position of pixel you want to change. values 0 - 127
    /// y is the y position of pixel you want to change. values 0 - 31 or 0 - 63
    /// \param x - position of pixel you want to change. values 0 - 127
    /// \param y - position of pixel you want to change. values 0 - 31 or 0 - 63
    /// \param mode - mode describes setting behavior. See WriteMode doc for more information
    virtual void setPixel(int16_t x, int16_t y, WriteMode mode = WriteMode::ADD) = 0;

    /// \brief Sends frame buffer to display so that it updated
    virtual void sendBuffer() = 0;

    /// \brief Adds bitmap image to frame buffer
    /// \param anchorX - sets start point of where to put the image on the screen
    /// \param anchorY - sets start point of where to put the image on the screen
    /// \param image_width - width of the image in pixels
    /// \param image_height - height of the image in pixels
    /// \param image - pointer to uint8_t (unsigned char) array containing image data
    /// \param mode - mode describes setting behavior. See WriteMode doc for more information
    inline void addBitmapImage(int16_t anchorX, int16_t anchorY, uint8_t image_width, uint8_t image_height, uint8_t* image, WriteMode mode = WriteMode::ADD)
    {
        uint8_t byte;
        // goes over every single bit in image and sets pixel data on its coordinates
        for (uint8_t y = 0; y < image_height; y++) {
            for (uint8_t x = 0; x < image_width / 8; x++) {
                byte = image[y * (image_width / 8) + x];
                for (uint8_t z = 0; z < 8; z++) {
                    if ((byte >> (7 - z)) & 1) {
                        this->setPixel(x * 8 + z + anchorX, y + anchorY, mode);
                    }
                }
            }
        }
    }

    /// \brief Manually set frame buffer. make sure it's correct size of 1024 bytes
    /// \param buffer - pointer to a new buffer
    inline void setBuffer(unsigned char* buffer)
    {
        this->frameBuffer.setBuffer(buffer);
    }

    /// \brief Flips the display
    /// \param orientation - 0 for not flipped, 1 for flipped display
    virtual void setOrientation(bool orientation) = 0;

    /// \brief Clears frame buffer aka set all bytes to 0
    inline void clear()
    {
        this->frameBuffer.clear();
    }

    /// \brief Inverts screen on hardware level. Way more efficient than setting buffer to all ones and then using WriteMode subtract.
    virtual void invertDisplay() = 0;

    /// \brief Sets display contrast according to ssd1306 documentation
    /// \param contrast - accepted values of 0 to 255 to set the contrast
    virtual void setContrast(unsigned char contrast) = 0;
};

}

#endif // OLED_IFACE_H