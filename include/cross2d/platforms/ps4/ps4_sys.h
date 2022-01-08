//
// Created by cpasjuste on 06/01/2022.
//

#ifndef C2D_PS4_SYS_H
#define C2D_PS4_SYS_H

namespace c2d {
    class PS4Sys {
    public:
        static void dumpShaderBinary(int id, const char *name, const char *type);

        static void print(const char *fmt, ...);
    };
}

#endif //C2D_PS4_SYS_H
