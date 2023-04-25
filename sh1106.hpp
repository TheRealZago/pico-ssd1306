#ifndef OLED_SH1106_H
#define OLED_SH1106_H

#include "oled.hpp"

namespace pico_oled {

/// \class SH1106 sh1106.h "pico-oled/sh1106.hpp"
/// \brief SH1106 class represents i2c connection to display
class SH1106 : public OLED {
private:
    /// Register addresses from datasheet
    enum REG_ADDRESSES : uint8_t {
        SH1106_LOWCOLUMN = 0x00,
        SH1106_HIGHCOLUMN = 0x10,
        SH1106_STARTLINE = 0x40,
        SH1106_CONTRAST = 0x81,
        SH1106_CLUMN_REMAP_OFF = 0xA0,
        SH1106_CLUMN_REMAP_ON = 0xA1,
        SH1106_DISPLAYALL_ON_RESUME = 0xA4,
        SH1106_DISPLAYALL_ON = 0xA5,
        SH1106_INVERTED_OFF = 0xA6,
        SH1106_INVERTED_ON = 0xA7,
        SH1106_MULTIPLEX = 0xA8,
        SH1106_DISPLAY_OFF = 0xAE,
        SH1106_DISPLAY_ON = 0xAF,
        SH1106_PAGEADDR = 0xB0,
        SH1106_COM_REMAP_OFF = 0xC0,
        SH1106_COM_REMAP_ON = 0xC8,
        SH1106_DISPLAYOFFSET = 0xD3,
        SH1106_DISPLAYCLOCKDIV = 0xD5,
        SH1106_PRECHARGE = 0xD9,
        SH1106_COMPINS = 0xDA,
        SH1106_VCOMDETECT = 0xDB,
        SH1106_READ_MOD_WRITE = 0xE0,
        SH1106_END_WRITE = 0xEE,

        SH1106_CHARGEPUMP = 0x30,
        SH1106_CHARGEPUMP_6V4 = 0x0,
        SH1106_CHARGEPUMP_7V4 = 0x1,
        SH1106_CHARGEPUMP_8V0 = 0x2,
        SH1106_CHARGEPUMP_9V0 = 0x3,
    };

protected:
    void cmd(const uint8_t& command) final;

public:
    /// \brief SH1106 constructor initialized display and sets all required registers for operation
    /// \param i2CInst - i2c instance. Either i2c0 or i2c1
    /// \param Address - display i2c address. usually for 128x32 0x3C and for 128x64 0x3D
    /// \param size - display size. Acceptable values W128xH32 or W128xH64
    SH1106(i2c_inst* i2CInst, uint8_t Address, Size size);

    bool IsConnected() final;
    void setPixel(const uint8_t x, const uint8_t y, const WriteMode mode) final;
    void sendBuffer() final;
    void setOrientation(bool orientation) final;
    void invertDisplay() final;
    void setContrast(const uint8_t contrast) final;
};

}

#endif // OLED_SH1106_H