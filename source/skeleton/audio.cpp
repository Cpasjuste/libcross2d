//
// Created by cpasjuste on 12/12/16.
//

#include <string>
#include <cross2d/skeleton/audio.h>


#include "cross2d/c2d.h"

using namespace c2d;

Audio::Audio(int rate, int samples, C2DAudioCallback cb) {

    sample_rate = rate;
    if (sample_rate <= 0) {
        return;
    }

    mutex = new C2DMutex();

    //samples = (int) ((float) rate / fps);
    m_samples = samples;
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

    printf("Audio: rate = %i, samples = %i, buf_size = %i\n",
           rate, samples, buffer_size);
}

void Audio::reset() {
    if (buffer != nullptr) {
        memset(buffer, 0, (size_t) buffer_size);
    }
    audioBuffer->clear();
    paused = false;
}

void Audio::pause(int pause) {
    paused = pause == 1;
}

int Audio::getSampleRate() const {
    return sample_rate;
}

int Audio::getChannels() const {
    return channels;
}

short *Audio::getBuffer() {
    return buffer;
}

int Audio::getBufferSize() const {
    return buffer_size;
}

int Audio::getSamples() const {
    return m_samples;
}

bool Audio::isAvailable() const {
    return available;
}

Audio::~Audio() {
    if (buffer != nullptr) {
        free(buffer);
    }
    if (audioBuffer != nullptr) {
        delete (audioBuffer);
    }
    delete (mutex);
}
