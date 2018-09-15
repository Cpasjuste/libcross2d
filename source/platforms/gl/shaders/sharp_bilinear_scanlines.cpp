//
// Created by cpasjuste on 15/09/18.
//

#include "sharp_bilinear_scanlines.h"

// texture
const char *vertex_sharp_bilinear_scanlines = R"text(
    #version 330 core

    #define pi 3.141592654

    layout (location = 0) in vec2 positionAttribute;
    layout (location = 1) in vec4 colorAttribute;
    layout (location = 2) in vec2 texCoordAttribute;

    uniform mat4 modelViewMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 textureMatrix;

    uniform vec2 textureSize;
    uniform vec2 inputSize;
    uniform vec2 outputSize;

    out vec4 frontColor;
    out vec4 texCoord;

    out vec2 omega;
    out vec2 precalc_texel;
    out vec2 precalc_scale;

// vertex compatibility #defines
#define vTexCoord texCoord.xy
#define SourceSize vec4(textureSize, 1.0 / textureSize) // either TextureSize or InputSize
#define outsize vec4(outputSize, 1.0 / outputSize)

    void main()
    {
        gl_Position = projectionMatrix * (modelViewMatrix * vec4(positionAttribute.x, positionAttribute.y, 0.0, 1.0));
        frontColor = colorAttribute;
        texCoord = textureMatrix * vec4(texCoordAttribute, 0.0, 1.0);

        omega = vec2(pi * outputSize.x, 2.0 * pi * textureSize.y);
        precalc_texel = vTexCoord * SourceSize.xy;
        precalc_scale = max(floor(outsize.xy / inputSize.xy), vec2(1.0, 1.0));
    }
)text";

const char *fragment_sharp_bilinear_scanlines = R"text(
    #version 330

    #pragma parameter SCANLINE_BASE_BRIGHTNESS "Scanline Base Brightness" 0.60 0.0 1.0 0.01
    #pragma parameter SCANLINE_HORIZONTAL_MODULATION "Scanline Horizontal Modulation" 0.0 0.0 2.00 0.01
    #pragma parameter SCANLINE_VERTICAL_MODULATION "Scanline Vertical Modulation" 0.75 0.0 2.0 0.01

    in vec4 frontColor;
    in vec4 texCoord;
    in vec2 omega;

    uniform vec2 outputSize;
    uniform vec2 textureSize;
    uniform vec2 inputSize;

    uniform sampler2D texture;

    out vec4 fragColor;

    // fragment compatibility #defines
    #define Source texture
    #define vTexCoord texCoord.xy
    #define SourceSize vec4(textureSize, 1.0 / textureSize) // either TextureSize or InputSize
    #define outsize vec4(outputSize, 1.0 / outputSize)

    #define SCANLINE_BASE_BRIGHTNESS 0.60
    #define SCANLINE_HORIZONTAL_MODULATION 0.0
    #define SCANLINE_VERTICAL_MODULATION 0.75

    in vec2 precalc_texel;
    in vec2 precalc_scale;

    void main()
    {
        vec2 texel = precalc_texel;
        vec2 scale = precalc_scale;

        vec2 texel_floored = floor(texel);
        vec2 s = fract(texel);
        vec2 region_range = 0.5 - 0.5 / scale;

        // Figure out where in the texel to sample to get correct pre-scaled bilinear.
        // Uses the hardware bilinear interpolator to avoid having to sample 4 times manually.

        vec2 center_dist = s - 0.5;
        vec2 f = (center_dist - clamp(center_dist, -region_range, region_range)) * scale + 0.5;

        vec2 mod_texel = texel_floored + f;

        vec3 res = texture2D(Source, mod_texel / SourceSize.xy).xyz;

        // thick scanlines (thickness pre-calculated in vertex shader based on source resolution)
        vec2 sine_comp = vec2(SCANLINE_HORIZONTAL_MODULATION, SCANLINE_VERTICAL_MODULATION);

        vec3 scanline = res * (SCANLINE_BASE_BRIGHTNESS + dot(sine_comp * sin(vTexCoord * omega), vec2(1.0, 1.0)));
        fragColor = vec4(scanline.rgb, 1.0);
    }
)text";
