////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include "skeleton/c2d_sfml/Color.hpp"
#include <algorithm>

namespace c2d {
////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////
    const Color Color::Black(0, 0, 0);
    const Color Color::White(255, 255, 255);
    const Color Color::Red(255, 0, 0);
    const Color Color::Green(0, 255, 0);
    const Color Color::Blue(0, 0, 255);
    const Color Color::Yellow(255, 255, 0);
    const Color Color::Magenta(255, 0, 255);
    const Color Color::Cyan(0, 255, 255);
    const Color Color::Orange(255, 165, 0);
    const Color Color::Gray(55, 55, 55);
    const Color Color::GrayLight(40, 40, 40);
    const Color Color::Transparent(0, 0, 0, 0);


////////////////////////////////////////////////////////////
    Color::Color() :
            r(0),
            g(0),
            b(0),
            a(255) {

    }


////////////////////////////////////////////////////////////
    Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) :
            r(red),
            g(green),
            b(blue),
            a(alpha) {

    }


////////////////////////////////////////////////////////////
    Color::Color(uint32_t color) :
            r((color & 0xff000000) >> 24),
            g((color & 0x00ff0000) >> 16),
            b((color & 0x0000ff00) >> 8),
            a((color & 0x000000ff) >> 0) {

    }


////////////////////////////////////////////////////////////
    uint32_t Color::toRGBA() const {
        return (r << 24) | (g << 16) | (b << 8) | a;
    }

    uint32_t Color::toARGB() const {
        return (a << 24) | (r << 16) | (g << 8) | b;
    }


////////////////////////////////////////////////////////////
    bool operator==(const Color &left, const Color &right) {
        return (left.r == right.r) &&
               (left.g == right.g) &&
               (left.b == right.b) &&
               (left.a == right.a);
    }


////////////////////////////////////////////////////////////
    bool operator!=(const Color &left, const Color &right) {
        return !(left == right);
    }


////////////////////////////////////////////////////////////
    Color operator+(const Color &left, const Color &right) {
        return Color(uint8_t(std::min(int(left.r) + right.r, 255)),
                     uint8_t(std::min(int(left.g) + right.g, 255)),
                     uint8_t(std::min(int(left.b) + right.b, 255)),
                     uint8_t(std::min(int(left.a) + right.a, 255)));
    }


////////////////////////////////////////////////////////////
    Color operator-(const Color &left, const Color &right) {
        return Color(uint8_t(std::max(int(left.r) - right.r, 0)),
                     uint8_t(std::max(int(left.g) - right.g, 0)),
                     uint8_t(std::max(int(left.b) - right.b, 0)),
                     uint8_t(std::max(int(left.a) - right.a, 0)));
    }


////////////////////////////////////////////////////////////
    Color operator*(const Color &left, const Color &right) {
        return Color(uint8_t(int(left.r) * right.r / 255),
                     uint8_t(int(left.g) * right.g / 255),
                     uint8_t(int(left.b) * right.b / 255),
                     uint8_t(int(left.a) * right.a / 255));
    }


////////////////////////////////////////////////////////////
    Color &operator+=(Color &left, const Color &right) {
        return left = left + right;
    }


////////////////////////////////////////////////////////////
    Color &operator-=(Color &left, const Color &right) {
        return left = left - right;
    }


////////////////////////////////////////////////////////////
    Color &operator*=(Color &left, const Color &right) {
        return left = left * right;
    }

} // namespace sf
