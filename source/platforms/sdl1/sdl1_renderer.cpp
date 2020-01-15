//
// Created by cpasjuste on 21/11/16.
//

//
// SDL1 software renderer
//

#ifndef __GL1__

#include "cross2d/c2d.h"

using namespace c2d;

SDL1Renderer::SDL1Renderer(const Vector2f &size) : Renderer(size) {

    printf("SDL1Renderer()\n");

    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return;
    }

    screen = SDL_SetVideoMode((int) getSize().x, (int) getSize().y, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return;
    }

    available = true;
}

void SDL1Renderer::draw(VertexArray *vertexArray, const Transform &transform, Texture *texture) {

    if (!vertexArray || vertexArray->getVertexCount() < 1) {
        printf("SDL2Renderer::draw: no vertices\n");
        return;
    }

    size_t count = vertexArray->getVertexCount();
    int type = vertexArray->getPrimitiveType();
    Vertex *vertices = vertexArray->getVertices()->data();

    SDL1Texture *tex = texture ? ((SDL1Texture *) texture) : nullptr;

    //printf("draw: type=%i | vertex=%i | tex=%p\n", type, (int) count, tex);

    if (type == 4 && !tex) {
        // TriangleStrip (outline)
        // TODO: fix this ugly crap
        if (vertices[0].color.a < 255) {
            //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        }
        Uint32 color = SDL_MapRGBA(screen->format,
                                   vertices[0].color.r,
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
        SDL_FillRect(screen, &dst, color);

        p[0] = {transform.transformPoint(
                (int) vertices[2].position.x,
                (int) vertices[1].position.y)};
        p[1] = {transform.transformPoint(
                (int) vertices[5].position.x,
                (int) vertices[5].position.y)};
        dst = {(int) p[0].x, (int) p[0].y,
               (int) (p[1].x - p[0].x), (int) (p[1].y - p[0].y)};
        SDL_FillRect(screen, &dst, color);

        p[0] = {transform.transformPoint(
                (int) vertices[7].position.x,
                (int) vertices[6].position.y)};
        p[1] = {transform.transformPoint(
                (int) vertices[5].position.x,
                (int) vertices[5].position.y)};
        dst = {(int) p[0].x, (int) p[0].y,
               (int) (p[1].x - p[0].x), (int) (p[1].y - p[0].y)};
        SDL_FillRect(screen, &dst, color);

        p[0] = {transform.transformPoint(
                (int) vertices[9].position.x,
                (int) vertices[9].position.y)};
        p[1] = {transform.transformPoint(
                (int) vertices[8].position.x,
                (int) vertices[7].position.y)};
        dst = {(int) p[0].x, (int) p[0].y,
               (int) (p[1].x - p[0].x), (int) (p[1].y - p[0].y)};
        SDL_FillRect(screen, &dst, color);
        if (vertices[0].color.a < 255) {
            //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }
        //} else if (type == 3 || type == 5) {
    } else {
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
                //SDL_SetAlpha(tex->surface, SDL_SRCALPHA, vertices[i].color.a);
                SDL_BlitSurface(tex->surface, &src, screen, &dst);
            } else {
                SDL_FillRect(screen, &dst, SDL_MapRGBA(screen->format,
                                                       vertices[i].color.r,
                                                       vertices[i].color.g,
                                                       vertices[i].color.b,
                                                       vertices[i].color.a));
            }
        }
    }
}

void SDL1Renderer::clear() {
    SDL_FillRect(screen, nullptr,
                 SDL_MapRGBA(screen->format, m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
}

void SDL1Renderer::flip(bool draw, bool inputs) {

    process_inputs = _process_inputs;
    Renderer::onUpdate();

    if (draw) {
        clear();
    }

    // call base class (draw childs)
    Renderer::flip(draw, inputs);

    if (draw) {
        SDL_Flip(screen);
    }
}

void SDL1Renderer::delay(unsigned int ms) {
    SDL_Delay(ms);
}

SDL1Renderer::~SDL1Renderer() {

    if (screen) {
        SDL_FreeSurface(screen);
    }
    SDL_Quit();
}

#endif // __SDL1__
