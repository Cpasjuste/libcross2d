//
// Created by cpasjuste on 15/09/18.
//

#include "retro_v2.h"

// texture
const char *vertex_retro_v2 = R"text(
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

const char *fragment_retro_v2 = R"text(
    #version 330

    in vec4 frontColor;
    in vec4 texCoord;

    uniform int frameDirection;
    uniform int frameCount;
    uniform vec2 outputSize;
    uniform vec2 textureSize;
    uniform vec2 inputSize;

    uniform sampler2D texture;

    out vec4 fragColor;

#define Source texture
#define vTexCoord texCoord.xy

#define SourceSize vec4(textureSize, 1.0 / textureSize) // either TextureSize or InputSize
#define outsize vec4(outputSize, 1.0 / outputSize)

#define RETRO_PIXEL_SIZE 0.84

    void main()
    {
        // Reading the texel
        vec3 E = pow(texture2D(Source, vTexCoord).xyz, vec3(2.4));

        vec2 fp = fract(vTexCoord*SourceSize.xy);
        vec2 ps = inputSize.xy * outsize.zw;

        vec2 f = clamp(clamp(fp + 0.5*ps, 0.0, 1.0) - RETRO_PIXEL_SIZE, vec2(0.0), ps)/ps;

        float max_coord =  max(f.x, f.y);

        vec3 res = mix(E*(1.04+fp.x*fp.y), E*0.36, max_coord);

        // Product interpolation
        fragColor = vec4(clamp( pow(res, vec3(1.0 / 2.2)), 0.0, 1.0 ), 1.0);

        //fragColor = texture2D(texture, texCoord.xy) * frontColor;
    }
)text";
