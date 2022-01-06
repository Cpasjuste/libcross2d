//
// Created by cpasjuste on 06/01/2022.
//

#include <cstdio>
#include <cstdarg>
#include <orbis/libkernel.h>

#include "cross2d/platforms/ps4/ps4_debug.h"

static char c2d_debug_str[512];

void c2d::PS4Debug::print(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(c2d_debug_str, 511, fmt, args);
    va_end(args);
    sceKernelDebugOutText(0, c2d_debug_str);
}
