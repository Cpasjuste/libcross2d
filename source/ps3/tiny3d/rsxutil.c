#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include <sysutil/video.h>
#include <rsx/gcm_sys.h>
#include <rsx/rsx.h>
#include <rsx/mm.h>
#include "commands.h"

#include "rsxutil.h"

gcmContextData *context; // Context to keep track of the RSX buffer.

videoResolution Video_Resolution; // Screen Resolution

u32 *Video_buffer[2]; // The buffer we will be drawing into
static u32 offset[2]; // The offset of the buffers in RSX memory

static u32 *depth_buffer; // Depth buffer.
static u32 depth_offset;

int Video_pitch;
static int depth_pitch;
static u32 zformat;

u8 Video_aspect;

// Initilize and rsx
void init_screen(int command_buffer, int z_method) {
    // Allocate a 1Mb buffer, alligned to a 1Mb boundary to be our shared IO memory with the RSX.
    void *host_addr = memalign(1024 * 1024, command_buffer);
    assert(host_addr != NULL);

    if (z_method) zformat = REALITY_TARGET_FORMAT_ZETA_Z24S8; else zformat = REALITY_TARGET_FORMAT_ZETA_Z16;

    // Initilise Reality, which sets up the command buffer and shared IO memory
    context = rsxInit(0x10000, command_buffer, host_addr);
    assert(context != NULL);

    videoState state;
    assert(videoGetState(0, 0, &state) == 0); // Get the state of the display
    assert(state.state == 0); // Make sure display is enabled

    // Get the current resolution
    assert(videoGetResolution(state.displayMode.resolution, &Video_Resolution) == 0);

    Video_pitch = 4 * ((Video_Resolution.width + 15) / 16) * 16; // each pixel is 4 bytes

    if (!z_method)
        // 16 bit float. Note it uses 1920 as minimun because i thinking to use buffer Z with setupRenderTarget2() with one surface > screen
        depth_pitch = 2 * ((Video_Resolution.width > 1920) ? (((Video_Resolution.width + 31) / 32) * 32) : 1920);
    else
        // 32 bit float. Note it uses 1920 as minimun because i thinking to use buffer Z with setupRenderTarget2() with one surface > screen
        depth_pitch = 4 * ((Video_Resolution.width > 1920) ? (((Video_Resolution.width + 15) / 16) * 16) : 1920);

    // Configure the buffer format to xRGB
    videoConfiguration vconfig;
    memset(&vconfig, 0, sizeof(videoConfiguration));
    vconfig.resolution = state.displayMode.resolution;
    vconfig.format = VIDEO_BUFFER_FORMAT_XRGB;
    vconfig.pitch = Video_pitch;
    Video_aspect = vconfig.aspect = state.displayMode.aspect;

    assert(videoConfigure(0, &vconfig, NULL, 0) == 0);
    assert(videoGetState(0, 0, &state) == 0);

    s32 buffer_size = Video_pitch * Video_Resolution.height;
    s32 depth_buffer_size;

    if (!z_method)
        // 16 bit float. Note it uses 1088 as minimun because i thinking to use buffer Z with setupRenderTarget2() with one surface > screen
        depth_buffer_size =
                depth_pitch * ((Video_Resolution.height > 1088) ? (((Video_Resolution.height + 31) / 32) * 32) : 1088);
    else
        // 32 bit float. Note it uses 1920 as minimun because i thinking to use buffer Z with setupRenderTarget2() with one surface > screen
        depth_buffer_size =
                depth_pitch * ((Video_Resolution.height > 1088) ? (((Video_Resolution.height + 15) / 16) * 16) : 1088);
    printf("buffers will be 0x%x bytes\n", buffer_size);

    gcmSetFlipMode(GCM_FLIP_VSYNC); // Wait for VSYNC to flip

    // Allocate two buffers for the RSX to draw to the screen (double buffering)
    Video_buffer[0] = rsxMemalign(64, buffer_size);
    Video_buffer[1] = rsxMemalign(64, buffer_size);
    assert(Video_buffer[0] != NULL && Video_buffer[1] != NULL);

    depth_buffer = rsxMemalign(64, depth_buffer_size);

    assert(rsxAddressToOffset(Video_buffer[0], &offset[0]) == 0);
    assert(rsxAddressToOffset(Video_buffer[1], &offset[1]) == 0);
    // Setup the display buffers
    assert(gcmSetDisplayBuffer(0, offset[0], Video_pitch, Video_Resolution.width, Video_Resolution.height) == 0);
    assert(gcmSetDisplayBuffer(1, offset[1], Video_pitch, Video_Resolution.width, Video_Resolution.height) == 0);

    assert(rsxAddressToOffset(depth_buffer, &depth_offset) == 0);

    gcmResetFlipStatus();
    flip(1);
    waitFlip();
}

void waitFlip() { // Block the PPU thread untill the previous flip operation has finished.
    while (1) {
        int ret = gcmGetFlipStatus();

        if (ret <= 0) break;
        usleep(200);
    }
    gcmResetFlipStatus();
}

void flip(s32 buffer) {
    assert(gcmSetFlip(context, buffer) == 0);
    rsxFlushBuffer(context);
    gcmSetWaitFlip(context); // Prevent the RSX from continuing until the flip has finished.
}

void setupRenderTarget(u32 currentBuffer) {
    // Set the color0 target to point at the offset of our current surface
    internal_reality_SetRenderSurface(context, REALITY_SURFACE_COLOR0, REALITY_RSX_MEMORY,
                                      offset[currentBuffer], Video_pitch);

    // Setup depth buffer
    internal_reality_SetRenderSurface(context, REALITY_SURFACE_ZETA, REALITY_RSX_MEMORY,
                                      depth_offset, depth_pitch);

    // Choose color0 as the render target and tell the rsx about the surface format.
    internal_reality_SelectRenderTarget(context, REALITY_TARGET_0,
                                        REALITY_TARGET_FORMAT_COLOR_A8R8G8B8 |
                                        zformat |
                                        REALITY_TARGET_FORMAT_TYPE_LINEAR,
                                        Video_Resolution.width, Video_Resolution.height, 0, 0);
}

void setupRenderTarget2(u32 rsx_offset, u32 pitch, u32 width, u32 height, u32 format) {
    // Set the color0 target to point at the offset of our current surface
    internal_reality_SetRenderSurface(context, REALITY_SURFACE_COLOR1, REALITY_RSX_MEMORY,
                                      rsx_offset, pitch);

    // Setup depth buffer
    internal_reality_SetRenderSurface(context, REALITY_SURFACE_ZETA, REALITY_RSX_MEMORY,
                                      depth_offset, depth_pitch);

    // Choose color0 as the render target and tell the rsx about the surface format.
    internal_reality_SelectRenderTarget(context, REALITY_TARGET_1,
                                        ((format) ? REALITY_TARGET_FORMAT_COLOR_A8R8G8B8
                                                  : REALITY_TARGET_FORMAT_COLOR_X1R5G5B5) |
                                        zformat |
                                        REALITY_TARGET_FORMAT_TYPE_LINEAR,
                                        width, height, 0, 0);
}
