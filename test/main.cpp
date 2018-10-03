//
// Created by cpasjuste on 08/12/16.
//

#include "c2d.h"
#include "main.h"
#include <unistd.h>

#include "../res/default.h"
#include "../res/title.h"

using namespace c2d;

int main() {

    // create main renderer
    Renderer *renderer = new C2DRenderer(Vector2f(SCR_W, SCR_H));
    renderer->setFillColor(Color::Black);

    // create a rect
    Rectangle *rect = new C2DRectangle(
            FloatRect(renderer->getSize().x / 2, renderer->getSize().y / 2,
                      renderer->getSize().x / 2, renderer->getSize().y / 2));
    rect->setOriginCenter();
    rect->setFillColor(Color::Gray);
    rect->setOutlineColor(Color::Orange);
    rect->setOutlineThickness(8);

    // create a texture and add it to the rect
    Texture *tex = new C2DTexture((const unsigned char *) pfba_title, pfba_title_length);
    if (tex->available) {
        tex->setPosition(rect->getSize().x / 2, rect->getSize().y / 2);
        tex->setScale(0.5f, 0.5f);
        tex->setOriginCenter();
        rect->add(tex);
    }

    // create a font
    Font *font = new Font();
    if (font->loadFromMemory(pfba_font, pfba_font_length)) {
        // create a text and add it to the rect
        Text *text = new Text("Hello world", *font);
        text->setOutlineColor(Color::Blue);
        text->setOutlineThickness(2);
        text->setOriginCenter();
        text->setPosition(rect->getSize().x / 2, 32);
        rect->add(text);
    }

    // add all this crap to the renderer
    renderer->add(rect);

    Input *input = new C2DInput(nullptr);
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
    auto *tweenAlpha = new TweenAlpha(255, 0, 4.0f, TweenLoop::PingPong);
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
