//
// Created by cpasjuste on 12/12/16.
//

#include <cstdio>
#include <switch.h>
#include <malloc.h>

#include "c2d.h"

using namespace c2d;

#define NO_MEMCPY 0

typedef struct NXAudioBuffer {
    unsigned char *read_buffer;
    int read_buffer_size;
    unsigned char *buffer;
    int len;
    int size;
    int buffered;
    int write_pos;
    int read_pos;
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

static void read_buffer(unsigned char *data, int len) {

    //printf("write_buffer: mutexLock\n");
    //mutexLock(&mutex);

    for (int i = 0; i < len; i += 4) {

        while (audioBuffer.buffered == audioBuffer.read_buffer_size) {
            // oops, audio thread too slow, drop audio
            // printf("write_buffer: buffered = buffer size\n");
            return;
            //condvarWait(&cond.var);
        }

        *(int *) ((char *) (audioBuffer.read_buffer + audioBuffer.read_pos)) = *(int *) ((char *) (data + i));
        audioBuffer.read_pos = (audioBuffer.read_pos + 4) % audioBuffer.read_buffer_size;
        audioBuffer.buffered += 4;
    }

    //printf("write: buffered=%i, pos=%i\n",
    //       audioBuffer.buffered, audioBuffer.write_pos);
    //mutexUnlock(&mutex);
}

static void write_buffer(void *arg) {

    printf("audio thread started\n");

    while (running) {

        if (!running) {
            break;
        }

        int len = audioBuffer.size;

        if (audioBuffer.buffered >= len) {

            source_buffer.next = 0;
            //source_buffer.buffer_size = (u64) audioBuffer.len;
            source_buffer.buffer_size = (u64) 512; // NICE
            source_buffer.data_size = (u64) audioBuffer.size;
            source_buffer.data_offset = (u64) 0;
            if (audioBuffer.write_pos + len < audioBuffer.read_buffer_size) {
#if NO_MEMCPY
                source_buffer.buffer = audioBuffer.read_buffer + audioBuffer.write_pos;
#else
                memcpy(audioBuffer.buffer,
                       audioBuffer.read_buffer + audioBuffer.write_pos, (size_t) len);
                source_buffer.buffer = audioBuffer.buffer;
#endif
            } else {
                // should not happen if we're all good
                // printf("read_buffer: pos + len > size !!!\n");
#if NO_MEMCPY
                source_buffer.buffer = audioBuffer.read_buffer;
#else
                int tail = audioBuffer.read_buffer_size - audioBuffer.write_pos;
                memcpy(audioBuffer.buffer,
                       audioBuffer.read_buffer + audioBuffer.write_pos, (size_t) tail);
                memcpy(audioBuffer.buffer + tail, audioBuffer.read_buffer, (size_t) len - tail);
                source_buffer.buffer = audioBuffer.buffer;
#endif
            }

            mutexLock(&mutex);
            audoutPlayBuffer(&source_buffer, &released_buffer);
            mutexUnlock(&mutex);

            audioBuffer.write_pos = (audioBuffer.write_pos + len) % audioBuffer.read_buffer_size;
            audioBuffer.buffered -= len;

        } else {
            // should not happen as soon as audio is playing
            // printf("buffered < len\n");
            svcSleepThread(1000000 * 16); // 16 ms
        }

        condvarWakeOne(&cond.var);
    }

    printf("audio thread ended\n");
}

NXAudio::NXAudio(int freq, int fps) : Audio(freq, fps) {

    if (!available) {
        return;
    }

    audioBuffer.buffer = NULL;
    audioBuffer.read_buffer = NULL;

    if (buffer) {
        free(buffer);
    }
    buffer_len = 1024;
    buffer_size = buffer_len * channels * 2;
    buffer = (short *) malloc((size_t) buffer_size);
    memset(buffer, 0, (size_t) buffer_size);
#if !NO_MEMCPY
    u32 s = (u32) (buffer_size + 0xfff) & ~0xfff;
    audioBuffer.buffer = (unsigned char *) memalign(0x1000, s);
    memset(audioBuffer.buffer, 0, (size_t) s);
#endif
    audioBuffer.size = buffer_size;
    audioBuffer.len = buffer_len;
    audioBuffer.read_buffer_size = buffer_size * 8;
    u32 size_aligned = (u32) (audioBuffer.read_buffer_size + 0xfff) & ~0xfff;
    audioBuffer.read_buffer = (unsigned char *) memalign(0x1000, size_aligned);
    memset(audioBuffer.read_buffer, 0, (size_t) size_aligned);

    audioBuffer.buffered = 0;
    audioBuffer.write_pos = 0;
    audioBuffer.read_pos = 0;

    printf("NXAudio: rate: %i, buf size: %i, buf len: %i, stream buf size: %i\n",
           frequency, audioBuffer.size, audioBuffer.len, audioBuffer.read_buffer_size);
    //printf("Sample rate: 0x%x\n", audoutGetSampleRate());
    //printf("Channel count: 0x%x\n", audoutGetChannelCount());
    //printf("PCM format: 0x%x\n", audoutGetPcmFormat());
    printf("audio device state: 0x%x\n", audoutGetDeviceState());

    // init audio (audio can't be reloaded properly ?!)
    int ret = audoutInitialize();
    printf("audoutInitialize: 0x%x\n", ret);

    // Start audio playback.
    ret = audoutStartAudioOut();
    printf("audoutStartAudioOut: 0x%x\n", ret);
    if (ret != 0) {
        available = false;
        return;
    }

    //mutexInit(&mutex);
    //mutexInit(&cond.mutex);
    //condvarInit(&cond.var, &cond.mutex);

    ret = threadCreate(&thread, write_buffer, NULL, 0x1000, 0x20, -2);
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

    //ret = audoutWaitPlayFinish(&released_buffer, U64_MAX);
    //printf("audoutWaitPlayFinish: %i\n", ret);
    ret = audoutStopAudioOut();
    printf("audoutStopAudioOut: %i\n", ret);
    audoutExit();

    if (audioBuffer.buffer) {
        free(audioBuffer.buffer);
    }
    if (audioBuffer.read_buffer) {
        free(audioBuffer.read_buffer);
    }
}

void NXAudio::Play() {

    if (!available) {
        return;
    }

    read_buffer((unsigned char *) buffer, buffer_size);
}

void NXAudio::Pause(int pause) {

    if (!available) {
        return;
    }

    audio_pause = pause;
}
