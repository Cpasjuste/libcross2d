//
// Created by cpasjuste on 12/12/16.
//

#include <3ds.h>
#include <cstdio>
#include <malloc.h>
#include "cross2d/platforms/3ds/ctr_audio.h"

using namespace c2d;

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
static const int THREAD_AFFINITY = -1;           // Execute thread on any core
static const int THREAD_STACK_SZ = 32 * 1024;    // 32kB stack for audio thread
static Thread threadId;
static LightEvent s_event;
static ndspWaveBuf s_waveBufs[3];
static int16_t *s_audioBuffer = nullptr;
volatile bool s_quit = false;
static LightLock lock;

// NDSP audio frame callback
// This signals the audioThread to decode more things
// once NDSP has played a sound frame, meaning that there should be
// one or more available waveBufs to fill with more data.
static void audioCallback(void *const nul_) {
    (void) nul_;  // Unused

    if (s_quit) { // Quit flag
        return;
    }

    LightEvent_Signal(&s_event);
}

// Audio thread
// This handles calling the decoder function to fill NDSP buffers as necessary
static void audioThread(void *data) {

    CTRAudio *audio = (CTRAudio *) data;

    while (!s_quit) {  // Whilst the quit flag is unset,
        // search our waveBufs and fill any that aren't currently
        // queued for playback (i.e, those that are 'done')
        for (size_t i = 0; i < ARRAY_SIZE(s_waveBufs); ++i) {
            if (s_waveBufs[i].status != NDSP_WBUF_DONE) {
                continue;
            }

            ndspWaveBuf *waveBuf = &s_waveBufs[i];

            LightLock_Lock(&lock);
            if (audio->getAudioBuffer()->space_filled() >= audio->getBufferSize() >> 1) {
                audio->getAudioBuffer()->pull((int16_t *) waveBuf->data_pcm16, audio->getBufferSize() >> 1);
                LightLock_Unlock(&lock);
                // Pass samples to NDSP
                waveBuf->nsamples = audio->getSamples();
                ndspChnWaveBufAdd(0, waveBuf);
                DSP_FlushDataCache(waveBuf->data_pcm16, audio->getBufferSize());
            } else {
                LightLock_Unlock(&lock);
            }
        }

        // Wait for a signal that we're needed again before continuing,
        // so that we can yield to other things that want to run
        // (Note that the 3DS uses cooperative threading)
        LightEvent_Wait(&s_event);
    }
}

CTRAudio::CTRAudio(int freq, float fps, C2DAudioCallback cb) : Audio(freq, fps, cb) {

    // Setup LightEvent for synchronisation of audioThread
    LightEvent_Init(&s_event, RESET_ONESHOT);
    LightLock_Init(&lock);

    Result rc = ndspInit();
    if (R_FAILED(rc)) {
        if ((R_SUMMARY(rc) == RS_NOTFOUND) && (R_MODULE(rc) == RM_DSP))
            printf("CTRAudio: DSP init failed: dspfirm.cdc missing!\n");
        else
            printf("CTRAudio: DSP init failed. Error code: 0x%lx\n", rc);
        available = false;
        return;
    }

    ndspChnReset(0);
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
    ndspChnSetRate(0, (float) freq);
    ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);

    // Setup waveBufs for NDSP
    s_audioBuffer = (int16_t *) linearAlloc(buffer_size * ARRAY_SIZE(s_waveBufs));
    memset(&s_waveBufs, 0, sizeof(s_waveBufs));
    int16_t *s_buffer = s_audioBuffer;
    for (size_t i = 0; i < ARRAY_SIZE(s_waveBufs); ++i) {
        s_waveBufs[i].data_vaddr = s_buffer;
        s_waveBufs[i].status = NDSP_WBUF_DONE;
        s_buffer += buffer_size / sizeof(s_buffer[0]);
    }

    // Set the ndsp sound frame callback which signals our audioThread
    ndspSetCallback(audioCallback, nullptr);

    // Spawn audio thread
    // Set the thread priority to the main thread's priority ...
    int32_t priority = 0x30;
    svcGetThreadPriority(&priority, CUR_THREAD_HANDLE);
    // ... then subtract 1, as lower number => higher actual priority ...
    priority -= 1;
    // ... finally, clamp it between 0x18 and 0x3F to guarantee that it's valid.
    priority = priority < 0x18 ? 0x18 : priority;
    priority = priority > 0x3F ? 0x3F : priority;

    // Start the thread, passing our opusFile as an argument.
    threadId = threadCreate(audioThread, this,
                            THREAD_STACK_SZ, priority,
                            THREAD_AFFINITY, false);

}

void CTRAudio::play(const void *data, int samples, bool sync) {

    if (available && !paused) {

        if (callback != nullptr) {
            return;
        }

        int size = (samples * channels * (int) sizeof(int16_t)) >> 1;

        if (sync) {
            while (audioBuffer->space_filled() > size) {
                svcSleepThread(100000);
            }
        } else {
            if (audioBuffer->space_empty() <= 0) {
                LightLock_Lock(&lock);
                audioBuffer->clear();
                LightLock_Unlock(&lock);
            }
        }

        LightLock_Lock(&lock);
        audioBuffer->push((int16_t *) data, size);
        LightLock_Unlock(&lock);
    }
}

void CTRAudio::reset() {

    if (available) {
        ndspChnReset(0);
    }

    Audio::reset();
}

void CTRAudio::pause(int pause) {

    Audio::pause(pause);

    if (available) {
        ndspChnSetPaused(0, pause == 1);
    }
}

int CTRAudio::getQueuedSize() {
    return audioBuffer->space_filled() << 1;
}

CTRAudio::~CTRAudio() {

    s_quit = true;
    LightEvent_Signal(&s_event);
    threadJoin(threadId, UINT64_MAX);
    threadFree(threadId);

    ndspChnReset(0);
    linearFree(s_audioBuffer);
    ndspExit();
}
