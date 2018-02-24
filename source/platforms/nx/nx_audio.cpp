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

#define BUFFERED_AUDIO 0

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

static AudioOutBuffer source_buffer;
static AudioOutBuffer released_buffer;

static NXCond cond;
static Mutex mutex;
static Thread thread;

static int audio_pause = 0;
static int running = 1;

static void write_buffer(unsigned char *data, int len) {

#if !BUFFERED_AUDIO
    condvarWakeOne(&cond.var);
#else
    mutexLock(&mutex);

    for (int i = 0; i < len; i += 4) {

        while (audioBuffer.buffered == audioBuffer.size) {
            printf("write_buffer: condvarWait (buffered = buffer size)\n");
            //mutexUnlock(&mutex);
            condvarWait(&cond.var);
            //mutexLock(&mutex);
        }

        *(int *) ((char *) (audioBuffer.buffer + audioBuffer.write_pos)) = *(int *) ((char *) (data + i));
        audioBuffer.write_pos = (audioBuffer.write_pos + 4) % audioBuffer.size;
        audioBuffer.buffered += 4;
    }

    //printf("write: buffered=%i, pos=%i\n",
    //       audioBuffer.buffered, audioBuffer.write_pos);

    mutexUnlock(&mutex);
    //condvarWakeOne(&cond.var);
#endif
}

static void read_buffer(void *arg) {

    while (running) {

#if !BUFFERED_AUDIO
        condvarWait(&cond.var);
        if (!running) {
            break;
        }

        source_buffer.next = 0;
        source_buffer.buffer = audioBuffer.buffer;
        source_buffer.buffer_size = (u64) audioBuffer.size;
        source_buffer.data_size = (u64) audioBuffer.len;
        source_buffer.data_offset = (u64) 0;
        audoutPlayBuffer(&source_buffer, &released_buffer);
#else
        //printf("read_buffer: condvarWait\n");
        //condvarWait(&cond.var);

        int len = audioBuffer.sample_size; //3200; // audioBuffer.sample_size * 8; // audioBuffer.sample_size;

        if (!audio_pause && audioBuffer.buffered >= len) {
            if (audioBuffer.read_pos + len <= audioBuffer.size) {
                //printf("read: buffered=%i, pos=%i\n",
                //       audioBuffer.buffered, audioBuffer.read_pos);
                source_buffer.next = 0;
                source_buffer.buffer = audioBuffer.buffer;
                source_buffer.buffer_size = (u64) audioBuffer.size;
                source_buffer.data_size = (u64) len;
                source_buffer.data_offset = (u64) audioBuffer.read_pos;
                mutexLock(&mutex);
                audoutPlayBuffer(&source_buffer, &released_buffer);
                mutexUnlock(&mutex);
            } else {
                // drop
                printf("read_buffer: read_pos + len > audioBuffer.size\n");
                svcSleepThread(1000000);
            }
            audioBuffer.read_pos = (audioBuffer.read_pos + len) % audioBuffer.size;
            audioBuffer.buffered -= len;
        } else {
            // should not happen as soon as audio is playing
            //printf("read_buffer: audio_pause || buffered < len\n");
            svcSleepThread(1000000);
        }

        //mutexUnlock(&mutex);
        condvarWakeOne(&cond.var);
#endif
    }
}

NXAudio::NXAudio(int freq, int fps) : Audio(freq, fps) {

    if (!available) {
        return;
    }

    if (buffer) {
        free(buffer);
    }
    buffer_len = audioBuffer.len = 2048;
    buffer_size = buffer_len * 4 * 2;
    u32 size_aligned = (u32) (buffer_size + 0xfff) & ~0xfff;
    buffer = (short *) memalign(0x1000, size_aligned);
    memset(buffer, 0, (size_t) size_aligned);
#if !BUFFERED_AUDIO
    audioBuffer.len = buffer_len;
    audioBuffer.size = buffer_size;
    audioBuffer.buffer = (unsigned char *) buffer;
#else
    //audioBuffer.sample_size = 512;
    //printf("sample size: %i\n", audioBuffer.sample_size);
    audioBuffer.size = buffer_size * 10; // audioBuffer.sample_size * channels * 2 * 8;
    u32 size_aligned = (u32) (audioBuffer.size + 0xfff) & ~0xfff;
    audioBuffer.buffer = (unsigned char *) memalign(0x1000, size_aligned);
    memset(audioBuffer.buffer, 0, (size_t) size_aligned);
    audioBuffer.sample_size = 2048;
    audioBuffer.buffered = 0;
    audioBuffer.read_pos = 0;
    audioBuffer.write_pos = 0;
#endif

    printf("Sample rate: 0x%x\n", audoutGetSampleRate());
    printf("Channel count: 0x%x\n", audoutGetChannelCount());
    printf("PCM format: 0x%x\n", audoutGetPcmFormat());
    printf("Device state: 0x%x\n", audoutGetDeviceState());

    // Start audio playback.
    int ret = audoutStartAudioOut();
    printf("audoutStartAudioOut: 0x%x\n", ret);
    if (ret != 0) {
        available = false;
        return;
    }

    mutexInit(&mutex);
    mutexInit(&cond.mutex);
    condvarInit(&cond.var, &cond.mutex);

    ret = threadCreate(&thread, read_buffer, NULL, 0x1000, 0x2B, -2);
    printf("threadCreate: 0x%x\n", ret);
    if (ret != 0) {
        audoutStopAudioOut();
        available = false;
        return;
    }

    ret = threadStart(&thread);
    printf("threadStart: %i\n", ret);
    if (ret != 0) {
        audoutStopAudioOut();
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
