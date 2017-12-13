//
// Created by cpasjuste on 08/12/16.
//

#include "C2D.h"

#include "main.h"

using namespace C2D;

int main() {


    Renderer *renderer = new C2DRenderer(0, 0, SCR_W, SCR_H);

    C2DRectangle *mainRect = new C2DRectangle(32, 32, 400, 300, 0, C2D_COL_BLUE);
    C2DRectangle *childRect = new C2DRectangle(32, 32, 200, 150, 0, C2D_COL_RED);
    mainRect->Add(childRect);

    renderer->Add(mainRect);

    for (int i = 0; i < 10; i++) {

        if (i == 5) {
            delete (childRect);
        }

        mainRect->SetRect(
                mainRect->GetRect().x + (i * 10), mainRect->GetRect().y + (i * 10),
                mainRect->GetRect().w - (i * 10), mainRect->GetRect().h - (i * 10));

        renderer->Draw();
        renderer->Delay(500);
    }

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
    texture->Lock(Rect(), (void **) &buffer, &pitch);
    fread(buffer, 1, 384 * 224 * 2, pFile);
    fclose(pFile);
    texture->Unlock();
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
