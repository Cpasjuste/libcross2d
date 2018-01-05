//
// Created by cpasjuste on 08/12/16.
//

#include "c2d.h"
#include "main.h"

using namespace c2d;

int main() {

    // create main renderer
    Renderer *renderer = new C2DRenderer(Vector2f(SCR_W, SCR_H));

    // create a rect
    Rectangle *rect = new C2DRectangle(FloatRect(208, 208, 400, 400));
    rect->setOriginCenter();
    rect->setFillColor(Color::Gray);
    rect->setOutlineColor(Color::Orange);
    rect->setOutlineThickness(8);

    // create a texture and add it to the rect
    Texture *tex = new C2DTexture(TEX_PATH);
    if (tex->available) {
        tex->setPosition(rect->getSize().x / 2, rect->getSize().y / 2);
        tex->setOriginCenter();
        rect->add(tex);
    }

    // create a line and add it to the rect
    Line *line = new C2DLine(rect->getPoint(0), rect->getPoint(2), 2);
    line->setFillColor(Color::Blue);
    line->setOutlineColor(Color::Green);
    line->setOutlineThickness(1);
    rect->add(line);

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


    for (int i = 0; i < 10; i++) {

        if (i > 4) {
            rect->move(32, 0);
            rect->setScale(rect->getScale().x - 0.1f, rect->getScale().y - 0.1f);
            rect->rotate(5);
        }

        renderer->flip();
        renderer->delay(500);
    }

    // will delete widgets recursively
    delete (renderer);

    return 0;
}

#if 0
void print_keys(unsigned int state) {

    printf("state: %i ", state);
    if (state & Input::Key::KEY_DOWN)
        printf(" - DOWN");
    if (state & Input::Key::KEY_UP)
        printf(" - UP");
    if (state & Input::Key::KEY_LEFT)
        printf(" - LEFT");
    if (state & Input::Key::KEY_RIGHT)
        printf(" - RIGHT");
    printf("\n");
}

int main() {

    // renderer
    Renderer *renderer = (Renderer *) new C2DRenderer(SCR_W, SCR_H);

    // input
    Input *input = (Input *) new C2DInput(renderer);
    input->SetJoystickMapping(0, KEYS, 2000);
    //input->SetKeyboardMapping(KEYS);

    // font
    Font *font = (Font *) new C2DFont(renderer, FONT_PATH, 20);

    // texture
    Texture *texture = (Texture *) new C2DTexture(renderer, TEX_PATH);

    /*
    // test for fba rgb565 framebuffer (little endian)
    Texture *texture = (Texture *) new C2DTexture(renderer, 384, 224);
    FILE *pFile;
    pFile = fopen("/dev_hdd0/frame.bin", "rb");
    if (pFile == NULL) {
        printf("File error\n");
    }
    unsigned char *buffer;
    int pitch;
    texture->lock(Rect(), (void **) &buffer, &pitch);
    fread(buffer, 1, 384 * 224 * 2, pFile);
    fclose(pFile);
    texture->unlock();
    */

    while (true) {

        Input::Player player = input->Update(0)[0];
        if (player.state) {
            if (player.state & EV_QUIT) {
                break;
            }
            print_keys(player.state);
            renderer->Delay(100);
        }

        renderer->Clear();

        // window
        Rect rect = {0, 0, renderer->width - 1, renderer->height - 1};
        renderer->DrawRect(rect, C2D_COL_ORANGE, false);

        // inside "inside rect"
        rect.Scale(-1);
        renderer->DrawRect(rect, C2D_COL_GREEN, false);

        // "inside rect" inside border
        rect.Scale(-1);
        renderer->DrawRect(rect, C2D_COL_ORANGE, false);

        // inside/background rect
        rect.Scale(-1);
        renderer->DrawRect(rect, C2D_COL_GRAY_LIGHT);

        // screen centers
        renderer->DrawLine(0, rect.h / 2, rect.w, rect.h / 2); // X
        renderer->DrawLine(rect.w / 2, 0, rect.w / 2, rect.h); // Y

        // top middle text
        font->Draw(rect.w / 2, 0, "Hello World");

        // top left text
        font->Draw(0, 0, "Hello World");

        // centered text
        Rect r = {rect.w / 2, rect.h / 2, 0, 0};
        font->scaling = 2;
        r.w = font->GetWidth("Hello World");
        r.h = font->GetHeight("Hello World");
        r.x -= r.w / 2;
        r.y -= r.h / 2;
        renderer->DrawRect(r, C2D_COL_RED, false);
        font->Draw(r, C2D_COL_ORANGE, true, true, "Hello World Scaled");
        font->scaling = 1;

        // y centered and truncated text
        rect.x = 0;
        rect.y = 0;
        rect.w = 100;
        font->Draw(rect, C2D_COL_WHITE, false, true, "Hello World");

        texture->Draw(32, 32, texture->width, texture->height);

        renderer->Flip();
    }

    delete (input);
    delete (font);
    delete (texture);
    delete (renderer);
}

#endif
