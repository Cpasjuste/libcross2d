//
// Created by cpasjuste on 08/12/2021.
//

#include "app.h"

using namespace c2d;

App::App(const c2d::Vector2f &screenSize) : C2DRenderer(screenSize) {
    // add a simple rectangle
    rectangle = new C2DRectangle({screenSize.x - 32, screenSize.y - 32});
    rectangle->setFillColor(Color::Orange);
    rectangle->setOrigin(Origin::Center);
    rectangle->setPosition(screenSize.x / 2, screenSize.y / 2);
    App::add(rectangle);
}

bool App::onInput(c2d::Input::Player *players) {
    // exit on any key press
    if (players[0].keys != 0) {
        quit = true;
    }

    return C2DRenderer::onInput(players);
}

void App::onUpdate() {
    C2DRenderer::onUpdate();
}
