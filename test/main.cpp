//
// Created by cpasjuste on 08/12/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

int main(int argc, char *argv[]) {

    // create main renderer
    auto renderer = new C2DRenderer(Vector2f(C2D_SCREEN_WIDTH, C2D_SCREEN_HEIGHT));
    //renderer->setPrintStats(true);
    renderer->setClearColor(Color::Black);

#if 0
    auto *tex = new C2DTexture(renderer->getIo()->getRomFsPath() + "bird.png");
    auto *sprite = new AnimatedSprite(tex, {128, 128}, 4, 8, 1.5f);
    renderer->add(sprite);
#endif

#if 0
    auto border = new C2DRectangle({2, 2,
                                    renderer->getSize().x - 4, renderer->getSize().y - 4});
    border->setFillColor(Color::Transparent);
    border->setOutlineColor(Color::Red);
    border->setOutlineThickness(2);
    renderer->add(border);

    // scale font and texture for different resolution (devices),
    // based on 720p resolution
    float scaling = renderer->getSize().y / 720.0f;

    // create a rect
    auto rect = new C2DRectangle(
            {renderer->getSize().x / 2, renderer->getSize().y / 2,
             renderer->getSize().x / 2, renderer->getSize().y / 2});
    rect->setOrigin(Origin::Center);
    rect->setFillColor(Color::Gray);
    rect->setOutlineColor(Color::Orange);
    rect->setOutlineThickness(8 * scaling);

    // create a texture and add it to the rect
    auto tex = new C2DTexture(renderer->getIo()->getRomFsPath() + "gbatemp.png");
    if (tex->available) {
        tex->setPosition(rect->getSize().x / 2, rect->getSize().y / 2);
        tex->setScale(0.5f * scaling, 0.5f * scaling);
        tex->setOrigin(Origin::Center);
        rect->add(tex);
    }

    // create a text
    auto text = new Text("libcross2d @ Cpasjuste");
    text->setOutlineThickness(2);
    text->setPosition(rect->getSize().x - 16 * scaling, rect->getSize().y - 16 * scaling);
    text->setOrigin(Origin::BottomRight);
    text->setScale(scaling, scaling);
    rect->add(text);

    // add all this crap to the renderer
    renderer->add(rect);

    // add some tweening :)
    auto tweenPos = new TweenPosition(
            {renderer->getSize().x / 2 - (256 * scaling), rect->getPosition().y},
            {renderer->getSize().x / 2 + (256 * scaling), rect->getPosition().y},
            4.0f, TweenLoop::PingPong);
    tweenPos->play();
    rect->add(tweenPos);
    auto tweenRot = new TweenRotation(0, 360, 5.0f, TweenLoop::PingPong);
    tweenRot->play();
    rect->add(tweenRot);
    auto tweenScale = new TweenScale(rect->getScale(), {2.0f, 2.0f}, 3.0f, TweenLoop::PingPong);
    tweenScale->play();
    rect->add(tweenScale);
    auto tweenColor = new TweenColor(rect->getFillColor(), Color(0x9bc4e2ff), 3.0f, TweenLoop::PingPong);
    tweenColor->play();
    rect->add(tweenColor);
    auto tweenAlpha = new TweenAlpha(255, 200, 3.0f, TweenLoop::PingPong);
    tweenAlpha->play();
    rect->add(tweenAlpha);
#endif

#if 1
    auto *view = new Rectangle(renderer->getSize());
    view->setOrigin(Origin::BottomLeft);
    view->setPosition(0, renderer->getSize().y);
    view->setScale(0.5f, 0.5f);
    renderer->add(view);

    auto *world = new PhysicsWorld();
    view->add(world);

    auto shape = new C2DTexture(renderer->getIo()->getRomFsPath() + "gbatemp.png");
    shape->setPosition(55, 200);
    shape->setSize(100, 100);
    shape->addPhysicsBody(world, b2_dynamicBody, 1.0f, 0.3f);
    world->add(shape);

    auto shape2 = new RectangleShape({0, 0, 100, 100});
    shape2->setFillColor(Color::Yellow);
    shape2->addPhysicsBody(world, b2_staticBody, 1.0f, 0.3f);
    world->add(shape2);

    auto shape3 = new RectangleShape({170, 0, 100, 100});
    shape3->setFillColor(Color::Yellow);
    shape3->addPhysicsBody(world, b2_staticBody, 1.0f, 0.3f);
    world->add(shape3);
#endif

    while (true) {

        //printf("%f x %f\n", shape->getPosition().x, shape->getPosition().y);

        // stop if any key is pressed
        if (renderer->getInput()->getKeys() != 0) {
            if (renderer->getInput()->getKeys() == 32768) {
                //shape->getPhysicsBody()->ApplyForce({0, -500}, shape->getPhysicsBody()->GetWorldCenter(), true);
            } else {
                break;
            }
        }

        // renderer everything
        renderer->flip();
    }

    // will delete child's (textures, shapes, text..)
    delete (renderer);

    return 0;
}
