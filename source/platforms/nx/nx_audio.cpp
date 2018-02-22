//
// Created by cpasjuste on 12/12/16.
//

#include <cstdio>
#include <switch.h>
#include <malloc.h>

#include "c2d.h"

using namespace c2d;

extern "C" {
#include <switch/kernel/condvar.h>
}

typedef struct NXAudioBuffer {
    unsigned char *buffer;
    int size;
    int sample_size;
    int len; // audio length
    int buffered;
    int read_pos;
    int write_pos;
} NXAudioBuffer;

static NXAudioBuffer audioBuffer;

typedef struct NXCond {
    Mutex mutex;
    CondVar var;
} NXCond;

static NXCond cond;
static Mutex mutex;

static int audio_pause = 0;
static int running = 1;

static Thread thread;
static AudioOutBuffer source_buffer;
static AudioOutBuffer released_buffer;

static void write_buffer(unsigned char *data, int len) {

    //printf("write_buffer: buffered=%i, rpos=%i, wpos=%i\n",
    //       audioBuffer.buffered, audioBuffer.read_pos, audioBuffer.write_pos);

    mutexLock(&mutex);

    for (int i = 0; i < len; i += 4) {

        while (audioBuffer.buffered == audioBuffer.size) {
            //printf("write_buffer: condvarWait (buffered = buffer size)\n");
            condvarWait(&cond.var);
        }

        *(int *) ((char *) (audioBuffer.buffer + audioBuffer.write_pos)) = *(int *) ((char *) (data + i));
        audioBuffer.write_pos = (audioBuffer.write_pos + 4) % audioBuffer.size;
        audioBuffer.buffered += 4;
    }

    //printf("write_buffer: mutexUnlock\n");
    mutexUnlock(&mutex);
    condvarWakeAll(&cond.var);
}

static void read_buffer(void *arg) {

    while (running) {

        //printf("read_buffer: condvarWait\n");
        condvarWait(&cond.var);
        mutexLock(&mutex);

        int len = 4000;

        //printf("read_buffer: buffered=%i, rpos=%i, wpos=%i\n",
        //       audioBuffer.buffered, audioBuffer.read_pos, audioBuffer.write_pos);

        if (audioBuffer.buffered >= len) {
            if (audioBuffer.read_pos + len <= audioBuffer.size) {
                source_buffer.next = 0;
                source_buffer.buffer = audioBuffer.buffer;
                source_buffer.buffer_size = (u64) audioBuffer.size;
                source_buffer.data_size = (u64) len;
                source_buffer.data_offset = (u64) audioBuffer.read_pos;
                audoutPlayBuffer(&source_buffer, &released_buffer);
            } else {
                // drop
            }

            audioBuffer.read_pos = (audioBuffer.read_pos + len) % audioBuffer.size;
            audioBuffer.buffered -= len;
        }

        mutexUnlock(&mutex);
        condvarWakeAll(&cond.var);
    }
}

NXAudio::NXAudio(int freq, int fps) : Audio(freq, fps) {

    if (!available) {
        return;
    }

    // Find the value which is slighly bigger than buffer_len*2
    for (audioBuffer.sample_size = 512; audioBuffer.sample_size < (buffer_len * 2); audioBuffer.sample_size <<= 1);
    audioBuffer.sample_size /= 4; // fix audio delay
    audioBuffer.len = buffer_len;
    audioBuffer.size = audioBuffer.sample_size * channels * 2 * 8;
    u32 size_aligned = (u32) (audioBuffer.size + 0xfff) & ~0xfff;
    audioBuffer.buffer = (unsigned char *) memalign(0x1000, size_aligned);
    memset(audioBuffer.buffer, 0, (size_t) size_aligned);

    audioBuffer.buffered = 0;
    audioBuffer.read_pos = 0;
    audioBuffer.write_pos = 0;

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
    mutexInit(&cond.mutex);
    condvarInit(&cond.var, &cond.mutex);

    ret = threadCreate(&thread, read_buffer, NULL, 0x1000, 0x2B, -2);
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
    condvarWakeAll(&cond.var);

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

    write_buffer((unsigned char *) buffer, buffer_size);
}

void NXAudio::Pause(int pause) {

    if (!available) {
        return;
    }
    audio_pause = pause;
}
