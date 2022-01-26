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

// TODO: (re)add precompiled shader support (rebuild shaders for ps4)
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

GLShader::GLShader(const char *source, const std::string &version) {
    GLuint vsh, fsh;
    GLint link_result;
    std::string vertex, fragment;

    size_t pos = std::string(source).find("#version");
    if (pos != std::string::npos) {
        printf("GLShader::GLShader: version: (from source)\n");
        vertex = std::string(source).insert(pos + 13, "#define VERTEX\n");
        fragment = std::string(source).insert(pos + 13, "#define FRAGMENT\n");
    } else {
        printf("GLShader::GLShader: version: %s\n", version.c_str());
        vertex = std::string("#version " + version + "\n#define VERTEX\n") + source;
        fragment = std::string("#version " + version + "\n#define FRAGMENT\n") + source;
    }

    vsh = compile(GL_VERTEX_SHADER, vertex.c_str(), 0);
    if (!vsh) {
        printf("GLShader: vsh compilation failed\n");
        return;
    }

    fsh = compile(GL_FRAGMENT_SHADER, fragment.c_str(), 0);
    if (!fsh) {
        glDeleteShader(vsh);
        printf("GLShader: fsh compilation failed\n");
        return;
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

GLShader::~GLShader() {
    if (program) {
        GL_CHECK(glDeleteProgram(program));
    }
}

GLShaderList::GLShaderList() : ShaderList() {
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
    const char *glsl = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::string version = "100";
#ifndef __GLES2__
    if (glsl && strlen(glsl) > 0) {
        char v[32];
        int major, minor;
        sscanf(v, "%d.%d", &major, &minor);
        if (major > 0) {
            snprintf(v, 15, "%i", major * 100 + minor);
            version = v;
        }
    }
#endif

    // color shader
    color = new Shader("color", new GLShader(color_v, color_f,
                                             color_v_length, color_f_length));
    // texture shader
    get(0)->data = new GLShader(texture_v, texture_f,
                                texture_v_length, texture_f_length);

    // crt shaders
    add("crt-aperture", new GLShader(c2d_crt_aperture_shader, version));
    add("crt-caligari", new GLShader(c2d_crt_caligari_shader, version));
    add("crt-cgwg-fast", new GLShader(c2d_crt_cgwg_fast_shader, version));
    add("crt-easymode", new GLShader(c2d_crt_easymode_shader, version));
    add("crt-fakelottes", new GLShader(c2d_crt_fakelottes_shader, version));
    add("crt-geom", new GLShader(c2d_crt_geom_shader, version));
    add("crt-geom-flat", new GLShader(c2d_crt_geom_flat_shader, version));
    add("crt-hyllian", new GLShader(c2d_crt_hyllian_shader, version));
    add("crt-lottes-fast", new GLShader(c2d_crt_lottes_fast_shader, version));
    add("crt-lottes", new GLShader(c2d_crt_lottes_shader, version));
    add("crt-mattias", new GLShader(c2d_crt_mattias_shader, version));
    add("crt-nes-mini", new GLShader(c2d_crt_nes_mini_shader, version));
    add("crt-pi", new GLShader(c2d_crt_pi_shader, version));
    add("crt-pi-flat", new GLShader(c2d_crt_pi_flat_shader, version));
    add("crt-zfast", new GLShader(c2d_crt_zfast_shader, version));

    // handheld shaders
    add("handheld-bevel", new GLShader(c2d_handheld_bevel_shader, version));
    add("handheld-dot", new GLShader(c2d_handheld_dot_shader, version));
    add("handheld-lcd1x", new GLShader(c2d_handheld_lcd1x_shader, version));
    add("handheld-lcd3x", new GLShader(c2d_handheld_lcd3x_shader, version));
    add("handheld-retro-v2", new GLShader(c2d_handheld_retro_v2_shader, version));
    add("handheld-zfast-lcd", new GLShader(c2d_handheld_zfast_lcd_shader, version));

    // interpolation shaders
    add("interpolation-aann", new GLShader(c2d_interpolation_aann_shader, version));
    add("interpolation-pixellate", new GLShader(c2d_interpolation_pixellate_shader, version));
    add("interpolation-quilez", new GLShader(c2d_interpolation_quilez_shader, version));
    add("interpolation-sharp-bilinear", new GLShader(c2d_interpolation_sharp_bilinear_shader, version));
    add("interpolation-sharp-bilinear-scanlines",
        new GLShader(c2d_interpolation_sharp_bilinear_scanlines_shader, version));

    // scanlines
    add("scanline-simple", new GLShader(c2d_scanline_simple_shader, version));

    // sharp
    add("sharp-2xsal", new GLShader(c2d_sharp_2xsal_shader, version));
    add("sharp-sabr-v3.0", new GLShader(c2d_sharp_sabr_v30_shader, version));
    add("sharp-supereagle", new GLShader(c2d_sharp_supereagle_shader, version));
    add("sharp-xbrz-freescale", new GLShader(c2d_sharp_xbrz_freescale_shader, version));

#endif //__PSP2__
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
