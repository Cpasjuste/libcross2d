//
// Created by cpasjuste on 08/12/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

int main(int argc, char **argv) {

    // create main renderer
    auto *renderer = new C2DRenderer(Vector2f(C2D_SCREEN_WIDTH, C2D_SCREEN_HEIGHT));
    renderer->setClearColor(Color::Black);

    // scale font and texture for different resolution (devices),
    // based on 720p resolution
    float scaling = renderer->getSize().y / 720.0f;

    // create a rect
    auto *rect = new C2DRectangle(
            {renderer->getSize().x / 2, renderer->getSize().y / 2,
             renderer->getSize().x / 2, renderer->getSize().y / 2});

    rect->setOrigin(Origin::Center);
    rect->setFillColor(Color::Gray);
    rect->setOutlineColor(Color::Orange);
    rect->setOutlineThickness(8 * scaling);

    // create a texture and add it to the rect
    auto *tex = new C2DTexture(renderer->getIo()->getDataReadPath() + "gbatemp.png");
    if (tex->available) {
        tex->setPosition(rect->getSize().x / 2, rect->getSize().y / 2);
        tex->setScale(0.5f * scaling, 0.5f * scaling);
        tex->setOrigin(Origin::Center);
        rect->add(tex);
    }

    // create a font
    auto *text = new C2DText("libcross2d @ Cpasjuste");
    text->setOutlineThickness(2);
    text->setPosition(rect->getSize().x - 16 * scaling, rect->getSize().y - 16 * scaling);
    text->setOrigin(Origin::BottomRight);
    text->setScale(scaling, scaling);
    rect->add(text);

    // add all this crap to the renderer
    renderer->add(rect);

    // add some tweening :)
    auto *tweenPos = new TweenPosition(
            {renderer->getSize().x / 2 - (256 * scaling), rect->getPosition().y},
            {renderer->getSize().x / 2 + (256 * scaling), rect->getPosition().y},
            4.0f, TweenLoop::PingPong);
    tweenPos->play();
    rect->add(tweenPos);
    auto *tweenRot = new TweenRotation(0, 360, 5.0f, TweenLoop::PingPong);
    tweenRot->play();
    rect->add(tweenRot);
    auto *tweenScale = new TweenScale(rect->getScale(), {2.0f, 2.0f}, 3.0f, TweenLoop::PingPong);
    tweenScale->play();
    rect->add(tweenScale);
    auto *tweenColor = new TweenColor(rect->getFillColor(), Color(0x9bc4e2ff), 3.0f, TweenLoop::PingPong);
    tweenColor->play();
    rect->add(tweenColor);
    auto *tweenAlpha = new TweenAlpha(255, 200, 3.0f, TweenLoop::PingPong);
    tweenAlpha->play();
    rect->add(tweenAlpha);

    while (true) {

        // stop if any key is pressed
        if (renderer->getInput()->getKeys()) {
            break;
        }

        // time / delta time
        //float delta = renderer->getDeltaTime().asSeconds();
        //printf("Time: %f (delta: %f), fps: %2g\n",
        //       renderer->getElapsedTime().asSeconds(), delta, renderer->getFps());

        // renderer everything
        renderer->flip();
    }

    // will delete child's (textures, shapes, text..)
    delete (renderer);

    return 0;
}
