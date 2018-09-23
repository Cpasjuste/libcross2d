//
// Created by cpasjuste on 17/09/18.
//

// vertex color shader
const char *color_v = R"text(
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

const char *color_f = R"text(
    #version 330 core

    in vec4 frontColor;
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(frontColor);
    }
)text";
