#include "sh1106.hpp"

namespace pico_oled {
SH1106::SH1106(i2c_inst* i2CInst, uint16_t Address, Size size)
    : OLED::OLED(i2CInst, Address, Type::SH1106, size)
{
    // this is a list of setup commands for the display
    uint8_t setup[] = {
        SH1106_DISPLAY_OFF,
        SH1106_LOWCOLUMN,
        SH1106_HIGHCOLUMN,
        SH1106_STARTLINE,

        SH1106_CONTRAST,
        0x7F,

        SH1106_INVERTED_OFF,

        SH1106_MULTIPLEX,
        63,

        SH1106_DISPLAYOFFSET,
        0x00,

        SH1106_DISPLAYCLOCKDIV,
        0x80,

        SH1106_PRECHARGE,
        0x22,

        SH1106_COMPINS,
        0x12,

        SH1106_VCOMDETECT,
        0x40,

        SH1106_CHARGEPUMP,
        0x14,

        SH1106_DISPLAYALL_ON_RESUME,
        SH1106_DISPLAY_ON
    };

    // send each one of the setup commands
    for (uint8_t& command : setup) {
        this->cmd(command);
    }

    // clear the buffer and send it to the display
    // if not done display shows garbage data
    this->clear();
    this->sendBuffer();
}

void SH1106::setPixel(int16_t x, int16_t y, WriteMode mode)
{
    // return if position out of bounds
    if ((x < 0) || (x >= this->width) || (y < 0) || (y >= this->height))
        return;

    // byte to be used for buffer operation
    uint8_t byte;

    // display with 32 px height requires doubling of set bits, reason to this is explained in readme
    // this shifts 1 to byte based on y coordinate
    // remember that buffer is a one dimension array, so we have to calculate offset from coordinates
    if (size == Size::W128xH32) {
        y = (y << 1) + 1;
        byte = 1 << (y & 7);
        char byte_offset = byte >> 1;
        byte = byte | byte_offset;
    } else {
        byte = 1 << (y & 7);
    }

    // check the write mode and manipulate the frame buffer
    if (mode == WriteMode::ADD) {
        this->frameBuffer.byteOR(x + (y / 8) * this->width, byte);
    } else if (mode == WriteMode::SUBTRACT) {
        this->frameBuffer.byteAND(x + (y / 8) * this->width, ~byte);
    } else if (mode == WriteMode::INVERT) {
        this->frameBuffer.byteXOR(x + (y / 8) * this->width, byte);
    }
}

void SH1106::sendBuffer()
{
    this->cmd(SH1106_PAGEADDR);
    this->cmd(SH1106_LOWCOLUMN);
    this->cmd(SH1106_HIGHCOLUMN);

    // create a temporary buffer of size of buffer plus 1 byte for startline command aka 0x40
    unsigned char data[FRAMEBUFFER_SIZE + 1];

    data[0] = SH1106_STARTLINE;

    // copy framebuffer to temporary buffer
    memcpy(data + 1, frameBuffer.get(), FRAMEBUFFER_SIZE);

    // send data to device
    i2c_write_blocking(this->i2CInst, this->address, data, FRAMEBUFFER_SIZE + 1, false);
}

void SH1106::setOrientation(bool orientation)
{
    // remap columns and rows scan direction, effectively flipping the image on display
    if (orientation) {
        this->cmd(SH1106_CLUMN_REMAP_OFF);
        this->cmd(SH1106_COM_REMAP_OFF);
    } else {
        this->cmd(SH1106_CLUMN_REMAP_ON);
        this->cmd(SH1106_COM_REMAP_ON);
    }
}

void SH1106::invertDisplay()
{
    this->cmd(SH1106_INVERTED_OFF | !this->inverted);
    inverted = !inverted;
}

void SH1106::cmd(uint8_t command)
{
    // 0x00 is a byte indicating to SH1106 that a command is being sent
    uint8_t data[2] = { 0x00, command };
    i2c_write_blocking(this->i2CInst, this->address, data, 2, false);
}

void SH1106::setContrast(unsigned char contrast)
{
    this->cmd(SH1106_CONTRAST);
    this->cmd(contrast);
}

}
