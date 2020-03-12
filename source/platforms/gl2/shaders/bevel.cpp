//
// Created by cpasjuste on 17/09/18.
//

const char *bevel_v = R"text(

// Parameter lines go here:
#pragma parameter BEVEL_LEVEL "Bevel Level" 0.2 0.0 0.5 0.01
#pragma parameter InputGamma "Input Gamma" 2.4 0.1 5.0 0.1
#pragma parameter OutputGamma "Output Gamma" 2.2 0.1 5.0 0.1

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

COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 COLOR;
COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 COL0;
COMPAT_VARYING vec4 TEX0;
// out variables go here as COMPAT_VARYING whatever

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
    gl_Position = MVPMatrix * VertexCoord;
    COL0 = COLOR;
    TEX0.xy = TexCoord.xy * 1.0000001;
}

)text";

const char *bevel_f = R"text(

// Parameter lines go here:
#pragma parameter BEVEL_LEVEL "Bevel Level" 0.2 0.0 0.5 0.01
#pragma parameter InputGamma "Input Gamma" 2.4 0.1 5.0 0.1
#pragma parameter OutputGamma "Output Gamma" 2.2 0.1 5.0 0.1

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
uniform COMPAT_PRECISION float BEVEL_LEVEL;
uniform COMPAT_PRECISION float InputGamma;
uniform COMPAT_PRECISION float OutputGamma;
#else
#define BEVEL_LEVEL 0.2
#define InputGamma 2.4
#define OutputGamma 2.2
#endif

#define GAMMA_IN(color)     pow(color, vec3(InputGamma, InputGamma, InputGamma))
#define GAMMA_OUT(color)    pow(color, vec3(1.0 / OutputGamma, 1.0 / OutputGamma, 1.0 / OutputGamma))

vec3 bevel(vec2 pos, vec3 color)
{
    vec3 weight;

    float r = sqrt(dot(pos, vec2(1.0)));

    vec3 delta = mix(vec3(BEVEL_LEVEL), vec3(1.0-BEVEL_LEVEL), color);

    weight = delta*(1.-r);

    return color+weight;
}

void main()
{
    vec2 position = fract(vTexCoord*SourceSize.xy);

    vec3 color = GAMMA_IN(COMPAT_TEXTURE(Source, vTexCoord).rgb);

    color = clamp(bevel(position, color), 0.0, 1.0);

   FragColor = vec4(GAMMA_OUT(color), 1.0);
}

)text";
