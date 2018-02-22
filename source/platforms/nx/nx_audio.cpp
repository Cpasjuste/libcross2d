//
// Created by cpasjuste on 12/12/16.
//

#include <cstdio>
#include <switch.h>

#include "c2d.h"

using namespace c2d;

extern "C" {
#include <switch/kernel/condvar.h>
}

static unsigned char *buffer_fba;
static int buffer_fba_size;
static int buffer_fba_len;

static int audio_pause = 0;
static int running = 1;

static Thread thread;
static AudioOutBuffer source_buffer;
static AudioOutBuffer released_buffer;

static Mutex mutex;
static CondVar sema;

static void audio_thread(void *arg) {

    printf("audio_thread started\n");

    while (running) {

        int ret = condvarWaitTimeout(&sema, (u64) (5 * 1000 * 1000) * 1000);
        if (ret != 0xEA01) {
            //printf("condvarWait: 0x%08X\n", ret);

            /*
            while (buffer_fba_size == 0 || audio_pause) {
                svcSleepThread(100000);
                if (!running) {
                    break;
                }
            }
            */

            // Prepare the audio data source buffer.
            source_buffer.next = 0;
            source_buffer.buffer = buffer_fba;
            source_buffer.buffer_size = (u64) buffer_fba_size;
            source_buffer.data_size = (u64) buffer_fba_len;
            source_buffer.data_offset = 0;

            // Play this buffer once.
            Result res = audoutPlayBuffer(&source_buffer, &released_buffer);
            //printf("audPlay: 0x%0x (len=0x%0x)\n", res, buffer_fba_len);

            buffer_fba_size = 0;
        }
    }

    printf("audio_thread exited\n");
}

NXAudio::NXAudio(int freq, int fps) : Audio(freq, fps) {

    if (!available) {
        return;
    }

    int ret = audoutInitialize();
    printf("audoutInitialize: 0x%x\n", ret);
    if (ret != 0) {
        available = false;
        return;
    }

    printf("Sample rate: 0x%x\n", audoutGetSampleRate());
    printf("Channel count: 0x%x\n", audoutGetChannelCount());
    printf("PCM format: 0x%x\n", audoutGetPcmFormat());
    printf("Device state: 0x%x\n", audoutGetDeviceState());

    // Start audio playback.
    ret = audoutStartAudioOut();
    printf("audoutStartAudioOut: 0x%x\n", ret);
    if (ret != 0) {
        audoutExit();
        available = false;
        return;
    }

    mutexInit(&mutex);
    condvarInit(&sema, &mutex);

    ret = threadCreate(&thread, audio_thread, NULL, 0x1000, 0x2B, -2);
    printf("threadCreate: %i\n", ret);
    if (ret != 0) {
        audoutStopAudioOut();
        audoutExit();
        available = false;
        return;
    }

    ret = threadStart(&thread);
    printf("threadStart: %i\n", ret);
    if (ret != 0) {
        audoutStopAudioOut();
        audoutExit();
        available = false;
        return;
    }
}

NXAudio::~NXAudio() {

    if (!available) {
        return;
    }

    running = false;
    condvarWakeOne(&sema);

    int ret = threadWaitForExit(&thread);
    printf("threadWaitForExit: %i\n", ret);

    ret = threadClose(&thread);
    printf("threadClose: %i\n", ret);

    ret = audoutStopAudioOut();
    printf("audoutStopAudioOut: %i\n", ret);

    audoutExit();
}

void NXAudio::Play() {

    if (!available) {
        return;
    }

    buffer_fba = (unsigned char *) buffer;
    buffer_fba_size = buffer_size;
    buffer_fba_len = buffer_len;
    condvarWakeOne(&sema);
}

void NXAudio::Pause(int pause) {

    if (!available) {
        return;
    }
    audio_pause = pause;
}
