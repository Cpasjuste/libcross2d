//
// Created by cpasjuste on 26/11/18.
//

#include "cross2d/c2d.h"

using namespace c2d;

#ifndef NDEBUG

void initNxLink();

void deinitNxLink();

#endif

int switch_stock_cpu_clock = 0;
int switch_stock_gpu_clock = 0;
int switch_stock_emc_clock = 0;

SWITCHRenderer::SWITCHRenderer(const Vector2f &size) : SDL2Renderer(size) {

#ifndef NDEBUG
    initNxLink();
#endif

    printf("SWITCHRenderer(%ix%i)\n", (int) size.x, (int) size.y);
    romfsInit();

    if (R_SUCCEEDED(pcvInitialize())) {
        switch_stock_cpu_clock = SwitchSys::getClock(SwitchSys::Module::Cpu);
        switch_stock_gpu_clock = SwitchSys::getClock(SwitchSys::Module::Gpu);
        switch_stock_emc_clock = SwitchSys::getClock(SwitchSys::Module::Emc);
        printf("SWITCHRenderer(): clocks: cpu=%i, gpu=%i, emc=%i\n",
               switch_stock_cpu_clock, switch_stock_gpu_clock, switch_stock_emc_clock);
    }
}

SWITCHRenderer::~SWITCHRenderer() {

    printf("~SWITCHRenderer()\n");

    if (SwitchSys::getClock(SwitchSys::Module::Cpu) != switch_stock_cpu_clock) {
        if (switch_stock_cpu_clock > 0) {
            printf("~SWITCHRenderer(): restoring stock cpu clock (%i)\n", switch_stock_cpu_clock);
            SwitchSys::setClock(SwitchSys::Module::Cpu, switch_stock_cpu_clock);
        }
    }

    if (SwitchSys::getClock(SwitchSys::Module::Gpu) != switch_stock_gpu_clock) {
        if (switch_stock_gpu_clock > 0) {
            printf("~SWITCHRenderer(): restoring stock gpu clock (%i)\n", switch_stock_gpu_clock);
            SwitchSys::setClock(SwitchSys::Module::Gpu, switch_stock_gpu_clock);
        }
    }

    if (SwitchSys::getClock(SwitchSys::Module::Emc) != switch_stock_emc_clock) {
        if (switch_stock_emc_clock > 0) {
            printf("~SWITCHRenderer(): restoring stock emc clock (%i)\n", switch_stock_emc_clock);
            SwitchSys::setClock(SwitchSys::Module::Emc, switch_stock_emc_clock);
        }
    }

    pcvExit();

    romfsExit();

#ifndef NDEBUG
    deinitNxLink();
#endif
}
