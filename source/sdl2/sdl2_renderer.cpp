//
// Created by cpasjuste on 21/11/16.
//

#include "c2d.h"

using namespace c2d;

SDL2Renderer::SDL2Renderer(const Vector2f &size) : Renderer(size) {

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    Uint32 flags = SDL_WINDOW_OPENGL;
    if (!getSize().x || !getSize().y) { // force fullscreen if window size == 0
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    window = SDL_CreateWindow(
            "CROSS2D_SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            (int) getSize().x, (int) getSize().y, flags);

    if (window == nullptr) {
        window = SDL_CreateWindow(
                "CROSS2D_SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                (int) getSize().x, (int) getSize().y, 0);
        if (window == nullptr) {
            printf("Couldn't create window: %s\n", SDL_GetError());
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
            return;
        }
    }

    sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (sdl_renderer == nullptr) {
        printf("Couldn't create renderer: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s\n", SDL_GetError());
        return;
    }

    // sdl2 doesn't support rect rotation, create a texture for that
    draw_tex = new SDL2Texture(Vector2f(32, 32));
    void *pixels;
    int pitch;
    draw_tex->lock(FloatRect(), &pixels, &pitch);
    memset(pixels, 255, (size_t) 32 * pitch);
    draw_tex->unlock();

    printf("SDL2Renderer(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

/*
void SDL2Renderer::DrawLine(Line *line) {

    if (!line || line->getVisibility() == C2D_VISIBILITY_HIDDEN) {
        return;
    }

    printf("SDL2Renderer(%p): DrawLine(%p)\n", this, line);

    // set color
    Color c = line->GetColor();
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);

    // draw with world/absolute positions (GetWorldRect)
    // x = x1, y = y1, w = x2; h = y2
    //Vec4 r = line->GetRect();
    std::vector<Vec2> bounds = line->GetLocalBounds();
    SDL_RenderDrawLine(sdl_renderer,
                       (int) bounds[0].x, (int) bounds[0].y,
                       (int) bounds[1].x, (int) bounds[1].y);

    // reset color
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_NONE);
    }
    SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
}
*/

void SDL2Renderer::drawRectangle(const Rectangle &rectangle, const Transform &parentTransform) {

    printf("SDL2Renderer(%p): drawRectangle()\n", this);

    // set color
    if (rectangle.getFillColor().a < 255) {
        SDL_SetTextureBlendMode(draw_tex->tex, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(draw_tex->tex, rectangle.getFillColor().a);
    }

    SDL_SetTextureColorMod(draw_tex->tex,
                           rectangle.getFillColor().r,
                           rectangle.getFillColor().g,
                           rectangle.getFillColor().b);

    // set transform
    FloatRect trans = parentTransform.transformRect(rectangle.getGlobalBounds());
    SDL_Rect rect = {
            (int) trans.top,
            (int) trans.left,
            (int) trans.width,
            (int) trans.height};

    // float rotation = parentTransform.getMatrix()
    // if (rectangle->fill) {
    SDL_RenderCopyEx(sdl_renderer, draw_tex->tex, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    //SDL_RenderFillRect(sdl_renderer, &r_sdl);
    // } else {
    //     SDL_RenderDrawRect(sdl_renderer, &r_sdl);
    // }

    // reset color
    if (rectangle.getFillColor().a < 255) {
        SDL_SetTextureBlendMode(draw_tex->tex, SDL_BLENDMODE_NONE);
    }
}

void SDL2Renderer::drawTexture(const Texture &texture, const Transform &parentTransform) {

    printf("SDL2Renderer(%p): drawTexture()\n", this);

    SDL_Rect src = {
            texture.getTextureRect().top,
            texture.getTextureRect().left,
            texture.getTextureRect().width,
            texture.getTextureRect().height
    };

    FloatRect trans = parentTransform.transformRect(texture.getGlobalBounds());
    SDL_Rect dst = {
            (int) trans.top,
            (int) trans.left,
            (int) trans.width,
            (int) trans.height
    };

    SDL_RenderCopyEx(sdl_renderer, ((SDL2Texture *) &texture)->tex, &src, &dst, 0, NULL, SDL_FLIP_NONE);
}

void SDL2Renderer::flip() {

    printf("SDL2Renderer(%p): flip\n", this);

    // clear screen
    SDL_SetRenderDrawColor(sdl_renderer,
                           getFillColor().r,
                           getFillColor().g,
                           getFillColor().b,
                           getFillColor().a);
    SDL_RenderClear(sdl_renderer);

    // call base class (draw childs)
    Renderer::flip();

    // flip
    SDL_RenderPresent(sdl_renderer);
}

void SDL2Renderer::setShader(int shader) {
    // TODO
}

void SDL2Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL2Renderer::~SDL2Renderer() {

    printf("~SDL2Renderer\n");
    delete (draw_tex);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
