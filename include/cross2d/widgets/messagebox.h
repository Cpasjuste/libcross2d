//
// Created by cpasjuste on 05/02/18.
//

#ifndef C2D_MESSAGEBOX_H
#define C2D_MESSAGEBOX_H

#ifdef __WINDOWS__
#undef MessageBox
#endif

namespace c2d {

    class MessageBox : public RectangleShape {

    public:

        enum MessageBoxResult : int {
            LEFT = 0, RIGHT = 1, CANCEL = 2, TIMEOUT = 3
        };

        MessageBox(const c2d::FloatRect &rect, c2d::Input *input,
                   c2d::Font *font, int fontSize);

        // if "pressed" is supplied, pressed is set to the pressed key..
        int show(const std::string &title, const std::string &message,
                 const std::string &buttonLeftText = "", const std::string &buttonRightText = "",
                 int *pressed = nullptr, int timeout = 0);

        void hide();

        void setFillColor(const Color &color) override;

        void setOutlineColor(const Color &color) override;

        c2d::Text *getTitleText();

        c2d::Text *getMessageText();

    private:

        c2d::Text *title = nullptr;
        c2d::Text *message = nullptr;
        c2d::Text *timeout = nullptr;
        char timeout_str[16];
        c2d::Button *buttons[2];
        c2d::Input *input = nullptr;
    };
}

#endif //C2D_MESSAGEBOX_H
