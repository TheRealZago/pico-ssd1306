#include "sh1106.hpp"

#define SH1106_MAX_WIDTH (132)
#define SH1106_MAX_HEIGHT (64)
#define SH1106_PAGE_HEIGHT (8)
#define SH1106_FULL_BUFFER ((SH1106_MAX_WIDTH * SH1106_MAX_HEIGHT) >> 3)
#define SH1106_PAGE_SIZE ((SH1106_MAX_WIDTH * SH1106_PAGE_HEIGHT) >> 3) // SH1106 writes in 8 bit tall stripes

namespace pico_oled {
SH1106::SH1106(i2c_inst* i2CInst, uint8_t Address, Size size)
    : OLED::OLED(i2CInst, Address, Type::SH1106, size)
{
    // create a frame buffer
    this->frameBuffer = std::make_unique<FrameBuffer>(SH1106_FULL_BUFFER);

    // this is a list of setup commands for the display
    uint8_t setup[] = {
        SH1106_DISPLAY_OFF,

        SH1106_CONTRAST,
        0x7F,

        SH1106_INVERTED_OFF,

        SH1106_CLUMN_REMAP_ON,
        SH1106_COM_REMAP_ON,

        SH1106_MULTIPLEX,
        static_cast<uint8_t>(this->height - 1),

        0x20, // set memory mode
        0x00, // standard horz scan

        SH1106_DISPLAYOFFSET,
        0x00,

        SH1106_STARTLINE | 0x0,

        SH1106_DISPLAYCLOCKDIV,
        0x80,

        SH1106_PRECHARGE,
        0xF1,

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
    for (const uint8_t& command : setup) {
        this->cmd(command);
    }

    // clear the buffer and send it to the display
    // if not done display shows garbage data
    this->clear();
    this->sendBuffer();
}

bool SH1106::IsConnected() {
    uint8_t data[2] = { 0x00, SH1106_DISPLAY_ON };
    int rc = i2c_write_timeout_us(this->i2CInst, this->address, data, 2, false, 50000);
    if (rc == PICO_ERROR_GENERIC || rc == PICO_ERROR_TIMEOUT) {
        return false;
    }
    return true;
}

void SH1106::setPixel(const uint8_t x, const uint8_t y, const WriteMode mode)
{
    // return if position out of bounds
    if (x >= this->width|| y >= this->height)
        return;

    // byte to be used for buffer operation
    auto byte = static_cast<uint8_t>(1U << (y & 7));

    // check the write mode and manipulate the frame buffer
    if (mode == WriteMode::ADD) {
        this->frameBuffer->byteOR(x + (y >> 3) * SH1106_MAX_WIDTH, byte);
    } else if (mode == WriteMode::SUBTRACT) {
        this->frameBuffer->byteAND(x + (y >> 3) * SH1106_MAX_WIDTH, ~byte);
    } else if (mode == WriteMode::INVERT) {
        this->frameBuffer->byteXOR(x + (y >> 3) * SH1106_MAX_WIDTH, byte);
    }
}

void SH1106::sendBuffer()
{
    const size_t pageCount = (this->height / 8);
    uint8_t pageBuffer[SH1106_PAGE_SIZE + 1];
    unsigned char displayShift = 2;

    this->cmd(SH1106_LOWCOLUMN | displayShift);
    this->cmd(SH1106_HIGHCOLUMN);
    this->cmd(SH1106_READ_MOD_WRITE);
    pageBuffer[0] = SH1106_STARTLINE;
    for (size_t currPage = 0; currPage < pageCount; currPage++) {
        this->cmd(static_cast<uint8_t>(SH1106_PAGEADDR | currPage));
        memcpy(pageBuffer + 1, frameBuffer->get() + (SH1106_PAGE_SIZE * currPage), SH1106_PAGE_SIZE);
        i2c_write_timeout_us(this->i2CInst, this->address, pageBuffer, SH1106_PAGE_SIZE + 1, false, 50000);
    }
    this->cmd(SH1106_END_WRITE);
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

void SH1106::cmd(const uint8_t& command)
{
    // 0x00 is a byte indicating to SH1106 that a command is being sent
    uint8_t data[2] = { 0x00, command };
    i2c_write_timeout_us(this->i2CInst, this->address, data, 2, false, 50000);
}

void SH1106::setContrast(const uint8_t contrast)
{
    this->cmd(SH1106_CONTRAST);
    this->cmd(contrast);
}

}
