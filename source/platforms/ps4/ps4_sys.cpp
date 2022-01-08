//
// Created by cpasjuste on 06/01/2022.
//

#include <cstdio>
#include <cstdarg>
#include <orbis/libkernel.h>
#include <orbis/Pigletv2VSH.h>

#include "cross2d/platforms/ps4/ps4_sys.h"

using namespace c2d;

extern "C" void
glPigletGetShaderBinarySCE(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);

static char c2d_debug_str[512];

void PS4Sys::print(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(c2d_debug_str, 511, fmt, args);
    va_end(args);
    sceKernelDebugOutText(0, c2d_debug_str);
}

void PS4Sys::dumpShaderBinary(int id, const char *name, const char *type) {
    GLenum format = 0;
    char buf[16384];
    int size;

    glPigletGetShaderBinarySCE((GLuint) id, 16383, &size, &format, &buf);
    PS4Sys::print("dumpShaderBinary: glPigletGetShaderBinarySCE: read: %i\n", size);
    if (size > 0) {
        char path[256];
        sprintf(path, "/data/%s_%s.bin", name, type);
        PS4Sys::print("dumpShaderBinary: dumping shader to: %s\n", path);
        FILE *fp = fopen(path, "w+");
        fwrite(buf, 1, size, fp);
        fclose(fp);
    }
}
