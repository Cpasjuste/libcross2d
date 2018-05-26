//
// Created by cpasjuste on 12/12/16.
//

#include <string>
#include "c2d.h"

using namespace c2d;

Audio::Audio(int freq, int fps, C2DAudioCallback cb) {

    frequency = freq;

    if (frequency <= 0) {
        return;
    }

    callback = cb;

    if (callback == nullptr) {
        buffer_len = freq / fps;
        buffer_size = buffer_len * channels * 2;
        buffer = (short *) malloc((size_t) buffer_size);
        if (buffer == nullptr) {
            printf("Audio: error, can't alloc\n");
            return;
        }
        memset(buffer, 0, (size_t) buffer_size);
    }

    available = true;
    printf("Audio: rate = %i, buf size = %i, buf len = %i\n", freq, buffer_size, buffer_len);
}

void Audio::reset() {

    if (callback == nullptr && buffer) {
        memset(buffer, 0, (size_t) buffer_size);
    }

    paused = 0;
}

void Audio::play() {
}

void Audio::pause(int pause) {
    paused = pause;
}

Audio::~Audio() {
    if (callback == nullptr && buffer) {
        free(buffer);
        buffer = NULL;
    }
}
