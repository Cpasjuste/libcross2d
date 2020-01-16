//
// Created by cpasjuste on 21/11/16.
//

#include "cross2d/c2d.h"

#include "ds.h"
//#include <kos/blockdev.h>
//#include <ext2/fs_ext2.h>
//#include <fat/fs_fat.h>

using namespace c2d;

/*
enum { fsaa = 0 };
pvr_init_params_t params = {
        // Enable opaque and translucent polygons with size 16
        { PVR_BINSIZE_32, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_8 },

        // Vertex buffer size 512K
        1024 * 1024 * 1.5,

        0, fsaa
};
*/

DCRenderer::DCRenderer(const Vector2f &s) : GL1Renderer(s) {

    printf("DCRenderer\n");

    dbglog_set_level(DBG_WARNING);

    //InitSDCard();
    //InitIDE();

    /*
    fs_ext2_init();
    fs_fat_init();
    devices[0].inited = sd_init() == 0;
    mount(Device::Type::Sd, Device::Flag::ReadOnly);
    //devices[1].inited = g1_ata_init() == 0;
    //mount(Device::Type::Hdd, Device::Flag::ReadOnly);
    */

    glKosInit();
    glInit();

    available = true;

    printf("DCRenderer(GL)(%p): %ix%i\n", this, (int) s.x, (int) s.y);
}

void DCRenderer::flip(bool draw, bool inputs) {

    // call base class (draw childs)
    GL1Renderer::flip(draw, inputs);

    if (draw) {
        glKosSwapBuffers();
    }
}

void DCRenderer::delay(unsigned int ms) {
    thd_sleep((int) ms);
}

bool DCRenderer::mount(Device::Type type, Device::Flag flag) {

    /*
    kos_blockdev_t deviceBlock;
    uint8 deviceType;
    Device *device = type == Device::Type::Sd ? &devices[0] : &devices[1];

    if (!device->inited) {
        return false;
    }

    if (type == Device::Type::Sd) {
        if (sd_blockdev_for_partition(0, &deviceBlock, &deviceType) != 0) {
            return false;
        }
        device->path = "/sd";
        device->type = Device::Type::Sd;
        device->flag = flag;
    } else {
        if (g1_ata_blockdev_for_partition(0, 1, &deviceBlock, &deviceType) != 0) {
            if (g1_ata_blockdev_for_partition(0, 0, &deviceBlock, &deviceType) != 0) {
                return false;
            }
        }
        device->path = "/hdd";
        device->type = Device::Type::Hdd;
        device->flag = flag;
    }

    if (deviceType == 0x83) {
        if (fs_ext2_mount(device->path.c_str(), &deviceBlock,
                          flag == Device::Flag::ReadOnly ? FS_EXT2_MOUNT_READONLY : FS_EXT2_MOUNT_READWRITE) != 0) {
            return false;
        }
        device->format = Device::Format::Ext2;
    } else if (deviceType == 0x04 || deviceType == 0x06 || deviceType == 0x0B || deviceType == 0x0C) {
        if (fs_fat_mount(device->path.c_str(), &deviceBlock,
                         flag == Device::Flag::ReadOnly ? FS_FAT_MOUNT_READONLY : FS_FAT_MOUNT_READWRITE) != 0) {
            return false;
        }
        device->format = Device::Format::Fat;
    }

    device->mounted = true;
    */

    return true;
}

void DCRenderer::unmount(Device::Type type) {

    /*
    Device *device = type == Device::Type::Sd ? &devices[0] : &devices[1];
    if (device->inited && device->mounted) {
        if (device->format == Device::Format::Ext2) {
            fs_ext2_sync(device->path.c_str());
            fs_ext2_unmount(device->path.c_str());
        } else {
            fs_fat_sync(device->path.c_str());
            fs_fat_unmount(device->path.c_str());
        }
        device->mounted = false;
    }
    */
}

DCRenderer::~DCRenderer() {

    printf("~DCRenderer()\n");

    /*
    if (devices[1].inited) {
        unmount(Device::Type::Hdd);
        g1_ata_shutdown();
    }

    if (devices[0].inited) {
        unmount(Device::Type::Sd);
        sd_shutdown();
    }

    fs_fat_shutdown();
    fs_ext2_shutdown();
    */
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
