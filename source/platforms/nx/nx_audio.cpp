//
// Created by cpasjuste on 12/12/16.
//

#include <cstdio>
#include <switch.h>
#include <malloc.h>

#include "c2d.h"

using namespace c2d;

static NXAudioBuffer audioBuffer;
static Thread thread;
static int running = 1;
static int _paused = 0;
static Mutex mutex;

static void read_buffer(unsigned char *data, int len) {

    mutexLock(&mutex);

    for (int i = 0; i < len; i += 4) {

        if (audioBuffer.buffered == audioBuffer.read_buffer_size) {
            // oups
            printf("write_buffer: buffered = buffer size\n");
            break;
        }

        //*(int *) ((char *) (audioBuffer.read_buffer + audioBuffer.read_pos)) = *(int *) ((char *) (data + i));
        memcpy(audioBuffer.read_buffer + audioBuffer.read_pos, data + i, 4);
        audioBuffer.read_pos = (audioBuffer.read_pos + 4) % audioBuffer.read_buffer_size;
        audioBuffer.buffered += 4;
    }

    mutexUnlock(&mutex);
}

static void write_buffer(void *arg) {

    printf("NXAudio: audio thread started\n");

    int len = 2048 * 4; // audioBuffer.size; // 2048 * 4;

    for (int i = 0; i < 2; i++) {
        u32 size = (u32) (len + 0xfff) & ~0xfff;
        audioBuffer.buffer[i] = memalign(0x1000, size);
        audioBuffer.source_buffer[i].next = NULL;
        audioBuffer.source_buffer[i].buffer = audioBuffer.buffer[i];
        audioBuffer.source_buffer[i].buffer_size = (u64) len; // (u64) audioBuffer.size; // (u64) len / 4;
        audioBuffer.source_buffer[i].data_size = (u64) len; // (u64) audioBuffer.size; // (u64) len;
        audioBuffer.source_buffer[i].data_offset = (u64) 0;
        audoutAppendAudioOutBuffer(&audioBuffer.source_buffer[i]);
    }

    while (running) {

        if (!_paused && audioBuffer.buffered >= len) {

            audoutWaitPlayFinish(&audioBuffer.released_buffer,
                                 &audioBuffer.released_count, U64_MAX);

            mutexLock(&mutex);

            if (audioBuffer.write_pos + len < audioBuffer.read_buffer_size) {
                memcpy(audioBuffer.released_buffer->buffer,
                       audioBuffer.read_buffer + audioBuffer.write_pos, (size_t) len);
            } else {
                int tail = audioBuffer.read_buffer_size - audioBuffer.write_pos;
                memcpy(audioBuffer.released_buffer->buffer,
                       audioBuffer.read_buffer + audioBuffer.write_pos, (size_t) tail);
                memcpy((unsigned char *) audioBuffer.released_buffer->buffer + tail,
                       audioBuffer.read_buffer, (size_t) len - tail);
            }

            mutexUnlock(&mutex);

            audoutAppendAudioOutBuffer(audioBuffer.released_buffer);
            audioBuffer.write_pos = (audioBuffer.write_pos + len) % audioBuffer.read_buffer_size;
            audioBuffer.buffered -= len;

        } else {
            // should not happen as soon as audio is playing
            // printf("buffered < len\n");
            svcSleepThread(1000000 * 16); // 16 ms
        }
    }

    free(audioBuffer.buffer[0]);
    free(audioBuffer.buffer[1]);

    printf("NXAudio: audio thread ended\n");
}

NXAudio::NXAudio(int freq, int fps) : Audio(freq, fps) {

    if (!available) {
        return;
    }

    audioBuffer.size = buffer_size;
    audioBuffer.len = buffer_len;
    audioBuffer.read_buffer_size = buffer_size * 16;
    audioBuffer.read_buffer = (unsigned char *) malloc((size_t) buffer_size * 16);
    memset(audioBuffer.read_buffer, 0, (size_t) buffer_size * 16);

    audioBuffer.buffered = 0;
    audioBuffer.write_pos = 0;
    audioBuffer.read_pos = 0;

    printf("NXAudio: rate: %i, buf size: %i, buf len: %i, stream buf size: %i\n",
           frequency, audioBuffer.size, audioBuffer.len, audioBuffer.read_buffer_size);
    printf("NXAudio: device state: 0x%x\n", audoutGetDeviceState());

    // init audio (audio can't be reloaded properly ?!)
    int ret = audoutInitialize();
    printf("NXAudio: audoutInitialize: 0x%x\n", ret);

    // Start audio playback.
    ret = audoutStartAudioOut();
    printf("NXAudio: audoutStartAudioOut: 0x%x\n", ret);
    if (ret != 0) {
        audoutExit();
        available = false;
        return;
    }

    mutexInit(&mutex);

    ret = threadCreate(&thread, write_buffer, NULL, 0x5000, 0x2B, -2);
    printf("NXAudio: threadCreate: 0x%x\n", ret);
    if (ret != 0) {
        audoutStopAudioOut();
        audoutExit();
        available = false;
        return;
    }

    ret = threadStart(&thread);
    printf("NXAudio: threadStart: %i\n", ret);
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
    int ret = threadWaitForExit(&thread);
    printf("NXAudio: threadWaitForExit: %i\n", ret);

    ret = threadClose(&thread);
    printf("NXAudio: threadClose: %i\n", ret);

    //ret = audoutWaitPlayFinish(&released_buffer, U64_MAX);
    //printf("audoutWaitPlayFinish: %i\n", ret);
    ret = audoutStopAudioOut();
    printf("NXAudio: audoutStopAudioOut: %i\n", ret);
    audoutExit();

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
    _paused = pause;
}
