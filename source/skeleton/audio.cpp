//
// Created by cpasjuste on 12/12/16.
//

#include <string>
#include <malloc.h>
#include "c2d.h"

using namespace c2d;

Audio::Audio(int freq, int fps) {

    frequency = freq;

    if (frequency <= 0) {
        return;
    }

    if (frequency == 48000) {
        buffer_len = 801;
    } else {
        buffer_len = ((freq * 100) / fps);
    }

    buffer_size = buffer_len * channels * 2;

#ifdef __NX__
    //int size = (buffer_size + 4095) & ~4095;
    //buffer = (short *) malloc((size_t) size);
    //u32 raw_data_size = (SAMPLESPERBUF * BYTESPERSAMPLE * 2);
    buffer_len = 800;
    buffer_size = buffer_len * channels * 2;
    u32 size_aligned = (u32) (buffer_size + 0xfff) & ~0xfff;
    buffer = (short *) memalign(0x1000, size_aligned);
    memset(buffer, 0, size_aligned);
#else
    buffer = (short *) malloc((size_t) buffer_size);
#endif
    if (buffer == NULL) {
        printf("Audio: error, can't alloc\n");
        return;
    }

    memset(buffer, 0, (size_t) buffer_size);
    available = true;

    printf("Audio: rate = %i, buf size = %i, buf len = %i\n", freq, buffer_size, buffer_len);
}

void Audio::Pause(int pause) {
    paused = pause;
}

Audio::~Audio() {
    if (available) {
        if (buffer != NULL) {
            free(buffer);
            buffer = NULL;
        }
    }
}
