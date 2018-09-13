//
// Created by cpasjuste on 27/01/17.
//

#include "c2d.h"

using namespace c2d;

// vertex color shader
static const char *const vertexShaderSource = R"text(
    #version 330 core

    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec4 aColor;
    out vec4 ourColor;
    uniform mat4 mdlvMtx;
    uniform mat4 projMtx;

    void main()
    {
        vec4 pos = mdlvMtx * vec4(aPos.x, aPos.y, 1.0, 1.0);
        gl_Position = projMtx * pos;
        ourColor = aColor;
    }
)text";

static const char *const fragmentShaderSource = R"text(
    #version 330 core

    in vec4 ourColor;
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(ourColor);
    }
)text";

// @fincs: https://github.com/switchbrew/switch-examples/blob/master/graphics/opengl/simple_triangle/source/main.cpp#L197
static GLuint createAndCompileShader(GLenum type, const char *source) {

    GLint success;
    GLchar msg[512];

    GLuint handle = glCreateShader(type);
    if (!handle) {
        printf("%u: cannot create shader\n", type);
        return 0;
    }
    glShaderSource(handle, 1, &source, nullptr);
    glCompileShader(handle);
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(handle, sizeof(msg), nullptr, msg);
        printf("%u: %s\n", type, msg);
        glDeleteShader(handle);
        return 0;
    }

    return handle;
}

GLShader::GLShader(const char *vertex, const char *fragment) {

    GLuint vsh = createAndCompileShader(GL_VERTEX_SHADER, vertex);
    if (!vsh) {
        return;
    }

    GLuint fsh = createAndCompileShader(GL_FRAGMENT_SHADER, fragment);
    if (!fsh) {
        glDeleteShader(vsh);
        return;
    }

    program = glCreateProgram();
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char buf[512];
        glGetProgramInfoLog(program, sizeof(buf), nullptr, buf);
        printf("Link error: %s\n", buf);
    }

    glDeleteShader(vsh);
    glDeleteShader(fsh);

    available = true;
}

GLShader::~GLShader() {
    if (program) {
        glDeleteProgram(program);
    }
}

GLShaderList::GLShaderList(const std::string &shadersPath) : ShaderList(shadersPath) {

    auto *colorShader = new GLShader(vertexShaderSource, fragmentShaderSource);
    color = new Shader("color", colorShader);

    /*
    // parent class add a "NONE" shader, we set it to a simple texture shader
    get(0)->data = create((SceGxmProgram *) texture_v, (SceGxmProgram *) texture_f);

    add("lcd3x",
        create((SceGxmProgram *) lcd3x_v, (SceGxmProgram *) lcd3x_f));
    add("sharp+scan",
        create((SceGxmProgram *) sharp_bilinear_v, (SceGxmProgram *) sharp_bilinear_f));
    add("sharp",
        create((SceGxmProgram *) sharp_bilinear_simple_v, (SceGxmProgram *) sharp_bilinear_simple_f));

    printf("PSP2ShaderList: found %i shaders\n", getCount() - 1);
    */
}

GLShaderList::~GLShaderList() {

}
