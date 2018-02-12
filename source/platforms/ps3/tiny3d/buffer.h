#pragma once

#include <rsx/gcm_sys.h>
#include <rsx/rsx.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef assert
#define assert(x) if(!(x)) return
#endif


#define INTERNAL_COMMAND_LENGTH(context, len) if(context->current + len*4 > context->end) assert(rsxContextCallback(context, len) == 0)

#if 0
typedef union {
    float	 f;
    uint32_t u;
} ieee32;
#endif

// Internal function that you shouldn't use unless you are directly manulipting the buffer.
s32 __attribute__((noinline)) rsxContextCallback(gcmContextData *context, u32 count);

void commandBufferPut(gcmContextData *context, uint32_t value);

// Inline macros for writing command packets to the rsx buffer.
void inline extern internal_commandBufferPutCmd1(gcmContextData *context, uint32_t command, uint32_t v1) {
    uint32_t *buffer = context->current;
    *buffer++ = command | 1 << 18;
    *buffer++ = v1;
    context->current = buffer;
}

void inline extern internal_commandBufferPutCmd2(gcmContextData *context, uint32_t command, uint32_t v1, uint32_t v2) {
    uint32_t *buffer = context->current;
    *buffer++ = command | 2 << 18;
    *buffer++ = v1;
    *buffer++ = v2;
    context->current = buffer;
}

void inline extern
internal_commandBufferPutCmd3(gcmContextData *context, uint32_t command, uint32_t v1, uint32_t v2, uint32_t v3) {
    uint32_t *buffer = context->current;
    *buffer++ = command | 3 << 18;
    *buffer++ = v1;
    *buffer++ = v2;
    *buffer++ = v3;
    context->current = buffer;
}

void inline extern
internal_commandBufferPutCmd4(gcmContextData *context, uint32_t command, uint32_t v1, uint32_t v2, uint32_t v3,
                              uint32_t v4) {
    uint32_t *buffer = context->current;
    *buffer++ = command | 4 << 18;
    *buffer++ = v1;
    *buffer++ = v2;
    *buffer++ = v3;
    *buffer++ = v4;
    context->current = buffer;
}

void inline extern
internal_commandBufferPutCmd5(gcmContextData *context, uint32_t command, uint32_t v1, uint32_t v2, uint32_t v3,
                              uint32_t v4, uint32_t v5) {
    uint32_t *buffer = context->current;
    *buffer++ = command | 5 << 18;
    *buffer++ = v1;
    *buffer++ = v2;
    *buffer++ = v3;
    *buffer++ = v4;
    *buffer++ = v5;
    context->current = buffer;
}

void inline extern
internal_commandBufferPutCmd6(gcmContextData *context, uint32_t command, uint32_t v1, uint32_t v2, uint32_t v3,
                              uint32_t v4, uint32_t v5, uint32_t v6) {
    uint32_t *buffer = context->current;
    *buffer++ = command | 6 << 18;
    *buffer++ = v1;
    *buffer++ = v2;
    *buffer++ = v3;
    *buffer++ = v4;
    *buffer++ = v5;
    *buffer++ = v6;
    context->current = buffer;
}

void inline extern
internal_commandBufferPutCmd7(gcmContextData *context, uint32_t command, uint32_t v1, uint32_t v2, uint32_t v3,
                              uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7) {
    uint32_t *buffer = context->current;
    *buffer++ = command | 7 << 18;
    *buffer++ = v1;
    *buffer++ = v2;
    *buffer++ = v3;
    *buffer++ = v4;
    *buffer++ = v5;
    *buffer++ = v6;
    *buffer++ = v7;
    context->current = buffer;
}

void inline extern
internal_commandBufferPutCmd8(gcmContextData *context, uint32_t command, uint32_t v1, uint32_t v2, uint32_t v3,
                              uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8) {
    uint32_t *buffer = context->current;
    *buffer++ = command | 8 << 18;
    *buffer++ = v1;
    *buffer++ = v2;
    *buffer++ = v3;
    *buffer++ = v4;
    *buffer++ = v5;
    *buffer++ = v6;
    *buffer++ = v7;
    *buffer++ = v8;
    context->current = buffer;
}

#ifdef __cplusplus
}
#endif
