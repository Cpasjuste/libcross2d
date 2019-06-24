//
// Created by cpasjuste on 17/09/18.
//

// vertex color shader
const char *color_v = R"text(
#if __VERSION__ >= 130
#define COMPAT_VARYING out
#define COMPAT_ATTRIBUTE in
#define COMPAT_TEXTURE texture
#define COMPAT_PRECISION
#else
// assume opengl es...
#define COMPAT_VARYING varying
#define COMPAT_ATTRIBUTE attribute
#define COMPAT_TEXTURE texture2D
#define COMPAT_PRECISION mediump
#endif

    COMPAT_ATTRIBUTE vec2 positionAttribute;
    COMPAT_ATTRIBUTE vec4 colorAttribute;

    uniform mat4 modelViewMatrix;
    uniform mat4 projectionMatrix;

    COMPAT_VARYING vec4 frontColor;

    void main()
    {
        gl_Position = projectionMatrix * (modelViewMatrix * vec4(positionAttribute.x, positionAttribute.y, 0.0, 1.0));
        frontColor = colorAttribute;
    }
)text";

const char *color_f = R"text(
#if __VERSION__ >= 130
#define COMPAT_VARYING in
#define COMPAT_TEXTURE texture
out vec4 fragColor;
#define COMPAT_PRECISION
#else
// assume opengl es...
#define COMPAT_VARYING varying
#define fragColor gl_FragColor
#define COMPAT_TEXTURE texture2D
#define COMPAT_PRECISION mediump
#endif

    COMPAT_VARYING COMPAT_PRECISION vec4 frontColor;

    void main()
    {
        fragColor = vec4(frontColor);
    }
)text";
