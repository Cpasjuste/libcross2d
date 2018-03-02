//
// Created by cpasjuste on 12/12/16.
//

#include <cstdio>
#include <switch.h>
#include <malloc.h>

#include "c2d.h"

using namespace c2d;

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
static AudioOutBuffer source_buffer;
static AudioOutBuffer released_buffer;

static Thread thread;
static int running = 1;

static void read_buffer(unsigned char *data, int len) {

    for (int i = 0; i < len; i += 4) {

        while (audioBuffer.buffered == audioBuffer.read_buffer_size) {
            // oops, audio thread too slow, drop audio
            // printf("write_buffer: buffered = buffer size\n");
            return;
        }

        *(int *) ((char *) (audioBuffer.read_buffer + audioBuffer.read_pos)) = *(int *) ((char *) (data + i));
        audioBuffer.read_pos = (audioBuffer.read_pos + 4) % audioBuffer.read_buffer_size;
        audioBuffer.buffered += 4;
    }
}

static void write_buffer(void *arg) {

    printf("NXAudio: audio thread started\n");

    while (running) {

        if (!running) {
            break;
        }

        int len = audioBuffer.size;

        if (audioBuffer.buffered >= len) {

            source_buffer.next = 0;
            source_buffer.buffer_size = (u64) audioBuffer.len;
            source_buffer.data_size = (u64) audioBuffer.size;
            source_buffer.data_offset = (u64) 0;
            if (audioBuffer.write_pos + len < audioBuffer.read_buffer_size) {
                memcpy(audioBuffer.buffer,
                       audioBuffer.read_buffer + audioBuffer.write_pos, (size_t) len);
                source_buffer.buffer = audioBuffer.buffer;
            } else {
                // should not happen if we're all good
                int tail = audioBuffer.read_buffer_size - audioBuffer.write_pos;
                memcpy(audioBuffer.buffer,
                       audioBuffer.read_buffer + audioBuffer.write_pos, (size_t) tail);
                memcpy(audioBuffer.buffer + tail, audioBuffer.read_buffer, (size_t) len - tail);
                source_buffer.buffer = audioBuffer.buffer;
            }

            audoutPlayBuffer(&source_buffer, &released_buffer);

            audioBuffer.write_pos = (audioBuffer.write_pos + len) % audioBuffer.read_buffer_size;
            audioBuffer.buffered -= len;

        } else {
            // should not happen as soon as audio is playing
            // printf("buffered < len\n");
            svcSleepThread(1000000 * 16); // 16 ms
        }
    }

    printf("NXAudio: audio thread ended\n");
}

NXAudio::NXAudio(int freq, int fps) : Audio(freq, fps) {

    if (!available) {
        return;
    }

    audioBuffer.buffer = NULL;
    audioBuffer.read_buffer = NULL;

    u32 s = (u32) (buffer_size + 0xfff) & ~0xfff;
    audioBuffer.buffer = (unsigned char *) memalign(0x1000, s);
    memset(audioBuffer.buffer, 0, (size_t) s);
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
    printf("NXAudio: device state: 0x%x\n", audoutGetDeviceState());

    // init audio (audio can't be reloaded properly ?!)
    int ret = audoutInitialize();
    printf("NXAudio: audoutInitialize: 0x%x\n", ret);

    // Start audio playback.
    ret = audoutStartAudioOut();
    printf("NXAudio: audoutStartAudioOut: 0x%x\n", ret);
    if (ret != 0) {
        available = false;
        return;
    }

    ret = threadCreate(&thread, write_buffer, NULL, 0x1000, 0x20, -2);
    printf("NXAudio: threadCreate: 0x%x\n", ret);
    if (ret != 0) {
        audoutStopAudioOut();
        available = false;
        return;
    }

    ret = threadStart(&thread);
    printf("NXAudio: threadStart: %i\n", ret);
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

    int ret = threadWaitForExit(&thread);
    printf("NXAudio: threadWaitForExit: %i\n", ret);

    ret = threadClose(&thread);
    printf("NXAudio: threadClose: %i\n", ret);

    //ret = audoutWaitPlayFinish(&released_buffer, U64_MAX);
    //printf("audoutWaitPlayFinish: %i\n", ret);
    ret = audoutStopAudioOut();
    printf("NXAudio: audoutStopAudioOut: %i\n", ret);
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

}
