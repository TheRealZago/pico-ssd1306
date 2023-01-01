#ifndef OLED_TEXTRENDERER_H
#define OLED_TEXTRENDERER_H

#include "../oled.hpp"

#include "12x16_font.h"
#include "16x32_font.h"
#include "5x8_font.h"
#include "8x8_font.h"

namespace pico_oled {

/// \enum pico_oled::Rotation
enum class Rotation {
    /// deg0 - means no rotation
    deg0,
    /// deg 90 - means 90 deg rotation
    deg90,
};

/// \brief Draws a single glyph on the screen
/// \param oled - pointer to an OLED object aka initialised display
/// \param font - pointer to a font data array
/// \param c - char to be drawn
/// \param anchor_x, anchor_y - coordinates setting where to put the glyph
/// \param mode - mode describes setting behavior. See WriteMode doc for more information
/// \param rotation - either rotates the char by 90 deg or leaves it unrotated
void drawChar(pico_oled::OLED* oled, const unsigned char* font, char c, uint8_t anchor_x, uint8_t anchor_y, WriteMode mode = WriteMode::ADD, Rotation rotation = Rotation::deg0);

/// \brief Draws text on screen
/// \param oled - pointer to a OLED object aka initialised display
/// \param font - pointer to a font data array
/// \param text - text to be drawn
/// \param anchor_x, anchor_y - coordinates setting where to put the text
/// \param mode - mode describes setting behavior. See WriteMode doc for more information
/// \param rotation - either rotates the text by 90 deg or leaves it unrotated
void drawText(pico_oled::OLED* oled, const unsigned char* font, const char* text, uint8_t anchor_x, uint8_t anchor_y, WriteMode mode = WriteMode::ADD, Rotation rotation = Rotation::deg0);
}

#endif // OLED_TEXTRENDERER_H
