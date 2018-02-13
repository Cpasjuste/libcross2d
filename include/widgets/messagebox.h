//
// Created by cpasjuste on 05/02/18.
//

#ifndef CROSS2D_MESSAGEBOX_H
#define CROSS2D_MESSAGEBOX_H

#define C2D_MESSAGEBOX_CONFIRM  0
#define C2D_MESSAGEBOX_CANCEL   1

namespace c2d {

    class MessageBox : public c2d::Rectangle {

    public:

        MessageBox(const c2d::FloatRect &rect, c2d::Input *input,
                   c2d::Font &font, int fontSize);

        int show(const std::string &title, const std::string &message,
                 const std::string &confirmText, const std::string &cancelText = "");

        void setFillColor(const Color &color);

        void setOutlineColor(const Color &color);

    private:

        c2d::Text *title = NULL;
        c2d::Text *message = NULL;
        c2d::Button *buttons[2];
        c2d::Input *input = NULL;
    };
}

#endif //CROSS2D_MESSAGEBOX_H
