//
// Created by cpasjuste on 27/01/17.
//

#ifdef __GL1__

#include "cross2d/c2d.h"
//#include "shaders/shaders.h"

using namespace c2d;

static GLuint createAndCompileShader(GLenum type, const char *source) {

    /*
    GLint success;
    GLchar msg[512];
    GLuint handle;

    GL_CHECK(handle = glCreateShader(type));
    if (!handle) {
        printf("%u: cannot create shader\n", type);
        return 0;
    }

    GL_CHECK(glShaderSource(handle, 1, &source, nullptr));
    GL_CHECK(glCompileShader(handle));
    GL_CHECK(glGetShaderiv(handle, GL_COMPILE_STATUS, &success));

    if (!success) {
        glGetShaderInfoLog(handle, sizeof(msg), nullptr, msg);
        printf("%u: %s\n", type, msg);
        glDeleteShader(handle);
        return 0;
    }

    return handle;
    */
    return 0;
}

GLShader::GLShader(const char *vertex, const char *fragment, int type) {

    /*
    GLuint vsh, fsh;
    std::string v, f;

#ifdef __SDL2_GLES__
    v = std::string("#version 100\n") + vertex;
    f = std::string("#version 100\n") + fragment;
#else
    v = std::string("#version 330\n") + vertex;
    f = std::string("#version 330\n") + fragment;
#endif

    GL_CHECK(vsh = createAndCompileShader(GL_VERTEX_SHADER, v.c_str()));
    if (!vsh) {
        return;
    }

    GL_CHECK(fsh = createAndCompileShader(GL_FRAGMENT_SHADER, f.c_str()));
    if (!fsh) {
        glDeleteShader(vsh);
        return;
    }

    GL_CHECK(program = glCreateProgram());
    GL_CHECK(glAttachShader(program, vsh));
    GL_CHECK(glAttachShader(program, fsh));
    GL_CHECK(glBindAttribLocation(program, 0, "positionAttribute"));
    GL_CHECK(glBindAttribLocation(program, 1, "colorAttribute"));
    GL_CHECK(glBindAttribLocation(program, 2, "texCoordAttribute"));
    GL_CHECK(glLinkProgram(program));

    GLint success;
    GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &success));
    if (!success) {
        char buf[512];
        glGetProgramInfoLog(program, sizeof(buf), nullptr, buf);
        printf("Link error: %s\n", buf);
    }

    GL_CHECK(glDeleteShader(vsh));
    GL_CHECK(glDeleteShader(fsh));

    scale_type = type;
    available = true;
    */
}

void GLShader::SetUniformMatrix(const GLchar *n, const GLfloat *v) {
    /*
    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(program, n));
    if (loc > -1) {
        GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, v));
    }
    */
}

void GLShader::SetUniform(const GLchar *n, const Vector2f &v) {
    /*
    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(program, n));
    if (loc > -1) {
        GL_CHECK(glUniform2f(loc, v.x, v.y));
    }
    */
}

void GLShader::SetUniform(const GLchar *n, const float v0, const float v1) {
    /*
    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(program, n));
    if (loc > -1) {
        GL_CHECK(glUniform2f(loc, v0, v1));
    }
    */
}

void GLShader::SetUniform(const GLchar *n, int v) {
    /*
    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(program, n));
    if (loc > -1) {
        GL_CHECK(glUniform1i(loc, v));
    }
    */
}

GLuint GLShader::GetProgram() {
    return program;
}

int GLShader::getScaleType() {
    return scale_type;
}

GLShader::~GLShader() {
    /*
    if (program) {
        GL_CHECK(glDeleteProgram(program));
    }
    */
}

GLShaderList::GLShaderList(const std::string &shadersPath) : ShaderList(shadersPath) {
    /*
    // add color shader
    auto *colorShader = new GLShader(color_v, color_f);
    color = new Shader("color", colorShader);
    get(0)->data = new GLShader(texture_v, texture_f);
    if (C2D_SCREEN_HEIGHT > 240) {
        add("retro v2", new GLShader(retro_v2_v, retro_v2_f));
        add("lcd3x", new GLShader(lcd3x_v, lcd3x_f));
    }
#ifndef __SDL2_GLES__
    add("scanlines", new GLShader(scanlines_v, scanlines_f));
    add("crt caligari", new GLShader(crt_caligari_v, crt_caligari_f));
    add("crt easymode", new GLShader(crt_easymode_v, crt_easymode_f));
    add("crt geom", new GLShader(crt_geom_v, crt_geom_f));
    add("crt lottes", new GLShader(crt_lottes_v, crt_lottes_f));
    add("sharp bilinear", new GLShader(sharp_bilinear_v, sharp_bilinear_f));
    add("sharp bilinear scanlines", new GLShader(sharp_bilinear_scanlines_v, sharp_bilinear_scanlines_f));
    add("sal2x", new GLShader(sal2x_v, sal2x_f));
    add("supereagle", new GLShader(supereagle_v, supereagle_f));
    add("sabr v3", new GLShader(sabr_v3_v, sabr_v3_f));
    add("xbrz freescale", new GLShader(xbrz_freescale_v, xbrz_freescale_f, GLShader::SCALE_TYPE_VIEWPORT));
#else
    add("sharp bilinear", new GLShader(sharp_bilinear_v, sharp_bilinear_f));
#endif
    */
}

GLShaderList::~GLShaderList() {

    if (color) {
        if (color->data != nullptr) {
            delete ((GLShader *) color->data);
        }
        delete (color);
    }

    for (int i = 0; i < getCount(); i++) {
        if (get(i)->data != nullptr) {
            delete ((GLShader *) get(i)->data);
        }
    }
}

#endif // __GL1__
