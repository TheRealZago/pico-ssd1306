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
    };

protected:
    void cmd(uint8_t command) override;

public:
    /// \brief SH1106 constructor initialized display and sets all required registers for operation
    /// \param i2CInst - i2c instance. Either i2c0 or i2c1
    /// \param Address - display i2c address. usually for 128x32 0x3C and for 128x64 0x3D
    /// \param size - display size. Acceptable values W128xH32 or W128xH64
    SH1106(i2c_inst* i2CInst, uint16_t Address, Size size);

    void setPixel(int16_t x, int16_t y, WriteMode mode) override;
    void sendBuffer() override;
    void setOrientation(bool orientation) override;
    void invertDisplay() override;
    void setContrast(unsigned char contrast) override;
};

}

#endif // OLED_SH1106_H