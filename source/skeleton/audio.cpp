//
// Created by cpasjuste on 12/12/16.
//

#include <string>
#include <skeleton/audio.h>

#include "c2d.h"

using namespace c2d;

Audio::Audio(int rate, int fps, C2DAudioCallback cb) {

    sample_rate = rate;
    if (sample_rate <= 0) {
        return;
    }

    buffer_len = (int) ((float) rate / (float) fps);
    buffer_size = buffer_len * channels * 2;
    buffer = (short *) malloc((size_t) buffer_size);
    if (buffer == nullptr) {
        printf("Audio: error, can't alloc buffer (size=%i)\n", buffer_size);
        return;
    }
    memset(buffer, 0, (size_t) buffer_size);

    callback = cb;
    available = true;

    printf("Audio: rate = %i, fps = %i, buf_size = %i, buf_len = %i\n", rate, fps, buffer_size, buffer_len);
}

void Audio::reset() {

    if (buffer) {
        memset(buffer, 0, (size_t) buffer_size);
    }
    paused = 0;
}

void Audio::pause(int pause) {
    paused = pause;
}

Audio::~Audio() {
    if (buffer) {
        free(buffer);
        buffer = nullptr;
    }
}

int Audio::getSampleRate() {
    return sample_rate;
}

int Audio::getChannels() {
    return channels;
}

short *Audio::getBuffer() {
    return buffer;
}

int Audio::getBufferSize() {
    return buffer_size;
}

int Audio::getBufferLen() {
    return buffer_len;
}

bool Audio::isAvailable() {
    return available == 1;
}
