//
// Created by cpasjuste on 27/01/17.
//

#include "c2d.h"

using namespace c2d;

// vertex color shader
static const char *const vertexColor = R"text(
    #version 330 core

    layout (location = 0) in vec2 positionAttribute;
    layout (location = 1) in vec4 colorAttribute;

    uniform mat4 modelViewMatrix;
    uniform mat4 projectionMatrix;

    out vec4 frontColor;

    void main()
    {
        gl_Position = projectionMatrix * (modelViewMatrix * vec4(positionAttribute.x, positionAttribute.y, 0.0, 1.0));
        frontColor = colorAttribute;
    }
)text";

static const char *const fragmentColor = R"text(
    #version 330 core

    in vec4 frontColor;
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(frontColor);
    }
)text";

// texture
static const char *const vertexTexture = R"text(
    #version 330 core

    layout (location = 0) in vec2 positionAttribute;
    layout (location = 1) in vec4 colorAttribute;
    layout (location = 2) in vec2 texCoordAttribute;

    uniform mat4 modelViewMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 textureMatrix;

    out vec4 frontColor;
    out vec4 texCoord;

    void main()
    {
        gl_Position = projectionMatrix * (modelViewMatrix * vec4(positionAttribute.x, positionAttribute.y, 0.0, 1.0));

        frontColor = colorAttribute;

        //vec4 coord = vec4(texCoordAttribute, 1.0, 1.0) * textureMatrix;
        texCoord = textureMatrix * vec4(texCoordAttribute, 0.0, 1.0);
    }
)text";

static const char *const fragmentTexture = R"text(
    #version 330

    in vec4 frontColor;
    in vec4 texCoord;

    uniform sampler2D texture;

    out vec4 fragColor;

    void main()
    {
        fragColor = texture2D(texture, texCoord.xy) * frontColor;
    }
)text";

// @fincs: https://github.com/switchbrew/switch-examples/blob/master/graphics/opengl/simple_triangle/source/main.cpp#L197
static GLuint createAndCompileShader(GLenum type, const char *source) {

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
}

GLShader::GLShader(const char *vertex, const char *fragment) {

    GLuint vsh, fsh;

    GL_CHECK(vsh = createAndCompileShader(GL_VERTEX_SHADER, vertex));
    if (!vsh) {
        return;
    }

    GL_CHECK(fsh = createAndCompileShader(GL_FRAGMENT_SHADER, fragment));
    if (!fsh) {
        glDeleteShader(vsh);
        return;
    }

    GL_CHECK(program = glCreateProgram());
    GL_CHECK(glAttachShader(program, vsh));
    GL_CHECK(glAttachShader(program, fsh));
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

    available = true;
}

void GLShader::SetUniformMatrix(const GLchar *name, const GLfloat *value) {

    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(program, name));
    GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, value));
}

GLuint GLShader::GetProgram() {

    return program;
}

GLShader::~GLShader() {
    if (program) {
        GL_CHECK(glDeleteProgram(program));
    }
}

GLShaderList::GLShaderList(const std::string &shadersPath) : ShaderList(shadersPath) {

    auto *colorShader = new GLShader(vertexColor, fragmentColor);
    color = new Shader("color", colorShader);

    get(0)->data = new GLShader(vertexTexture, fragmentTexture);
}

GLShaderList::~GLShaderList() {

    if (color) {
        delete (color);
        color = nullptr;
    }

    for (int i = 0; i < getCount(); i++) {
        if (get(i)->data != nullptr) {
            delete ((GLShader *) get(i)->data);
            get(i)->data = nullptr;
        }
    }
}
