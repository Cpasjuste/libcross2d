//
// Created by cpasjuste on 27/01/17.
//

#ifdef __GL2__

#include "cross2d/c2d.h"

#ifdef __PSP2__

#include "shaders/color_f.h"
#include "shaders/color_v.h"
#include "shaders/texture_f.h"
#include "shaders/texture_v.h"
#include "shaders/lcd3x_f.h"
#include "shaders/lcd3x_v.h"

#else
#include "shaders/shaders.h"
#endif
using namespace c2d;

static GLuint createAndCompileShader(GLenum type, const char *source, int size = 0) {

    GLuint handle;

    GL_CHECK(handle = glCreateShader(type));
    if (!handle) {
        printf("%u: cannot create shader\n", type);
        return 0;
    }

#ifdef __PSP2__
    glShaderBinary(1, &handle, 0, source, size);
#else
    GLint success;
    GLchar msg[512];
    GL_CHECK(glShaderSource(handle, 1, &source, nullptr));
    GL_CHECK(glCompileShader(handle));
    GL_CHECK(glGetShaderiv(handle, GL_COMPILE_STATUS, &success));
    if (!success) {
        glGetShaderInfoLog(handle, sizeof(msg), nullptr, msg);
        printf("createAndCompileShader: %u: %s\n", type, msg);
        glDeleteShader(handle);
        return 0;
    }
#endif

    return handle;
}

GLShader::GLShader(const char *vertex, const char *fragment, int type, int vsize, int fsize) {

    GLuint vsh, fsh;
    std::string v, f;

#ifdef __PSP2__
    vsh = createAndCompileShader(GL_VERTEX_SHADER, vertex, vsize);
    fsh = createAndCompileShader(GL_FRAGMENT_SHADER, fragment, fsize);
#else
#if __GLES2__
    v = std::string("#version 100\n") + vertex;
    f = std::string("#version 100\n") + fragment;
#else
    v = std::string("#version 330\n") + vertex;
    f = std::string("#version 330\n") + fragment;
#endif

    GL_CHECK(vsh = createAndCompileShader(GL_VERTEX_SHADER, v.c_str(), vsize));
    if (!vsh) {
        printf("GLShader: vsh compilation failed\n");
        return;
    }

    GL_CHECK(fsh = createAndCompileShader(GL_FRAGMENT_SHADER, f.c_str(), fsize));
    if (!fsh) {
        glDeleteShader(vsh);
        printf("GLShader: fsh compilation failed\n");
        return;
    }
#endif
    GL_CHECK(program = glCreateProgram());
    GL_CHECK(glAttachShader(program, vsh));
    GL_CHECK(glAttachShader(program, fsh));
    GL_CHECK(glBindAttribLocation(program, 0, "a_Position"));
    GL_CHECK(glBindAttribLocation(program, 1, "a_Color"));
    GL_CHECK(glBindAttribLocation(program, 2, "a_Texcoord0"));
    GL_CHECK(glLinkProgram(program));

    GLint success;
    GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &success));
    if (!success) {
        char buf[512];
        glGetProgramInfoLog(program, sizeof(buf), nullptr, buf);
        printf("GLShader: Link error: %s\n", buf);
    }

    GL_CHECK(glDeleteShader(vsh));
    GL_CHECK(glDeleteShader(fsh));

    scale_type = type;
    available = true;
}

void GLShader::SetUniformMatrix(const GLchar *n, const GLfloat *v) {
    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(program, n));
    if (loc > -1) {
        GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, v));
    }
}

void GLShader::SetUniform(const GLchar *n, const Vector2f &v) {
    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(program, n));
    if (loc > -1) {
        GL_CHECK(glUniform2f(loc, v.x, v.y));
    }
}

void GLShader::SetUniform(const GLchar *n, const float v0, const float v1) {
    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(program, n));
    if (loc > -1) {
        GL_CHECK(glUniform2f(loc, v0, v1));
    }
}

void GLShader::SetUniform(const GLchar *n, int v) {
    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(program, n));
    if (loc > -1) {
        GL_CHECK(glUniform1i(loc, v));
    }
}

GLuint GLShader::GetProgram() {
    return program;
}

int GLShader::getScaleType() {
    return scale_type;
}

GLShader::~GLShader() {
    if (program) {
        GL_CHECK(glDeleteProgram(program));
    }
}

GLShaderList::GLShaderList(const std::string &shadersPath) : ShaderList(shadersPath) {
#ifdef __PSP2__
    // add color shader
    auto *colorShader = new GLShader((const char *) color_v_data, (const char *) color_f_data,
                                     GLShader::SCALE_TYPE_SOURCE, color_v_size, color_f_size);
    color = new Shader("color", colorShader);
    GLShaderList::get(0)->data = new GLShader((const char *) texture_v_data, (const char *) texture_f_data,
                                              GLShader::SCALE_TYPE_SOURCE, texture_v_size, texture_f_size);
    GLShaderList::add("lcd3x", new GLShader((const char *) lcd3x_v_data, (const char *) lcd3x_f_data,
                                            GLShader::SCALE_TYPE_SOURCE, lcd3x_v_size, lcd3x_f_size));
#else
    // add color shader
    auto *colorShader = new GLShader(color_v, color_f);
    color = new Shader("color", colorShader);
    get(0)->data = new GLShader(texture_v, texture_f);

#ifndef __GLES2__
    add("bevel", new GLShader(bevel_v, bevel_f));
    // TODO: add("crt pi", new GLShader(crt_pi_v, crt_pi_f));
    add("crt aperture", new GLShader(crt_aperture_v, crt_aperture_f));
    add("crt caligari", new GLShader(crt_caligari_v, crt_caligari_f));
    add("crt cgwg fast", new GLShader(crt_cgwg_fast_v, crt_cgwg_fast_f));
    add("crt easymode", new GLShader(crt_easymode_v, crt_easymode_f));
    add("crt geom", new GLShader(crt_geom_v, crt_geom_f));
    add("crt hyllian", new GLShader(crt_hyllian_v, crt_hyllian_f));
    // TODO: add("crt zfast", new GLShader(crt_zfast_v, crt_zfast_f));
    add("dot", new GLShader(dot_v, dot_f));
    if (C2D_SCREEN_HEIGHT > 240) {
        add("lcd3x", new GLShader(lcd3x_v, lcd3x_f));
        add("retro v2", new GLShader(retro_v2_v, retro_v2_f));
    }
    add("sabr v3", new GLShader(sabr_v3_v, sabr_v3_f));
    add("sal2x", new GLShader(sal2x_v, sal2x_f));
    add("scanlines", new GLShader(scanlines_v, scanlines_f));
    add("sharp bilinear", new GLShader(sharp_bilinear_v, sharp_bilinear_f));
    add("sharp bilinear scanlines", new GLShader(sharp_bilinear_scanlines_v, sharp_bilinear_scanlines_f));
    add("supereagle", new GLShader(supereagle_v, supereagle_f));
    // TODO: add("xbrz freescale", new GLShader(xbrz_freescale_v, xbrz_freescale_f, GLShader::SCALE_TYPE_VIEWPORT));
#else
    add("sharp bilinear", new GLShader(sharp_bilinear_v, sharp_bilinear_f));
#endif
#endif
}

GLShaderList::~GLShaderList() {

    if (color) {
        if (color->data != nullptr) {
            delete ((GLShader *) color->data);
        }
        delete (color);
    }

    for (int i = 0; i < GLShaderList::getCount(); i++) {
        if (GLShaderList::get(i)->data != nullptr) {
            delete ((GLShader *) GLShaderList::get(i)->data);
        }
    }
}

#endif // __GL2__
