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

#elif __PS4__

#include "../ps4/shaders/ps4_shaders.h"

#else

#include "shaders/shaders.h"

#endif

using namespace c2d;

static GLuint compile(GLenum type, const char *source, int size = 0) {

    GLuint handle;
    GLint compile_result;
    GLchar msg[512];

    GL_CHECK(handle = glCreateShader(type));
    if (!handle) {
        printf("GLShader::compile: glCreateShader failed (%s)\n",
               type == GL_VERTEX_SHADER ? "vertex" : "fragment");
        return 0;
    }

    if (size > 0) {
        //printf("compile: glShaderBinary: type: %s, handle = %i, size = %i\n",
        //       type == GL_VERTEX_SHADER ? "vertex" : "fragment", handle, size);
        glShaderBinary(1, &handle, 0, source, size);
    } else {
        GL_CHECK(glShaderSource(handle, 1, &source, nullptr));
        GL_CHECK(glCompileShader(handle));
    }

    GL_CHECK(glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_result));
    if (!compile_result) {
        glGetShaderInfoLog(handle, sizeof(msg), nullptr, msg);
        printf("GLShader::compile: %u: %s\n", type, msg);
        glDeleteShader(handle);
        return 0;
    }

    return handle;
}

GLShader::GLShader(const char *vertex, const char *fragment, int vsize, int fsize) {

    GLuint vsh, fsh;
    std::string v, f;
    GLint link_result;

    // use precompiled shaders if size > 0
    if (vsize > 0 || fsize > 0) {
        vsh = compile(GL_VERTEX_SHADER, vertex, vsize);
        fsh = compile(GL_FRAGMENT_SHADER, fragment, fsize);
    } else {
#if __GLES2__
        v = std::string("#version 100\n") + vertex;
        f = std::string("#version 100\n") + fragment;
#else
        v = std::string("#version 330\n") + vertex;
        f = std::string("#version 330\n") + fragment;
#endif
        vsh = compile(GL_VERTEX_SHADER, v.c_str(), vsize);
        if (!vsh) {
            printf("GLShader: vsh compilation failed\n");
            return;
        }

        fsh = compile(GL_FRAGMENT_SHADER, f.c_str(), fsize);
        if (!fsh) {
            glDeleteShader(vsh);
            printf("GLShader: fsh compilation failed\n");
            return;
        }
    }

    GL_CHECK(program = glCreateProgram());
    GL_CHECK(glAttachShader(program, vsh));
    GL_CHECK(glAttachShader(program, fsh));
    GL_CHECK(glBindAttribLocation(program, 0, "a_position"));
    GL_CHECK(glBindAttribLocation(program, 1, "a_color"));
    GL_CHECK(glBindAttribLocation(program, 2, "a_texCoord"));
    GL_CHECK(glLinkProgram(program));
    GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &link_result));
    if (!link_result) {
        char buf[512];
        glGetProgramInfoLog(program, sizeof(buf), nullptr, buf);
        printf("GLShader: Link error: %s\n", buf);
        glDeleteShader(vsh);
        glDeleteShader(fsh);
        return;
    }

#ifdef __PS4__
        //PS4Sys::dumpShaderBinary(vsh, name, "v");
        //PS4Sys::dumpShaderBinary(fsh, name, "f");
#endif

    GL_CHECK(glDeleteShader(vsh));
    GL_CHECK(glDeleteShader(fsh));

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
    // color shader
    color = new Shader("color", new GLShader(color_v, color_f,
                                             color_v_length, color_f_length));
    // texture shader
    get(0)->data = new GLShader(texture_v, texture_f,
                                texture_v_length, texture_f_length);

    // custom shaders
    add("bevel", new GLShader(bevel_v, bevel_f,
                              bevel_v_length, bevel_f_length));

    add("crt aperture", new GLShader(crt_aperture_v, crt_aperture_f,
                                     crt_aperture_v_length, crt_aperture_f_length));

    add("crt cgwg fast", new GLShader(crt_cgwg_fast_v, crt_cgwg_fast_f,
                                      crt_cgwg_fast_v_length, crt_cgwg_fast_f_length));

    add("crt easymode", new GLShader(crt_easymode_v, crt_easymode_f,
                                     crt_easymode_v_length, crt_easymode_f_length));

    add("crt hyllian", new GLShader(crt_hyllian_v, crt_hyllian_f,
                                    crt_hyllian_v_length, crt_hyllian_f_length));

    add("dot", new GLShader(dot_v, dot_f, dot_v_length, dot_f_length));

    if (C2D_SCREEN_HEIGHT > 240) {
        add("lcd3x", new GLShader(lcd3x_v, lcd3x_f,
                                  lcd3x_v_length, lcd3x_f_length));
        add("retro v2", new GLShader(retro_v2_v, retro_v2_f,
                                     retro_v2_v_length, retro_v2_f_length));
    }

    add("sabr v3", new GLShader(sabr_v3_v, sabr_v3_f,
                                sabr_v3_v_length, sabr_v3_f_length));

    add("sal2x", new GLShader(sal2x_v, sal2x_f,
                              sal2x_v_length, sal2x_f_length));

    add("scanlines", new GLShader(scanlines_v, scanlines_f,
                                  scanlines_v_length, scanlines_f_length));

    add("sharp bilinear", new GLShader(sharp_bilinear_v, sharp_bilinear_f,
                                       sharp_bilinear_v_length, sharp_bilinear_f_length));

    add("sharp bilinear scanlines", new GLShader(sharp_bilinear_scanlines_v, sharp_bilinear_scanlines_f,
                                                 sharp_bilinear_scanlines_v_length, sharp_bilinear_scanlines_f_length));

    add("supereagle", new GLShader(supereagle_v, supereagle_f,
                                   supereagle_v_length, supereagle_f_length));
#ifndef __GLES2__
    add("crt caligari", new GLShader(crt_caligari_v, crt_caligari_f));
    add("crt geom", new GLShader(crt_geom_v, crt_geom_f));
    add("crt geom flat", new GLShader(crt_geom_flat_v, crt_geom_flat_f));
#endif // __GLES2__
#endif // __PSP2__
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
