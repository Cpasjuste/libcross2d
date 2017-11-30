//
// Created by cpasjuste on 12/12/16.
//

#include <malloc.h>
#include <cstdio>
#include "audio.h"

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
    buffer = (short *) malloc((size_t) buffer_size);
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
