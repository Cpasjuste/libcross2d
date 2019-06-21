//
// Created by cpasjuste on 17/09/18.
//

const char *texture_v = R"text(
#if __VERSION__ >= 130
#define COMPAT_VARYING out
#define COMPAT_ATTRIBUTE in
#define COMPAT_TEXTURE texture
#else
#define COMPAT_VARYING varying
#define COMPAT_ATTRIBUTE attribute
#define COMPAT_TEXTURE texture2D
#endif
#ifdef GL_OES_standard_derivatives
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif

    COMPAT_ATTRIBUTE vec2 positionAttribute;
    COMPAT_ATTRIBUTE vec2 texCoordAttribute;
    COMPAT_ATTRIBUTE vec4 colorAttribute;

    uniform mat4 modelViewMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 textureMatrix;

    COMPAT_VARYING vec4 frontColor;
    COMPAT_VARYING vec4 texCoord;

    void main()
    {
        gl_Position = projectionMatrix * (modelViewMatrix * vec4(positionAttribute.x, positionAttribute.y, 0.0, 1.0));
        frontColor = colorAttribute;
        texCoord = textureMatrix * vec4(texCoordAttribute, 0.0, 1.0);
    }
)text";

const char *texture_f = R"text(
#if __VERSION__ >= 130
#define COMPAT_VARYING in
#define COMPAT_TEXTURE texture
out vec4 fragColor;
#else
#define COMPAT_VARYING varying
#define fragColor gl_FragColor
#define COMPAT_TEXTURE texture2D
#endif
#ifdef GL_OES_standard_derivatives
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif

    COMPAT_VARYING COMPAT_PRECISION vec4 frontColor;
    COMPAT_VARYING COMPAT_PRECISION vec4 texCoord;
    uniform sampler2D tex;

    void main()
    {
        fragColor = COMPAT_TEXTURE(tex, texCoord.xy) * frontColor;
    }
)text";
