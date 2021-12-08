//
// Created by cpasjuste on 08/12/2021.
//

#include "app.h"

using namespace c2d;

App::App(const c2d::Vector2f &screenSize) : C2DRenderer(screenSize) {

    // add a simple rectangle
    rectangle = new C2DRectangle({128, 128});
    rectangle->setFillColor(Color::Orange);
    rectangle->setOutlineColor(Color::Red);
    rectangle->setOutlineThickness(2);
    rectangle->setOrigin(Origin::Center);
    rectangle->setPosition(screenSize.x / 2, screenSize.y / 2);
    App::add(rectangle);
}

// onInput is only called when a key is pressed
bool App::onInput(c2d::Input::Player *players) {

    // move cube
    if (players[0].keys & Input::Key::Up) {
        rectangle->move({0, -1});
    }
    if (players[0].keys & Input::Key::Down) {
        rectangle->move({0, 1});
    }
    if (players[0].keys & Input::Key::Left) {
        rectangle->move({-1, 0});
    }
    if (players[0].keys & Input::Key::Right) {
        rectangle->move({1, 0});
    }

    // quit app (enter and space on a keyboard)
    if (players[0].keys & Input::Key::Start || players[0].keys & Input::Key::Select) {
        quit = true;
    }

    return C2DRenderer::onInput(players);
}

// onUpdate is called every frames
void App::onUpdate() {
    C2DRenderer::onUpdate();
}
