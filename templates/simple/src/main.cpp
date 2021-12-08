//
// Created by cpasjuste on 08/12/2021.
//

#include "app.h"

using namespace c2d;

int main(int argc, char *argv[]) {

    // create main app/renderer
    auto app = new App(Vector2f(C2D_SCREEN_WIDTH, C2D_SCREEN_HEIGHT));

    while (!app->quit) {
        // renderer everything
        app->flip();
    }

    // will delete all child's (textures, shapes, text..)
    delete (app);

    return 0;
}