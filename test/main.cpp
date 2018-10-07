//
// Created by cpasjuste on 08/12/16.
//

#include "cross2d/c2d.h"
#include "main.h"

#include "../res/default.h"
#include "../res/title.h"

using namespace c2d;

int main() {

    // create main renderer
    auto *renderer = new C2DRenderer(Vector2f(SCR_W, SCR_H));
    renderer->setFillColor(Color::Black);

    // create a rect
    auto *rect = new C2DRectangle(
            {renderer->getSize().x / 2, renderer->getSize().y / 2,
             renderer->getSize().x / 2, renderer->getSize().y / 2});

    rect->setOrigin(Origin::Center);
    rect->setFillColor(Color::Gray);
    rect->setOutlineColor(Color::Orange);
    rect->setOutlineThickness(8);

    // create a texture and add it to the rect
    auto *tex = new C2DTexture((const unsigned char *) pfba_title, pfba_title_length);
    if (tex->available) {
        tex->setPosition(rect->getSize().x / 2, rect->getSize().y / 2);
        tex->setScale(0.5f, 0.5f);
        tex->setOrigin(Origin::Center);
        rect->add(tex);
    }

    // create a font
    auto *font = new C2DFont();
    if (font->loadFromMemory(pfba_font, pfba_font_length)) {
        // some font have too much "margin"..
        font->setOffset({0, -10});
        // create a text and add it to the rect
        auto *text = new C2DText("Hello world", *font);
        text->setOutlineColor(Color::Blue);
        text->setOutlineThickness(2);
        text->setPosition(rect->getSize().x / 2, 32);
        text->setOrigin(Origin::Center);
        rect->add(text);
    }

    // add all this crap to the renderer
    renderer->add(rect);

    auto *input = new C2DInput(nullptr);
    input->setJoystickMapping(0, KEYS, 0);

    // add some tweening :)
    auto *tweenPos = new TweenPosition(
            {renderer->getSize().x / 2 - 256, rect->getPosition().y},
            {renderer->getSize().x / 2 + 256, rect->getPosition().y},
            4.0f, TweenLoop::PingPong);
    rect->add(tweenPos);
    auto *tweenRot = new TweenRotation(0, 360, 4.0f, TweenLoop::PingPong);
    rect->add(tweenRot);
    auto *tweenScale = new TweenScale(rect->getScale(), {2, 2}, 4.0f, TweenLoop::PingPong);
    rect->add(tweenScale);
    auto *tweenColor = new TweenColor(rect->getFillColor(), Color::Orange, 4.0f, TweenLoop::PingPong);
    rect->add(tweenColor);
    auto *tweenAlpha = new TweenAlpha(255, 100, 4.0f, TweenLoop::PingPong);
    rect->add(tweenAlpha);

    while (renderer->getElapsedTime().asSeconds() < 20) {

        // handle input
        unsigned int key = input->update()[0].state;
        if (key) {
            printf("input[0]: 0x%08X\n", key);
            if (key & EV_QUIT) {
                break;
            }
        }

        // time / delta time
        float delta = renderer->getDeltaTime().asSeconds();
        printf("Time: %f (delta: %f), fps: %2g\n",
               renderer->getElapsedTime().asSeconds(), delta, renderer->getFps());

        // renderer everything
        renderer->flip();
    }

    // will delete widgets recursively
    delete (input);
    delete (font);
    delete (renderer);

    return 0;
}
