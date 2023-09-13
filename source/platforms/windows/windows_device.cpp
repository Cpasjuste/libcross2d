//
// Created by cpasjuste on 04/02/2021.
//

#include "cross2d/c2d.h"
#include "windows.h"

using namespace c2d;

std::string WindowsDevice::getName() {
    return "windows";
}

Vector2i WindowsDevice::getDisplaySize() {
    return {
            GetSystemMetrics(SM_CXSCREEN),
            GetSystemMetrics(SM_CYSCREEN)
    };
}
