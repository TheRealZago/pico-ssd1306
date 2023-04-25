#include "ssd1306.hpp"

#define SSD1306_FULL_BUFFER 1024

namespace pico_oled {
SSD1306::SSD1306(i2c_inst* i2CInst, uint8_t Address, Size size)
    : OLED::OLED(i2CInst, Address, Type::SSD1306, size)
{
    // create a frame buffer
    this->frameBuffer = std::make_unique<FrameBuffer>(SSD1306_FULL_BUFFER);

    // this is a list of setup commands for the display
    uint8_t setup[] = {
        SSD1306_DISPLAY_OFF,
        SSD1306_LOWCOLUMN,
        SSD1306_HIGHCOLUMN,
        SSD1306_STARTLINE,

        SSD1306_MEMORYMODE,
        SSD1306_MEMORYMODE_HORZONTAL,

        SSD1306_CONTRAST,
        0x7F,

        SSD1306_INVERTED_OFF,

        SSD1306_MULTIPLEX,
        63,

        SSD1306_DISPLAYOFFSET,
        0x00,

        SSD1306_DISPLAYCLOCKDIV,
        0x80,

        SSD1306_PRECHARGE,
        0x22,

        SSD1306_COMPINS,
        0x12,

        SSD1306_VCOMDETECT,
        0x40,

        SSD1306_CHARGEPUMP,
        0x14,

        SSD1306_DISPLAYALL_ON_RESUME,
        SSD1306_DISPLAY_ON
    };

    // send each one of the setup commands
    for (const uint8_t& command : setup) {
        this->cmd(command);
    }

    // clear the buffer and send it to the display
    // if not done display shows garbage data
    this->clear();
    this->sendBuffer();
}

bool SSD1306::IsConnected() {
    uint8_t data[2] = { 0x00, SSD1306_DISPLAY_ON };
    int rc = i2c_write_timeout_us(this->i2CInst, this->address, data, 2, false, 50000);
    if (rc == PICO_ERROR_GENERIC || rc == PICO_ERROR_TIMEOUT) {
        return false;
    }
    return true;
}

void SSD1306::setPixel(const uint8_t x, const uint8_t y, const WriteMode mode)
{
    // return if position out of bounds
    if (x >= this->width || y >= this->height)
        return;

    // byte to be used for buffer operation
    uint8_t byte { 0x00 };
    uint8_t workY { y };

    // display with 32 px height requires doubling of set bits, reason to this is explained in readme
    // this shifts 1 to byte based on y coordinate
    // remember that buffer is a one dimension array, so we have to calculate offset from coordinates
    if (size == Size::W128xH32) {
        workY = static_cast<uint8_t>((workY << 1) + 1U);
        byte = static_cast<uint8_t>(1U << (workY & 7));
        uint8_t byte_offset = byte >> 1;
        byte = byte | byte_offset;
    } else {
        byte = static_cast<uint8_t>(1U << (workY & 7));
    }

    // check the write mode and manipulate the frame buffer
    if (mode == WriteMode::ADD) {
        this->frameBuffer->byteOR(x + (workY / 8) * this->width, byte);
    } else if (mode == WriteMode::SUBTRACT) {
        this->frameBuffer->byteAND(x + (workY / 8) * this->width, ~byte);
    } else if (mode == WriteMode::INVERT) {
        this->frameBuffer->byteXOR(x + (workY / 8) * this->width, byte);
    }
}

void SSD1306::sendBuffer()
{
    this->cmd(SSD1306_PAGEADDR); // Set page address from min to max
    this->cmd(0x00);
    this->cmd(0x07);
    this->cmd(SSD1306_COLUMNADDR); // Set column address from min to max
    this->cmd(0x00);
    this->cmd(127);

    // create a temporary buffer of size of buffer plus 1 byte for startline command aka 0x40
    unsigned char data[SSD1306_FULL_BUFFER + 1];

    data[0] = SSD1306_STARTLINE;

    // copy framebuffer to temporary buffer
    memcpy(data + 1, frameBuffer->get(), SSD1306_FULL_BUFFER);

    // send data to device
    i2c_write_timeout_us(this->i2CInst, this->address, data, SSD1306_FULL_BUFFER + 1, false, 50000);
}

void SSD1306::setOrientation(bool orientation)
{
    // remap columns and rows scan direction, effectively flipping the image on display
    if (orientation) {
        this->cmd(SSD1306_CLUMN_REMAP_OFF);
        this->cmd(SSD1306_COM_REMAP_OFF);
    } else {
        this->cmd(SSD1306_CLUMN_REMAP_ON);
        this->cmd(SSD1306_COM_REMAP_ON);
    }
}

void SSD1306::invertDisplay()
{
    this->cmd(SSD1306_INVERTED_OFF | !this->inverted);
    inverted = !inverted;
}

void SSD1306::cmd(const uint8_t& command)
{
    // 0x00 is a byte indicating to ssd1306 that a command is being sent
    uint8_t data[2] = { 0x00, command };
    i2c_write_timeout_us(this->i2CInst, this->address, data, 2, false, 50000);
}

void SSD1306::setContrast(unsigned char contrast)
{
    this->cmd(SSD1306_CONTRAST);
    this->cmd(contrast);
}

}
