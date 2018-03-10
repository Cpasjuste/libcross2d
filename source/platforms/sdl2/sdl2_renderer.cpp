//
// Created by cpasjuste on 21/11/16.
//

//
// SDL2 with software surfaces for rendering
//

#ifndef __SDL2_GL__

#include "c2d.h"

using namespace c2d;

static void rendere_rinfo(SDL_Renderer *rdr) {
    SDL_RendererInfo info;
    int i, n;

    n = SDL_GetNumRenderDrivers();

    SDL_GetRendererInfo(rdr, &info);
    printf("SDL Rendering info: n=%d, current=%s\n", n, info.name);

    for (i = 0; i < n; i++) {
        uint32_t j;

        SDL_GetRenderDriverInfo(i, &info);
        printf("    %d: %s flags=%08x num_formats=%u",
               i, info.name, info.flags,
               info.num_texture_formats);

        for (j = 0; j < info.num_texture_formats; j++) {
            uint32_t fmt = info.texture_formats[j];
            printf(" %s", SDL_GetPixelFormatName(fmt));
        }

        printf("\n");
    }
}

SDL2Renderer::SDL2Renderer(const Vector2f &size) : Renderer(size) {

#ifdef __NX__
    consoleDebugInit(debugDevice_SVC);
    stdout = stderr;
#endif

    if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    Uint32 flags = 0;
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
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
            return;
        }
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't create hw renderer: %s, trying sw renderer\n", SDL_GetError());
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (!renderer) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Couldn't create sw renderer: %s, giving up...\n", SDL_GetError());
            return;
        }
    }
    rendere_rinfo(renderer);

    // basic software effect, only used/usefull on switch for now
    shaderList = new ShaderList();
    shaderList->add("SCANLINE", NULL);
    shaderList->add("SCANLINE+", NULL);

    available = true;
}

void SDL2Renderer::draw(const VertexArray &vertices,
                        const Transform &transform,
                        const Texture *texture) {

    size_t count = vertices.getVertexCount();
    int type = vertices.getPrimitiveType();

    //printf("draw: type=%i | vertex=%i\n", type, (int) count);

    SDL2Texture *tex = ((SDL2Texture *) texture);
    if (type == 4) {
        // TriangleStrip (outline)
        // TODO: fix this ugly crap

        if (vertices[0].color.a < 255) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        }
        SDL_SetRenderDrawColor(renderer, vertices[0].color.r,
                               vertices[0].color.g,
                               vertices[0].color.b,
                               vertices[0].color.a);

        Vector2f p[2] = {
                {transform.transformPoint(
                        (int) vertices[1].position.x,
                        (int) vertices[1].position.y)},
                {transform.transformPoint(
                        (int) vertices[2].position.x,
                        (int) vertices[2].position.y)}
        };
        SDL_Rect dst = {(int) p[0].x, (int) p[0].y,
                        (int) (p[1].x - p[0].x), (int) (p[1].y - p[0].y)};
        SDL_RenderFillRect(renderer, &dst);

        p[0] = {transform.transformPoint(
                (int) vertices[2].position.x,
                (int) vertices[1].position.y)};
        p[1] = {transform.transformPoint(
                (int) vertices[5].position.x,
                (int) vertices[5].position.y)};
        dst = {(int) p[0].x, (int) p[0].y,
               (int) (p[1].x - p[0].x), (int) (p[1].y - p[0].y)};
        SDL_RenderFillRect(renderer, &dst);

        p[0] = {transform.transformPoint(
                (int) vertices[7].position.x,
                (int) vertices[6].position.y)};
        p[1] = {transform.transformPoint(
                (int) vertices[5].position.x,
                (int) vertices[5].position.y)};
        dst = {(int) p[0].x, (int) p[0].y,
               (int) (p[1].x - p[0].x), (int) (p[1].y - p[0].y)};
        SDL_RenderFillRect(renderer, &dst);

        p[0] = {transform.transformPoint(
                (int) vertices[9].position.x,
                (int) vertices[9].position.y)};
        p[1] = {transform.transformPoint(
                (int) vertices[8].position.x,
                (int) vertices[7].position.y)};
        dst = {(int) p[0].x, (int) p[0].y,
               (int) (p[1].x - p[0].x), (int) (p[1].y - p[0].y)};
        SDL_RenderFillRect(renderer, &dst);

        SDL_SetRenderDrawColor(renderer, getFillColor().r,
                               getFillColor().g,
                               getFillColor().b,
                               getFillColor().a);
        if (vertices[0].color.a < 255) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }

    } else if (type == 3 || type == 5) {

        // Triangles // TriangleFan
        int p0 = type == 3 ? 0 : 5;
        int p1 = type == 3 ? 5 : 3;

        for (size_t i = 0; i < count; i += 6) {

            Vector2f p[2] = {
                    {transform.transformPoint(
                            (int) vertices[i + p0].position.x,
                            (int) vertices[i + p0].position.y)},
                    {transform.transformPoint(
                            (int) vertices[i + p1].position.x,
                            (int) vertices[i + p1].position.y)}
            };

            SDL_Rect dst = {(int) p[0].x, (int) p[0].y,
                            (int) (p[1].x - p[0].x), (int) (p[1].y - p[0].y)};

            SDL_Rect src = {};
            if (type == 3) {
                src = {
                        (int) vertices[i + 0].texCoords.x,
                        (int) vertices[i + 0].texCoords.y,
                        (int) (vertices[i + 1].texCoords.x - vertices[i + 0].texCoords.x),
                        (int) (vertices[i + 3].texCoords.y - vertices[i + 0].texCoords.y)
                };
            } else {
                src = {
                        (int) vertices[i + 5].texCoords.x,
                        (int) vertices[i + 5].texCoords.y,
                        (int) (vertices[i + 5].texCoords.x + vertices[i + 3].texCoords.x),
                        (int) (vertices[i + 5].texCoords.y + vertices[i + 3].texCoords.y)
                };
            }

            if (tex) {
                SDL_SetTextureAlphaMod(tex->tex, vertices[i].color.a);
                SDL_SetTextureColorMod(tex->tex,
                                       vertices[i].color.r,
                                       vertices[i].color.g,
                                       vertices[i].color.b);
                SDL_RenderCopy(renderer, tex->tex, &src, &dst);
            } else {
                if (vertices[i].color.a < 255) {
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                }
                SDL_SetRenderDrawColor(renderer, vertices[i].color.r,
                                       vertices[i].color.g,
                                       vertices[i].color.b,
                                       vertices[i].color.a);
                SDL_RenderFillRect(renderer, &dst);
                SDL_SetRenderDrawColor(renderer, getFillColor().r,
                                       getFillColor().g,
                                       getFillColor().b,
                                       getFillColor().a);
                if (vertices[i].color.a < 255) {
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                }
            }
        }
    }
}

void SDL2Renderer::flip(bool draw) {

    if (draw) {

        SDL_SetRenderDrawColor(renderer,
                               getFillColor().r,
                               getFillColor().g,
                               getFillColor().b,
                               getFillColor().a);
        SDL_RenderClear(renderer);

        // call base class (draw childs)
        Renderer::flip();
    }

    SDL_RenderPresent(renderer);
}

void SDL2Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL2Renderer::~SDL2Renderer() {

    delete (shaderList);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif
