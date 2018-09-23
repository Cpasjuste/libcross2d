//
// Created by cpasjuste on 17/09/18.
//

// texture
const char *texture_v = R"text(
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
        texCoord = textureMatrix * vec4(texCoordAttribute, 0.0, 1.0);
    }
)text";

const char *texture_f = R"text(
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
