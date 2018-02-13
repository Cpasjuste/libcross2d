//
// Created by cpasjuste on 05/02/18.
//

#ifndef CROSS2D_BUTTON_H
#define CROSS2D_BUTTON_H

namespace c2d {

    class Button : public c2d::Rectangle {

    public:

        Button(const c2d::FloatRect &rect, c2d::Font &font,
               int fontSize, const std::string &text);

        void setText(const std::string &text);

    private:

        c2d::Text *text;
    };
}

#endif //CROSS2D_BUTTON_H
