//
// Created by cpasjuste on 05/02/18.
//

#ifndef CROSS2D_MESSAGEBOX_H
#define CROSS2D_MESSAGEBOX_H

namespace c2d {

    class MessageBox : public c2d::Rectangle {

    public:

        enum MessageBoxResult : int {
            LEFT = 0, RIGHT = 1, CANCEL = 2, TIMEOUT = 3
        };

        MessageBox(const c2d::FloatRect &rect, c2d::Input *input,
                   c2d::Font &font, int fontSize);

        int show(const std::string &title, const std::string &message,
                 const std::string &buttonLeftText = "", const std::string &buttonRightText = "",
                 int *pressed = NULL,
                 int timeout = 0); // if "pressed" is supplied, pressed is set to the pressed key..

        void setFillColor(const Color &color);

        void setOutlineColor(const Color &color);

    private:

        c2d::Text *title = NULL;
        c2d::Text *message = NULL;
        c2d::Text *timeout = NULL;
        char timeout_str[16];
        c2d::Button *buttons[2];
        c2d::Input *input = NULL;
    };
}

#endif //CROSS2D_MESSAGEBOX_H
