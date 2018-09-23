//
// Created by cpasjuste on 15/09/18.
//

// texture
const char *retro_v2_v = R"text(
    #version 330 core

    #if __VERSION__ >= 130
    #define COMPAT_VARYING out
    #define COMPAT_ATTRIBUTE in
    #define COMPAT_TEXTURE texture
    #else
    #define COMPAT_VARYING varying
    #define COMPAT_ATTRIBUTE attribute
    #define COMPAT_TEXTURE texture2D
    #endif

    #ifdef GL_ES
    #define COMPAT_PRECISION mediump
    #else
    #define COMPAT_PRECISION
    #endif

    // CROSS2D
    //COMPAT_ATTRIBUTE vec4 VertexCoord;
    //COMPAT_ATTRIBUTE vec4 COLOR;
    //COMPAT_ATTRIBUTE vec4 TexCoord;
    layout (location = 0) in vec4 VertexCoord;
    layout (location = 1) in vec4 COLOR;
    layout (location = 2) in vec4 TexCoord;
    uniform mat4 modelViewMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 textureMatrix;
    // CROSS2D
    COMPAT_VARYING vec4 COL0;
    COMPAT_VARYING vec4 TEX0;

    uniform mat4 MVPMatrix;
    uniform COMPAT_PRECISION int FrameDirection;
    uniform COMPAT_PRECISION int FrameCount;
    uniform COMPAT_PRECISION vec2 OutputSize;
    uniform COMPAT_PRECISION vec2 TextureSize;
    uniform COMPAT_PRECISION vec2 InputSize;

    // vertex compatibility #defines
    #define vTexCoord TEX0.xy
    #define SourceSize vec4(TextureSize, 1.0 / TextureSize) //either TextureSize or InputSize
    #define outsize vec4(OutputSize, 1.0 / OutputSize)

    void main()
    {
        // CROSS2D
        // gl_Position = MVPMatrix * VertexCoord;
        gl_Position = projectionMatrix * (modelViewMatrix * vec4(VertexCoord.x, VertexCoord.y, 0.0, 1.0));
        COL0 = COLOR;
        // CROSS2D
        // TEX0.xy = TexCoord.xy;
        TEX0 = textureMatrix * vec4(VertexCoord.x, VertexCoord.y, 0.0, 1.0);
    }

)text";

const char *retro_v2_f = R"text(
    #version 330

    #if __VERSION__ >= 130
    #define COMPAT_VARYING in
    #define COMPAT_TEXTURE texture
    out vec4 FragColor;
    #else
    #define COMPAT_VARYING varying
    #define FragColor gl_FragColor
    #define COMPAT_TEXTURE texture2D
    #endif

    #ifdef GL_ES
    #ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
    #else
    precision mediump float;
    #endif
    #define COMPAT_PRECISION mediump
    #else
    #define COMPAT_PRECISION
    #endif

    uniform COMPAT_PRECISION int FrameDirection;
    uniform COMPAT_PRECISION int FrameCount;
    uniform COMPAT_PRECISION vec2 OutputSize;
    uniform COMPAT_PRECISION vec2 TextureSize;
    uniform COMPAT_PRECISION vec2 InputSize;
    uniform sampler2D Texture;
    COMPAT_VARYING vec4 TEX0;

    // fragment compatibility #defines
    #define Source Texture
    #define vTexCoord TEX0.xy

    #define SourceSize vec4(TextureSize, 1.0 / TextureSize) //either TextureSize or InputSize
    #define outsize vec4(OutputSize, 1.0 / OutputSize)

    #ifdef PARAMETER_UNIFORM
    uniform COMPAT_PRECISION float RETRO_PIXEL_SIZE;
    #else
    #define RETRO_PIXEL_SIZE 0.84
    #endif

    void main()
    {
        // Reading the texel
        vec3 E = pow(COMPAT_TEXTURE(Source, vTexCoord).xyz, vec3(2.4));

        vec2 fp = fract(vTexCoord*SourceSize.xy);
        vec2 ps = InputSize.xy * outsize.zw;

        vec2 f = clamp(clamp(fp + 0.5*ps, 0.0, 1.0) - RETRO_PIXEL_SIZE, vec2(0.0), ps)/ps;

        float max_coord =  max(f.x, f.y);

        vec3 res = mix(E*(1.04+fp.x*fp.y), E*0.36, max_coord);

        // Product interpolation
        FragColor = vec4(clamp( pow(res, vec3(1.0 / 2.2)), 0.0, 1.0 ), 1.0);
    }
)text";


