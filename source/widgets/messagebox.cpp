//
// Created by cpasjuste on 05/02/18.
//

#include "c2d.h"

using namespace c2d;

MessageBox::MessageBox(const c2d::FloatRect &rect, c2d::Input *input,
                       c2d::Font &font, int fontSize)
        : Rectangle(rect) {

    this->input = input;
    float line_height = font.getLineSpacing((unsigned int) fontSize) + 16;

    this->title = new Text("TITLE", font, (unsigned int) fontSize);
    this->title->setScale(1.5f, 1.5f);
    this->title->setOutlineColor(Color::Black);
    this->title->setOutlineThickness(1);
    this->title->setSizeMax(getSize().x - 16, getSize().y - 8);
    this->title->setPosition(getSize().x / 2, line_height);
    this->title->setOriginCenter();
    add(this->title);

    this->message = new Text("MESSAGE", font, (unsigned int) fontSize);
    this->message->setOutlineColor(Color::Black);
    this->message->setOutlineThickness(1);
    this->message->setSizeMax(getSize().x - 16, getSize().y - 8);
    this->message->setPosition(getSize().x / 2, getSize().y * 0.45f);
    this->message->setOriginCenter();
    this->message->setLineSpacingModifier(4);
    add(this->message);

    for (int i = 0; i < 2; i++) {
        buttons[i] = new Button(
                FloatRect(0, 0, getSize().x / 3, getSize().y / 5),
                font, fontSize, "BUTTON");
        buttons[i]->setOutlineThickness(1);
        buttons[i]->setOrigin(buttons[i]->getSize().x / 2, 0);
        add(buttons[i]);
    }

    this->timeout = new Text("9", font, (unsigned int) fontSize);
    this->timeout->setOutlineColor(Color::Black);
    this->timeout->setOutlineThickness(1);
    this->timeout->setSizeMax(getSize().x - 16, getSize().y - 8);
    this->timeout->setPosition(getSize().x / 2, getSize().y - buttons[0]->getSize().y - 16);
    this->timeout->setOriginCenter();
    this->timeout->setLineSpacingModifier(4);
    add(this->timeout);

    this->setVisibility(Hidden);
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
    this->title->setOriginCenter();
    this->message->setString(message);
    this->message->setOriginCenter();
    this->timeout->setVisibility(timeout > 0 ? Visible : Hidden);

    // buttons
    if (!buttonLeftText.empty() || !buttonRightText.empty()) {

        buttons[0]->setFillColor(getOutlineColor());
        buttons[0]->setOutlineColor(Color::White);

        if (buttonRightText.empty()) {
            buttons[0]->setText(buttonLeftText);
            buttons[0]->setPosition(getSize().x / 2, getSize().y - buttons[0]->getSize().y - 16);
            buttons[0]->setVisibility(Visible);
            buttons[1]->setVisibility(Hidden);
            choices = 1;
        } else {
            buttons[0]->setText(buttonLeftText);
            buttons[0]->setPosition((getSize().x / 3) - 8,
                                    getSize().y - buttons[0]->getSize().y - 16);
            buttons[0]->setVisibility(Visible);
            buttons[1]->setText(buttonRightText);
            buttons[1]->setPosition(((getSize().x / 3) * 2) + 8,
                                    getSize().y - buttons[1]->getSize().y - 16);
            buttons[1]->setFillColor(getFillColor());
            buttons[1]->setVisibility(Visible);
            choices = 2;
        }
    } else {
        buttons[0]->setVisibility(Hidden);
        buttons[1]->setVisibility(Hidden);
    }

    setVisibility(Visible);
    setLayer(1000);

    input->clear(0);

    clock.restart();

    while (true) {

        if (timeout > 0) {
            int elapsed = (int) clock.getElapsedTime().asSeconds();
            if (elapsed >= timeout) {
                setVisibility(Hidden);
                input->clear(0);
                ret = TIMEOUT;
                break;
            }
            snprintf(timeout_str, 16, "%i", timeout - elapsed);
            this->timeout->setString(timeout_str);
            this->timeout->setOriginCenter();
        }

        if (pressed) {
            key = input->getButton();
            if (key > -1) {
                setVisibility(Hidden);
                input->clear(0);
                break;
            }
        } else {
            key = input->update(0)[0].state;
            if (key > 0) {

                if (key & Input::Key::KEY_LEFT) {
                    if (index > 0) {
                        index--;
                    }
                } else if (key & Input::Key::KEY_RIGHT) {
                    if (index < choices - 1) {
                        index++;
                    }
                } else if (key & Input::Key::KEY_FIRE1) {
                    setVisibility(Hidden);
                    input->clear(0);
                    ret = index == 1 ? RIGHT : LEFT;
                    break;
                } else if (key & Input::Key::KEY_FIRE2) {
                    setVisibility(Hidden);
                    input->clear(0);
                    ret = CANCEL;
                    break;
                }

                if (choices) {
                    setFillColor(getFillColor());
                    setOutlineColor(getOutlineColor());
                    buttons[index]->setFillColor(getOutlineColor());
                    buttons[index]->setOutlineColor(Color::White);
                }

                c2d_renderer->delay(150);
            }
        }

        c2d_renderer->flip();
    }

    if (pressed) {
        *pressed = key;
    }

    return ret;
}
