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
    int bus_id = (int) module;
    bool is_old = hosversionBefore(8, 0, 0);

    // restore stock clock
    if (new_hz <= 0) {
        switch (module) {
            case SwitchSys::Module::Cpu:
                new_hz = switch_stock_cpu_clock;
                if (!is_old) {
                    bus_id = PcvModuleId_CpuBus;
                }
                break;
            case SwitchSys::Module::Gpu:
                new_hz = switch_stock_gpu_clock;
                if (!is_old) {
                    bus_id = PcvModuleId_GPU;
                }
                break;
            case SwitchSys::Module::Emc:
                new_hz = switch_stock_emc_clock;
                if (!is_old) {
                    bus_id = PcvModuleId_EMC;
                }
                break;
            default:
                break;
        }
    }

    if (hosversionBefore(8, 0, 0)) {
        if (R_SUCCEEDED(pcvSetClockRate((PcvModule) bus_id, (u32) new_hz))) {
            return true;
        }
    } else {
        ClkrstSession session = {0};
        clkrstOpenSession(&session, (PcvModuleId) bus_id, 3);
        clkrstSetClockRate(&session, 1020000000);
        clkrstCloseSession(&session);
    }

    return false;
}
