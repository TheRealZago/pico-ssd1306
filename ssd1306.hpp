#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

#include "oled.hpp"

namespace pico_oled {

/// \class SSD1306 ssd1306.h "pico-ssd1306/ssd1306.h"
/// \brief SSD1306 class represents i2c connection to display
class SSD1306 : public OLED {
private:
    /// Register addresses from datasheet
    enum REG_ADDRESSES : uint8_t {
        SSD1306_CONTRAST = 0x81,
        SSD1306_DISPLAYALL_ON_RESUME = 0xA4,
        SSD1306_DISPLAYALL_ON = 0xA5,
        SSD1306_INVERTED_OFF = 0xA6,
        SSD1306_INVERTED_ON = 0xA7,
        SSD1306_DISPLAY_OFF = 0xAE,
        SSD1306_DISPLAY_ON = 0xAF,
        SSD1306_DISPLAYOFFSET = 0xD3,
        SSD1306_COMPINS = 0xDA,
        SSD1306_VCOMDETECT = 0xDB,
        SSD1306_DISPLAYCLOCKDIV = 0xD5,
        SSD1306_PRECHARGE = 0xD9,
        SSD1306_MULTIPLEX = 0xA8,
        SSD1306_LOWCOLUMN = 0x00,
        SSD1306_HIGHCOLUMN = 0x10,
        SSD1306_STARTLINE = 0x40,
        SSD1306_MEMORYMODE = 0x20,
        SSD1306_MEMORYMODE_HORZONTAL = 0x00,
        SSD1306_MEMORYMODE_VERTICAL = 0x01,
        SSD1306_MEMORYMODE_PAGE = 0x10,
        SSD1306_COLUMNADDR = 0x21,
        SSD1306_PAGEADDR = 0x22,
        SSD1306_COM_REMAP_OFF = 0xC0,
        SSD1306_COM_REMAP_ON = 0xC8,
        SSD1306_CLUMN_REMAP_OFF = 0xA0,
        SSD1306_CLUMN_REMAP_ON = 0xA1,
        SSD1306_CHARGEPUMP = 0x8D,

        SSD1306_EXTERNALVCC = 0x1,
        SSD1306_SWITCHCAPVCC = 0x2,
    };

protected:
    void cmd(uint8_t command) override;

public:
    /// \brief SSD1306 constructor initialized display and sets all required registers for operation
    /// \param i2CInst - i2c instance. Either i2c0 or i2c1
    /// \param Address - display i2c address. usually for 128x32 0x3C and for 128x64 0x3D
    /// \param size - display size. Acceptable values W128xH32 or W128xH64
    SSD1306(i2c_inst* i2CInst, uint16_t Address, Size size);

    bool IsConnected() override;
    void setPixel(int16_t x, int16_t y, WriteMode mode) override;
    void sendBuffer() override;
    void setOrientation(bool orientation) override;
    void invertDisplay() override;
    void setContrast(unsigned char contrast) override;
};

}

#endif // OLED_SSD1306_H