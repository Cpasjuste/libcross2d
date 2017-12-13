//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SFML_FONT_H_
#define _SFML_FONT_H_

#include "../skeleton/renderer.h"
#include "../skeleton/font.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class SFMLFont : Font {

public:
    SFMLFont(Renderer *renderer, const char *path, int size);

    void Draw(int x, int y, const char *fmt, ...);

    virtual int GetWidth(const char *fmt, ...);

    virtual int GetHeight(const char *fmt, ...);

private:
    sf::Font font;
};

#endif //_SFML_FONT_H_
