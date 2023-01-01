#include "TextRenderer.h"

namespace pico_oled {

void drawText(pico_oled::OLED* oled, const unsigned char* font, const char* text, uint8_t anchor_x, uint8_t anchor_y, WriteMode mode, Rotation rotation)
{
    uint8_t font_width = font[0];

    uint16_t n = 0;
    while (text[n] != '\0') {
        switch (rotation) {
        case Rotation::deg0:
            drawChar(oled, font, text[n], anchor_x + (n * font_width), anchor_y, mode, rotation);
            break;
        case Rotation::deg90:
            drawChar(oled, font, text[n], anchor_x, anchor_y + (n * font_width), mode, rotation);
            break;
        }

        n++;
    }
}

void drawChar(pico_oled::OLED* oled, const unsigned char* font, char c, uint8_t anchor_x, uint8_t anchor_y, WriteMode mode, Rotation rotation)
{
    if (c < 32)
        return;

    uint8_t font_width = font[0];
    uint8_t font_height = font[1];

    uint16_t seek = (c - 32) * (font_width * font_height) / 8 + 2;

    uint8_t b_seek = 0;

    for (uint8_t x = 0; x < font_width; x++) {
        for (uint8_t y = 0; y < font_height; y++) {
            if (font[seek] >> b_seek & 0b00000001) {
                switch (rotation) {
                case Rotation::deg0:
                    oled->setPixel(x + anchor_x, y + anchor_y, mode);
                    break;
                case Rotation::deg90:
                    oled->setPixel(-y + anchor_x + font_height, x + anchor_y, mode);
                    break;
                }
            }
            b_seek++;
            if (b_seek == 8) {
                b_seek = 0;
                seek++;
            }
        }
    }
}
}