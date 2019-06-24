//
// Created by cpasjuste on 17/09/18.
//

const char *sal2x_v = R"text(

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

const char *sal2x_f = R"text(

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
    // in variables go here as COMPAT_VARYING whatever

    // fragment compatibility #defines
    #define Source Texture
    #define vTexCoord TEX0.xy

    #define SourceSize vec4(TextureSize, 1.0 / TextureSize) //either TextureSize or InputSize
    #define outsize vec4(OutputSize, 1.0 / OutputSize)

    void main()
    {
        vec2 texsize = SourceSize.xy;
        float dx     = pow(texsize.x, -1.0) * 0.25;
        float dy     = pow(texsize.y, -1.0) * 0.25;
        vec3  dt     = vec3(1.0, 1.0, 1.0);


        vec2 UL =    vTexCoord + vec2(-dx, -dy);
        vec2 UR =    vTexCoord + vec2( dx, -dy);
        vec2 DL =    vTexCoord + vec2(-dx,  dy);
        vec2 DR =    vTexCoord + vec2( dx,  dy);


        vec3 c00 = COMPAT_TEXTURE(Source, UL).xyz;
        vec3 c20 = COMPAT_TEXTURE(Source, UR).xyz;
        vec3 c02 = COMPAT_TEXTURE(Source, DL).xyz;
        vec3 c22 = COMPAT_TEXTURE(Source, DR).xyz;

        float m1 = dot(abs(c00 - c22), dt) + 0.001;
        float m2 = dot(abs(c02 - c20), dt) + 0.001;

        FragColor = vec4((m1*(c02 + c20) + m2*(c22 + c00))/(2.0*(m1 + m2)), 1.0);
    }
)text";
