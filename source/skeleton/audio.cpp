//
// Created by cpasjuste on 12/12/16.
//

#include <string>
#include <cross2d/skeleton/audio.h>


#include "cross2d/c2d.h"

using namespace c2d;

Audio::Audio(int rate, int samples, C2DAudioCallback cb) {

    mutex = new C2DMutex();

    m_sample_rate = rate;
    m_samples = samples;
    m_samples_size = m_samples * channels * (int) sizeof(int16_t);

    m_buffer = new SampleBuffer();
    m_buffer->resize(samples * 5);

    callback = cb;
    available = true;

    printf("Audio: rate = %i, samples = %i, samples size = %i\n",
           rate, samples, m_samples_size);
}

void Audio::reset() {
    m_buffer->clear();
    paused = false;
}

void Audio::pause(int pause) {
    paused = pause == 1;
}

int Audio::getSampleRate() const {
    return m_sample_rate;
}

int Audio::getChannels() const {
    return channels;
}

int Audio::getSamples() const {
    return m_samples;
}

int Audio::getSamplesSize() const {
    return m_samples_size;
}

SampleBuffer *Audio::getSampleBuffer() {
    return m_buffer;
}

int Audio::getSampleBufferQueued() const {
    return m_buffer->space_filled();
}

int Audio::getSampleBufferCapacity() const {
    return m_buffer->space();
}

int Audio::getSampleBufferAvailable() const {
    return m_buffer->space_empty();
}

bool Audio::isAvailable() const {
    return available;
}

bool Audio::lock() {
    return mutex->lock();
}

bool Audio::unlock() {
    return mutex->unlock();
}

Audio::~Audio() {
    if (m_buffer) {
        delete (m_buffer);
    }
    if (mutex) {
        delete (mutex);
    }
}

