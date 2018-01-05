//
// Created by cpasjuste on 21/11/16.
//

#include <SDL2/SDL.h>
#include <psp2/kernel/threadmgr.h>

#include "libvita2d/include/vita2d.h"
#include "libvita2d/include/shared.h"

#include "psp2/psp2_renderer.h"
#include "psp2/psp2_texture.h"
#include "psp2/psp2_shaders.h"

using namespace c2d;

extern "C" {
void vita2d_set_texture_program();
void vita2d_set_texture_tint_program();
void vita2_set_texture_tint_color_uniform(unsigned int color);
void vita2d_set_texture_wvp_uniform(const vita2d_texture *texture);
}

PSP2Renderer::PSP2Renderer(const Vector2f &size) : Renderer(size) {

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    vita2d_init();

    this->shaders = (Shaders *)
            new PSP2Shaders("");
    setShader(0);
}

void PSP2Renderer::setShader(int index) {

    if (index == shaders->current || index >= shaders->getCount()) {
        return;
    }
    shaders->current = index;

    vita2d_shader *shader = (vita2d_shader *) shaders->get()->data;
    if (shader != NULL) {
        vita2d_texture_set_program(shader->vertexProgram, shader->fragmentProgram);
        vita2d_texture_set_wvp(shader->wvpParam);
        vita2d_texture_set_vertexInput(&shader->vertexInput);
        vita2d_texture_set_fragmentInput(&shader->fragmentInput);
    }
}

void PSP2Renderer::draw(const VertexArray &vertices,
                        const Transform &transform,
                        const Texture *texture) {

    unsigned int count = vertices.getVertexCount();

    uint16_t *v2d_indices = (uint16_t *) vita2d_pool_memalign(
            count * sizeof(uint16_t), sizeof(uint16_t));

    SceGxmPrimitiveType type;
    switch (vertices.getPrimitiveType()) {

        case PrimitiveType::Triangles:
            type = SCE_GXM_PRIMITIVE_TRIANGLES;
            break;

        case PrimitiveType::Lines:
            type = SCE_GXM_PRIMITIVE_LINES;
            break;

        case PrimitiveType::TriangleStrip:
            type = SCE_GXM_PRIMITIVE_TRIANGLE_STRIP;
            break;

        case PrimitiveType::TriangleFan:
            type = SCE_GXM_PRIMITIVE_TRIANGLE_FAN;
            break;

        default:
            printf("PSP2Render::draw: unsupported primitive type\n");
            return;
    }

    if (texture) {

        if (vertices[0].color == Color::White) {
            vita2d_set_texture_program();
            vita2d_set_texture_wvp_uniform(((PSP2Texture *) texture)->tex);
        } else {
            vita2d_set_texture_tint_program();
            vita2d_set_texture_wvp_uniform(((PSP2Texture *) texture)->tex);
            vita2_set_texture_tint_color_uniform(
                    RGBA8((unsigned int) vertices[0].color.r,
                          (unsigned int) vertices[0].color.g,
                          (unsigned int) vertices[0].color.b,
                          (unsigned int) vertices[0].color.a));
        }

        vita2d_texture_vertex *v2d_vertices =
                (vita2d_texture_vertex *) vita2d_pool_memalign(
                        count * sizeof(vita2d_texture_vertex),
                        sizeof(vita2d_texture_vertex));

        for (unsigned int i = 0; i < count; i++) {
            Vector2f v = transform.transformPoint(vertices[i].position);

            v2d_vertices[i].x = v.x;
            v2d_vertices[i].y = v.y;
            v2d_vertices[i].z = +0.5f;
            v2d_vertices[i].w = 1.0f;
            v2d_vertices[i].u = vertices[i].texCoords.x / texture->getSize().x;
            v2d_vertices[i].v = vertices[i].texCoords.y / texture->getSize().y;

            v2d_indices[i] = (uint16_t) i;
        }

        sceGxmSetFragmentTexture(_vita2d_context, 0, &((PSP2Texture *) texture)->tex->gxm_tex);
        sceGxmSetVertexStream(_vita2d_context, 0, v2d_vertices);
        sceGxmDraw(_vita2d_context, type, SCE_GXM_INDEX_FORMAT_U16, v2d_indices, count);

    } else {

        vita2d_color_vertex *v2d_vertices =
                (vita2d_color_vertex *)
                        vita2d_pool_memalign(
                                count * sizeof(vita2d_color_vertex),
                                sizeof(vita2d_color_vertex));

        for (unsigned int i = 0; i < count; i++) {
            Vector2f v = transform.transformPoint(vertices[i].position);
            v2d_vertices[i].x = v.x;
            v2d_vertices[i].y = v.y;
            v2d_vertices[i].z = +0.5f;
            v2d_vertices[i].color =
                    RGBA8((unsigned int) vertices[i].color.r,
                          (unsigned int) vertices[i].color.g,
                          (unsigned int) vertices[i].color.b,
                          (unsigned int) vertices[i].color.a);
            v2d_indices[i] = (uint16_t) i;
        }

        sceGxmSetVertexProgram(_vita2d_context, _vita2d_colorVertexProgram);
        sceGxmSetFragmentProgram(_vita2d_context, _vita2d_colorFragmentProgram);

        void *vertexDefaultBuffer;
        sceGxmReserveVertexDefaultUniformBuffer(_vita2d_context, &vertexDefaultBuffer);
        sceGxmSetUniformDataF(vertexDefaultBuffer, _vita2d_colorWvpParam, 0, 16, _vita2d_ortho_matrix);

        sceGxmSetVertexStream(_vita2d_context, 0, v2d_vertices);

        sceGxmDraw(_vita2d_context, type, SCE_GXM_INDEX_FORMAT_U16, v2d_indices, count);
    }
}

void PSP2Renderer::flip() {

    vita2d_start_drawing();

    // clear screen
    vita2d_set_clear_color(
            RGBA8((unsigned int) getFillColor().r,
                  (unsigned int) getFillColor().g,
                  (unsigned int) getFillColor().b,
                  (unsigned int) getFillColor().a));
    vita2d_clear_screen();

    // call base class (draw childs)
    Renderer::flip();

    // flip
    vita2d_end_drawing();
    vita2d_wait_rendering_done();
    vita2d_swap_buffers();
}

void PSP2Renderer::delay(unsigned int ms) {

    const Uint32 max_delay = 0xffffffffUL / 1000;
    if (ms > max_delay) {
        ms = max_delay;
    }
    sceKernelDelayThreadCB(ms * 1000);
}

PSP2Renderer::~PSP2Renderer() {

    vita2d_wait_rendering_done();
    vita2d_fini();
    delete (shaders);
}

void PSP2Renderer::startDrawing() {

    if (!drawing_started) {
        vita2d_start_drawing();
        drawing_started = true;
    }
}
