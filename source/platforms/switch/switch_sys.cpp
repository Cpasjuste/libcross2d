//
// Created by cpasjuste on 26/11/18.
//

#include "cross2d/platforms/switch/switch_sys.h"

using namespace c2d;

extern int switch_stock_cpu_clock;
extern int switch_stock_gpu_clock;
extern int switch_stock_emc_clock;

int SwitchSys::getClock(const SwitchSys::Module &module) {

    u32 hz;

    if (R_SUCCEEDED(pcvGetClockRate((PcvModule) module, &hz))) {
        return (int) hz;
    }

    return getClockStock(module);
}

int SwitchSys::getClockStock(const SwitchSys::Module &module) {

    switch (module) {
        case SwitchSys::Module::Cpu:
            return switch_stock_cpu_clock;
        case SwitchSys::Module::Gpu:
            return switch_stock_gpu_clock;
        case SwitchSys::Module::Emc:
            return switch_stock_emc_clock;
        default:
            return 0;
    }
}

bool SwitchSys::setClock(const SwitchSys::Module &module, int hz) {

    int new_hz = hz;

    // restore stock clock
    if (new_hz <= 0) {
        switch (module) {
            case SwitchSys::Module::Cpu:
                new_hz = switch_stock_cpu_clock;
                break;
            case SwitchSys::Module::Gpu:
                new_hz = switch_stock_gpu_clock;
                break;
            case SwitchSys::Module::Emc:
                new_hz = switch_stock_emc_clock;
                break;
            default:
                break;
        }
    }

    if (R_SUCCEEDED(pcvSetClockRate((PcvModule) module, (u32) new_hz))) {
        return true;
    }

    return false;
}
