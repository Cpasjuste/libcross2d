//
// Created by cpasjuste on 01/12/16.
//

#include "../../include/C2D.h"
#include "../../include/sfml/sfml_font.h"

SFMLFont::SFMLFont(Renderer *r, const char *p, int s) : Font(r, p, s) {

    if (!font.loadFromFile(path)) {
        printf("SFMLFont: load font error\n");
        size = 0;
        return;
    }
}

void SFMLFont::Draw(int x, int y, const char *fmt, ...) {

    if (size < 0) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    sf::Text text;
    text.setFont(font);
    text.setString(msg);
    text.setCharacterSize((unsigned int) size);

    text.setFillColor(sf::Color(
            color.r, color.g,
            color.b, color.a));
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);

    text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);
    text.scale(scaling, scaling);
    text.setPosition(x, y);

    ((SFMLRenderer *) renderer)->window.draw(text);
}

int SFMLFont::GetWidth(const char *fmt, ...) {

    if (size < 0) {
        return 0;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    sf::Text t;
    t.setFont(font);
    t.setCharacterSize((unsigned int) size);
    t.setString(msg);

    return (int) (scaling * (int) t.getLocalBounds().width + (int) (t.getLocalBounds().left / 2.0f));
}

int SFMLFont::GetHeight(const char *fmt, ...) {

    if (size < 0) {
        return 0;
    }

    // hack: return same height for every lines of text
    sf::Text t;
    t.setFont(font);
    t.setCharacterSize((unsigned int) size);
    t.setString("HpljA");

    int h = (int) ((t.getLocalBounds().height / 2) * (t.getLocalBounds().height / t.getLocalBounds().top));
    return (int) (h * scaling);
}
