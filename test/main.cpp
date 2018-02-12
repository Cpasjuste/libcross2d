//
// Created by cpasjuste on 08/12/16.
//

#include "c2d.h"
#include "main.h"

using namespace c2d;

int main() {

    // create main renderer
    Renderer *renderer = new C2DRenderer(Vector2f(SCR_W, SCR_H));
    renderer->setFillColor(Color::Black);

    // create a rect
    Rectangle *rect = new C2DRectangle(
            FloatRect(0, renderer->getSize().y / 2,
                      renderer->getSize().x / 2, renderer->getSize().y / 2));
    rect->setOriginCenter();
    rect->setFillColor(Color::Gray);
    rect->setOutlineColor(Color::Orange);
    rect->setOutlineThickness(8);

    // create a texture and add it to the rect
    Texture *tex = new C2DTexture(TEX_PATH);
    if (tex->available) {
        tex->setPosition(rect->getSize().x / 2, rect->getSize().y / 2);
        tex->setScale(0.5f, 0.5f);
        tex->setOriginCenter();
        rect->add(tex);
    }

    // create a font
    Font font;
    if (font.loadFromFile(FONT_PATH)) {
        // create a text and add it to the rect
        Text *text = new Text("Hello world", font);
        text->setOutlineColor(Color::Blue);
        text->setOutlineThickness(2);
        text->setOriginCenter();
        text->setPosition(rect->getSize().x / 2, 32);
        rect->add(text);
    }

    // add all this crap to the renderer
    renderer->add(rect);

    C2DClock clock;

    while (true) {

        float delta = renderer->getDeltaTime().asSeconds();
        printf("Time: %f (delta: %f)\n", clock.getElapsedTime().asSeconds(), delta);

        rect->move(100 * delta, 0);
        rect->setScale(rect->getScale().x - (0.1f * delta), rect->getScale().y - (0.1f * delta));
        rect->rotate(50 * delta);

        renderer->flip();

        if (rect->getPosition().x > renderer->getSize().x) {
            break;
        }
    }

    // will delete widgets recursively
    delete (renderer);

    return 0;
}
