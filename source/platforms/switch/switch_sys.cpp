//
// Created by cpasjuste on 26/11/18.
//

#include "cross2d/platforms/switch/switch_sys.h"

using namespace c2d;

int SwitchSys::stock_cpu_clock = 0;
int SwitchSys::stock_gpu_clock = 0;
int SwitchSys::stock_emc_clock = 0;

int SwitchSys::getClock(const SwitchSys::Module &module, bool stockClocks) {

    u32 hz = 0;

#ifdef NDEBUG
    int bus_id = 0;
    bool is_old = hosversionBefore(8, 0, 0);

    if (stockClocks) {
        switch (module) {
            case SwitchSys::Module::Cpu:
                return stock_cpu_clock;
            case SwitchSys::Module::Gpu:
                return stock_gpu_clock;
            case SwitchSys::Module::Emc:
                return stock_emc_clock;
            default:
                return 0;
        }
    }

    switch (module) {
        case SwitchSys::Module::Cpu:
            bus_id = is_old ? (int) module : (int) PcvModuleId_CpuBus;
            break;
        case SwitchSys::Module::Gpu:
            bus_id = is_old ? (int) module : (int) PcvModuleId_GPU;
            break;
        case SwitchSys::Module::Emc:
            bus_id = is_old ? (int) module : (int) PcvModuleId_EMC;
            break;
        default:
            break;
    }

    if (hosversionBefore(8, 0, 0)) {
        pcvGetClockRate((PcvModule) bus_id, &hz);
    } else {
        ClkrstSession session = {0};
        clkrstOpenSession(&session, (PcvModuleId) bus_id, 3);
        clkrstGetClockRate(&session, &hz);
        clkrstCloseSession(&session);
    }
#endif

    return (int) hz;
}

bool SwitchSys::setClock(const SwitchSys::Module &module, int hz) {

#ifdef NDEBUG
    int new_hz = hz;
    int bus_id = (int) module;
    bool is_old = hosversionBefore(8, 0, 0);

    switch (module) {
        case SwitchSys::Module::Cpu:
            new_hz = new_hz <= 0 ? stock_cpu_clock : new_hz;
            bus_id = is_old ? (int) module : (int) PcvModuleId_CpuBus;
            break;
        case SwitchSys::Module::Gpu:
            new_hz = new_hz <= 0 ? stock_gpu_clock : new_hz;
            bus_id = is_old ? (int) module : (int) PcvModuleId_GPU;
            break;
        case SwitchSys::Module::Emc:
            new_hz = new_hz <= 0 ? stock_emc_clock : new_hz;
            bus_id = is_old ? (int) module : (int) PcvModuleId_EMC;
            break;
        default:
            break;
    }

    if (is_old) {
        if (R_SUCCEEDED(pcvSetClockRate((PcvModule) bus_id, (u32) new_hz))) {
            return true;
        }
    } else {
        ClkrstSession session = {0};
        clkrstOpenSession(&session, (PcvModuleId) bus_id, 3);
        clkrstSetClockRate(&session, hz);
        clkrstCloseSession(&session);
    }
#endif

    return false;
}
