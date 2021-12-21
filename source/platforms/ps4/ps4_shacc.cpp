//
// Created by cpasjuste on 20/12/2021.
//

#include <libkernel.h>
#include <sys/mman.h>
#include <debugnet.h>
#include <cstring>

#include "cross2d/platforms/ps4/ps4_shacc.h"

using namespace c2d;

#define PIGLET_MODULE_NAME "libScePigletv2VSH.sprx"
#define MODULE_SHCOMP "/data/self/system/common/lib/libSceShaccVSH.sprx"
#define MODULE_SHADERS "/data/self/system/common/lib/libScePrecompiledShaders.sprx"

extern "C" int sceKernelGetModuleInfoByName(const char *name, SceKernelModuleInfo *info);

typedef void module_patch_cb_t(void *arg, uint8_t *base, uint64_t size);

static int modId;

static bool shaderCompilerGetModuleBase(const char *name, uint64_t *base, uint64_t *size, int level) {
    SceKernelModuleInfo moduleInfo;
    int ret;

    ret = sceKernelGetModuleInfoByName(name, &moduleInfo);
    if (ret) {
        debugNetPrintf(level, "ShaderCompiler::getModuleBase: sceKernelGetModuleInfoByName(%s) failed: 0x%08X\n", name,
                       ret);
        goto err;
    }

    if (base) {
        *base = (uint64_t) moduleInfo.segmentInfo[0].address;
    }
    if (size) {
        *size = moduleInfo.segmentInfo[0].size;
    }

    return true;

    err:
    return false;
}

static bool shaderCompilerPatchModule(const char *name, module_patch_cb_t *cb, void *arg, int level) {
    uint64_t base, size;
    int ret;

    if (!shaderCompilerGetModuleBase(name, &base, &size, level)) {
        debugNetPrintf(level, "ShaderCompiler::patchModule: getModuleBase return error\n");
        goto err;
    }
    debugNetPrintf(level, "ShaderCompiler::patchModule: module base=0x%08X size=%ld\n", base, size);

    ret = sceKernelMprotect((void *) base, size, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (ret) {
        debugNetPrintf(level, "ShaderCompiler::patchModule: sceKernelMprotect(%s) failed: 0x%08X\n", name, ret);
        goto err;
    }
    debugNetPrintf(level, "ShaderCompiler::patchModule: patching module\n");

    if (cb) {
        (*cb)(arg, (uint8_t *) base, size);
    }

    debugNetPrintf(level, "ShaderCompiler::patchModule: patching module done\n");

    return true;

    err:
    return false;
}

/* XXX: patches below are given for Piglet module from 4.74 Devkit PUP */
static void pgl_patches_cb(void *arg, uint8_t *base, uint64_t size) {
    /* Patch runtime compiler check */
    const uint8_t p_set_eax_to_1[] = {
            0x31, 0xC0, 0xFF, 0xC0, 0x90,
    };
    memcpy(base + 0x5451F, p_set_eax_to_1, sizeof(p_set_eax_to_1));

    /* Tell that runtime compiler exists */
    *(uint8_t *) (base + 0xB2DEC) = 0;
    *(uint8_t *) (base + 0xB2DED) = 0;
    *(uint8_t *) (base + 0xB2DEE) = 1;
    *(uint8_t *) (base + 0xB2E21) = 1;

    /* Inform Piglet that we have shader compiler module loaded */
    *(int32_t *) (base + 0xB2E24) = modId;
}

ShaderCompiler::ShaderCompiler() {

    // load piglet module for gles2
    modId = sceKernelLoadStartModule(MODULE_SHCOMP, 0, nullptr, 0, nullptr, nullptr);
    if (modId < 0) {
        debugNetPrintf(DEBUGNET_DEBUG, "ShaderCompiler: could not load module (0x%08x): %s\n", modId, MODULE_SHCOMP);
        return;
    }

    if (!shaderCompilerPatchModule(PIGLET_MODULE_NAME, &pgl_patches_cb, nullptr, DEBUGNET_DEBUG)) {
        sceKernelStopUnloadModule(modId, 0, nullptr, 0, nullptr, nullptr);
        modId = 0;
        debugNetPrintf(DEBUGNET_DEBUG, "ShaderCompiler: unable to patch piglet module.\n");
        return;
    }
}

bool ShaderCompiler::isAvailable() {
    return modId > 0;
}

ShaderCompiler::~ShaderCompiler() {
    if (modId > 0) {
        sceKernelStopUnloadModule(modId, 0, nullptr, 0, nullptr, nullptr);
    }
}
