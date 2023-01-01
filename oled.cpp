#include "oled.hpp"

namespace pico_oled {

OLED::OLED(i2c_inst* i2CInst, uint16_t Address, Type type, Size size)
{
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

}
