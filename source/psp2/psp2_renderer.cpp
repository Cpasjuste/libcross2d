//
// Created by cpasjuste on 21/11/16.
//

#include <SDL2/SDL.h>
#include <psp2/kernel/threadmgr.h>

#include "libvita2d/include/vita2d.h"
#include "libvita2d/include/shared.h"

#include "psp2/psp2_renderer.h"
#include "psp2/psp2_texture.h"
//#include "psp2/psp2_font.h"
//#include "psp2/psp2_texture.h"
#include "psp2/psp2_shaders.h"

using namespace c2d;

extern "C" {
void vita2d_set_texture_program();
void vita2d_set_texture_wvp_uniform(const vita2d_texture *texture);
}

//////////
// INIT //
//////////
PSP2Renderer::PSP2Renderer(const Vector2f &size) : Renderer(size) {

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    vita2d_init();

    this->shaders = (Shaders *) new PSP2Shaders("");
    setShader(0);
}
//////////
// INIT //
//////////

void PSP2Renderer::setShader(int index) {

    if (index == shaders->current || index >= shaders->Count()) {
        return;
    }
    shaders->current = index;

    vita2d_shader *shader = (vita2d_shader *) shaders->Get()->data;
    if (shader != NULL) {
        vita2d_texture_set_program(shader->vertexProgram, shader->fragmentProgram);
        vita2d_texture_set_wvp(shader->wvpParam);
        vita2d_texture_set_vertexInput(&shader->vertexInput);
        vita2d_texture_set_fragmentInput(&shader->fragmentInput);
    }
}

static void drawRectangleInternal(const VertexArray &vertices,
                                  const Transform &transform) {

    unsigned int count = vertices.getVertexCount();

    vita2d_color_vertex *v2d_vertices =
            (vita2d_color_vertex *)
                    vita2d_pool_memalign(count * sizeof(vita2d_color_vertex), sizeof(vita2d_color_vertex));

    uint16_t *v2d_indices =
            (uint16_t *)
                    vita2d_pool_memalign(count * sizeof(uint16_t), sizeof(uint16_t));

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
        default:
            type = SCE_GXM_PRIMITIVE_TRIANGLE_FAN;
            break;
    }

    sceGxmDraw(_vita2d_context, type, SCE_GXM_INDEX_FORMAT_U16, v2d_indices, count);
}


void PSP2Renderer::drawRectangle(Rectangle &rectangle, Transform &transform) {

    Transform combined = transform * rectangle.getTransform();
    drawRectangleInternal(rectangle.getVertices(), combined);
    if (rectangle.getOutlineThickness() > 0) {
        drawRectangleInternal(rectangle.getOutlineVertices(), combined);
    }
}

void PSP2Renderer::drawTexture(Texture &texture, Transform &transform) {

    vita2d_set_texture_program();
    vita2d_set_texture_wvp_uniform(((PSP2Texture *) &texture)->tex);

    // draw
    vita2d_texture_vertex *vertices = (vita2d_texture_vertex *) vita2d_pool_memalign(
            4 * sizeof(vita2d_texture_vertex), // 4 vertices
            sizeof(vita2d_texture_vertex));

    uint16_t *indices = (uint16_t *) vita2d_pool_memalign(
            4 * sizeof(uint16_t), // 4 indices
            sizeof(uint16_t));

    Transform combined = transform * texture.getTransform();

    Vector2f v0 = combined.transformPoint(texture.getPoint(0));
    vertices[0].x = v0.x;
    vertices[0].y = v0.y;
    vertices[0].z = +0.5f;
    vertices[0].w = 1.0f;
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;

    Vector2f v1 = combined.transformPoint(texture.getPoint(1));
    vertices[1].x = v1.x;
    vertices[1].y = v1.y;
    vertices[1].z = +0.5f;
    vertices[1].w = 1.0f;
    vertices[1].u = 1.0f;
    vertices[1].v = 0.0f;

    Vector2f v2 = combined.transformPoint(texture.getPoint(3));
    vertices[2].x = v2.x;
    vertices[2].y = v2.y;
    vertices[2].z = +0.5f;
    vertices[2].w = 1.0f;
    vertices[2].u = 0.0f;
    vertices[2].v = 1.0f;

    Vector2f v3 = combined.transformPoint(texture.getPoint(2));
    vertices[3].x = v3.x;
    vertices[3].y = v3.y;
    vertices[3].z = +0.5f;
    vertices[3].w = 1.0f;
    vertices[3].u = 1.0f;
    vertices[3].v = 1.0f;

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;

    sceGxmSetFragmentTexture(_vita2d_context, 0, &((PSP2Texture *) &texture)->tex->gxm_tex);

    sceGxmSetVertexStream(_vita2d_context, 0, vertices);
    sceGxmDraw(_vita2d_context, SCE_GXM_PRIMITIVE_TRIANGLE_STRIP, SCE_GXM_INDEX_FORMAT_U16, indices, 4);
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
