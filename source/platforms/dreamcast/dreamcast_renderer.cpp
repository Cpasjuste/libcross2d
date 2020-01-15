//
// Created by cpasjuste on 21/11/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

DCRenderer::DCRenderer(const Vector2f &s) : GL1Renderer(s) {

    printf("DCRenderer\n");

    dbglog_set_level(DBG_WARNING);

    glKosInit();
    glInit();

    available = true;

    printf("DCRenderer(GL)(%p): %ix%i\n", this, (int) s.x, (int) s.y);
}

void DCRenderer::flip(bool draw, bool inputs) {

    // call base class (draw childs)
    GL1Renderer::flip(draw, inputs);

    if (draw) {
        glutSwapBuffers();
    }
}

void DCRenderer::delay(unsigned int ms) {
    thd_sleep((int) ms);
}

DCRenderer::~DCRenderer() {
    printf("~DCRenderer()\n");
}

/// crap
_Atomic_word __attribute__ ((__unused__))
__gnu_cxx::__exchange_and_add(volatile _Atomic_word *__mem, int __val) throw() {
    _Atomic_word __result;

    __asm__ __volatile__
    ("0:\n"
     "\tmovli.l\t@%2,r0\n"
     "\tmov\tr0,%1\n"
     "\tadd\t%3,r0\n"
     "\tmovco.l\tr0,@%2\n"
     "\tbf\t0b"
    : "+m" (*__mem), "=&r" (__result)
    : "r" (__mem), "rI08" (__val)
    : "r0");

    return __result;
}

void __attribute__ ((__unused__)) __gnu_cxx::__atomic_add(volatile _Atomic_word *__mem, int __val) throw() {
    asm("0:\n"
        "\tmovli.l\t@%1,r0\n"
        "\tadd\t%2,r0\n"
        "\tmovco.l\tr0,@%1\n"
        "\tbf\t0b"
    : "+m" (*__mem)
    : "r" (__mem), "rI08" (__val)
    : "r0");
}
