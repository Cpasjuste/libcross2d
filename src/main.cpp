//
// Created by cpasjuste on 08/12/16.
//

#include <C2D.h>

#include "main.h"

int main() {

    // renderer
    Renderer *renderer = (Renderer *) new C2DRenderer(SCR_W, SCR_H);

    // input
    Input *input = (Input *) new C2DInput(renderer);
    input->SetKeyboardMapping(KEYS);

    // font
    Font *font = (Font *) new C2DFont(renderer, FONT_PATH, 20);

    // texture
    Texture *texture = (Texture *) new C2DTexture(renderer, TEX_PATH);

    /*
    Texture *texture = (Texture *) new C2DTexture(renderer, 384, 224);
    FILE *pFile;
    pFile = fopen("/home/cpasjuste/dev/frame.bin", "rb");
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
            if (player.state & EV_QUIT)
                break;
            //printf("state: %i\n", player.state);
            renderer->Delay(100);
        }

        renderer->Clear();

        /*
        // window
        Rect rect = {0, 0, renderer->width - 1, renderer->height - 1};
        renderer->DrawRect(rect, ORANGE, false);

        // inside rect
        rect.x += 1;
        rect.y += 1;
        rect.w -= 2;
        rect.h -= 2;
        renderer->DrawRect(rect, GRAY_LIGHT);

        // inside "inside rect"
        rect.x += 1;
        rect.y += 1;
        rect.w -= 2;
        rect.h -= 2;
        renderer->DrawRect(rect, GREEN, false);

        // "inside rect" inside border
        renderer->DrawBorder(rect, ORANGE);

        // screen centers
        renderer->DrawLine(0, rect.h / 2, rect.w, rect.h / 2); // X
        renderer->DrawLine(rect.w / 2, 0, rect.w / 2, rect.h); // Y

        // top middle text
        font->Draw(rect.w / 2, 0, "HELLO WORLD");

        // top left text
        font->Draw(0, 0, "HELLO WORLD");

        // centered text
        Rect r = {rect.w / 2, rect.h / 2, 0, 0};
        font->scaling = 2;
        r.w = font->GetWidth("HELLO WORLD");
        r.h = font->GetHeight("HELLO WORLD");
        r.x -= r.w / 2;
        r.y -= r.h / 2;
        renderer->DrawRect(r, RED, false);
        font->Draw(r, ORANGE, true, true, "HELLO WORLD SCALED");
        font->scaling = 1;

        // y centered and truncated text
        rect.x = 0;
        rect.y = 0;
        rect.w = 100;
        font->Draw(rect, WHITE, false, true, "HELLO WORLD");
        */

        texture->Draw(32, 32, texture->width, texture->height);

        renderer->Flip();
    }

    delete (input);
    delete (font);
    delete (texture);
    delete (renderer);
}
