//
// Created by cpasjuste on 12/12/16.
//

#include <string>

#include "cross2d/c2d.h"

using namespace c2d;

Audio::Audio(int rate, float fps, C2DAudioCallback cb) {

    sample_rate = rate;
    if (sample_rate <= 0) {
        return;
    }

    samples = (int) ((float) rate / fps);
    buffer_size = samples * channels * (int) sizeof(int16_t);
    buffer = (int16_t *) malloc(buffer_size);
    if (buffer == nullptr) {
        printf("Audio: error, can't alloc buffer (size=%i)\n", buffer_size);
        return;
    }
    memset(buffer, 0, (size_t) buffer_size);

    audioBuffer = new AudioBuffer();
    audioBuffer->resize(buffer_size * 3);

    callback = cb;
    available = true;

    printf("Audio: rate = %i, fps = %f, buf_size = %i, buf_len = %i\n", rate, fps, buffer_size, samples);
}

void Audio::reset() {

    if (buffer != nullptr) {
        memset(buffer, 0, (size_t) buffer_size);
    }
    paused = false;
}

void Audio::pause(int pause) {
    paused = pause == 1;
}

Audio::~Audio() {
    if (buffer != nullptr) {
        free(buffer);
    }
    if (audioBuffer != nullptr) {
        delete (audioBuffer);
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

int Audio::getSamples() {
    return samples;
}

bool Audio::isAvailable() {
    return available;
}
