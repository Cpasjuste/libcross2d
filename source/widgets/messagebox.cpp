//
// Created by cpasjuste on 05/02/18.
//

#include "cross2d/c2d.h"

#ifdef __WINDOWS__
#undef MessageBox
#endif

using namespace c2d;

MessageBox::MessageBox(const c2d::FloatRect &rect, c2d::Input *input,
                       c2d::Font *font, int fontSize)
        : RectangleShape(rect) {

    this->input = input;
    float line_height = font->getLineSpacing((unsigned int) fontSize) + 16;

    this->title = new Text("TITLE", (unsigned int) ((float) fontSize * 1.5f), font);
    this->title->setOutlineColor(Color::Black);
    this->title->setOutlineThickness(1);
    this->title->setWidth(getSize().x - 16);
    this->title->setPosition(getSize().x / 2, line_height);
    this->title->setOrigin(Origin::Center);
    add(this->title);

    this->message = new Text("MESSAGE", (unsigned int) fontSize, font);
    this->message->setOutlineColor(Color::Black);
    this->message->setOutlineThickness(1);
    this->message->setWidth(getSize().x - 16);
    this->message->setPosition(getSize().x / 2, getSize().y * 0.45f);
    this->message->setOrigin(Origin::Center);
    this->message->setLineSpacingModifier(4);
    add(this->message);

    for (int i = 0; i < 2; i++) {
        buttons[i] = new Button(
                FloatRect(0, 0, getSize().x / 3, getSize().y / 5),
                font, fontSize, "BUTTON");
        buttons[i]->setOutlineThickness(1);
        buttons[i]->setOrigin(Origin::Top);
        add(buttons[i]);
    }

    this->timeout = new Text("9", (unsigned int) fontSize, font);
    this->timeout->setOutlineColor(Color::Black);
    this->timeout->setOutlineThickness(1);
    this->timeout->setWidth(getSize().x - 16);
    this->timeout->setPosition(getSize().x / 2, getSize().y - buttons[0]->getSize().y - 16);
    this->timeout->setOrigin(Origin::Center);
    this->timeout->setLineSpacingModifier(4);
    add(this->timeout);

    this->setVisibility(Visibility::Hidden);
}

void MessageBox::setFillColor(const Color &color) {
    Shape::setFillColor(color);
    Color c = {(uint8_t) (color.r * 0.7f),
               (uint8_t) (color.b * 0.7f),
               (uint8_t) (color.b * 0.7f),
               (uint8_t) (color.a)};
    buttons[0]->setFillColor(c);
    buttons[1]->setFillColor(c);
}

void MessageBox::setOutlineColor(const Color &color) {
    Shape::setOutlineColor(color);
    title->setFillColor(color);
    buttons[0]->setOutlineColor(color);
    buttons[1]->setOutlineColor(color);
}

int MessageBox::show(const std::string &title, const std::string &message,
                     const std::string &buttonLeftText, const std::string &buttonRightText,
                     int *pressed, int timeout) { // if "pressed" is supplied, pressed is set to the pressed key..

    int ret = 0, index = 0, choices = 0;
    int key = 0;
    C2DClock clock;

    this->title->setString(title);
    this->title->setOrigin(Origin::Center);
    this->message->setString(message);
    this->message->setOrigin(Origin::Center);
    this->timeout->setVisibility(timeout > 0 ? Visibility::Visible : Visibility::Hidden);

    // buttons
    if (!buttonLeftText.empty() || !buttonRightText.empty()) {

        buttons[0]->setFillColor(getOutlineColor());
        buttons[0]->setOutlineColor(Color::White);

        if (buttonRightText.empty()) {
            buttons[0]->setText(buttonLeftText);
            buttons[0]->setPosition(getSize().x / 2, getSize().y - buttons[0]->getSize().y - 16);
            buttons[0]->setVisibility(Visibility::Visible);
            buttons[1]->setVisibility(Visibility::Hidden);
            choices = 1;
        } else {
            buttons[0]->setText(buttonLeftText);
            buttons[0]->setPosition((getSize().x / 3) - 8,
                                    getSize().y - buttons[0]->getSize().y - 16);
            buttons[0]->setVisibility(Visibility::Visible);
            buttons[1]->setText(buttonRightText);
            buttons[1]->setPosition(((getSize().x / 3) * 2) + 8,
                                    getSize().y - buttons[1]->getSize().y - 16);
            buttons[1]->setFillColor(getFillColor());
            buttons[1]->setVisibility(Visibility::Visible);
            choices = 2;
        }
    } else {
        buttons[0]->setVisibility(Visibility::Hidden);
        buttons[1]->setVisibility(Visibility::Hidden);
    }

    setVisibility(Visibility::Visible);
    setLayer(10000);

    if (choices > 0) {

        input->clear(0);
        clock.restart();

        while (true) {

            if (timeout > 0) {
                int elapsed = (int) clock.getElapsedTime().asSeconds();
                if (elapsed >= timeout) {
                    setVisibility(Visibility::Hidden);
                    ret = TIMEOUT;
                    break;
                }
                snprintf(timeout_str, 16, "%i", timeout - elapsed);
                this->timeout->setString(timeout_str);
                this->timeout->setOrigin(Origin::Center);
            }

            if (pressed) {
                key = input->waitButton();
                if (key > -1) {
                    setVisibility(Visibility::Hidden);
                    break;
                }
            } else {
                key = input->getKeys();
                if (key > 0 && key != Input::Key::Delay) {
                    if (key & Input::Key::Left) {
                        if (index > 0) {
                            index--;
                        }
                    } else if (key & Input::Key::Right) {
                        if (index < choices - 1) {
                            index++;
                        }
                    } else if (key & Input::Key::Fire1) {
                        setVisibility(Visibility::Hidden);
                        ret = index == 1 ? RIGHT : LEFT;
                        break;
                    } else if (key & Input::Key::Fire2) {
                        setVisibility(Visibility::Hidden);
                        ret = CANCEL;
                        break;
                    }

                    setFillColor(getFillColor());
                    setOutlineColor(getOutlineColor());
                    buttons[index]->setFillColor(getOutlineColor());
                    buttons[index]->setOutlineColor(Color::White);
                }
            }

            c2d_renderer->flip();
        }

        if (pressed) {
            *pressed = key;
        }
    } else {
        c2d_renderer->flip();
    }

    return ret;
}

void MessageBox::hide() {
    setVisibility(Visibility::Hidden);
}

c2d::Text *MessageBox::getTitleText() {
    return title;
}

c2d::Text *MessageBox::getMessageText() {
    return message;
}
