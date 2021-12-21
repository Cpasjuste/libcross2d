//
// Created by cpasjuste on 20/12/2021.
//

#ifndef C2D_PS4_SHACC_H
#define C2D_PS4_SHACC_H

// from https://github.com/orbisdev/orbisdev-liborbis/blob/master/source/orbislink/orbislink.c

namespace c2d {
    class ShaderCompiler {
    public:
        ShaderCompiler();

        ~ShaderCompiler();

        bool isAvailable();
    };
}

#endif //C2D_PS4_SHACC_H
