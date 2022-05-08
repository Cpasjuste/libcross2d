//
// Created by cpasjuste on 12/12/16.
//

#include <malloc.h>
#include "cross2d/c2d.h"

using namespace c2d;

static void audioThread(void *data, Uint8 *stream, int len) {
    auto audio = (SDL2Audio *) data;
    int samples = len >> 1;

    //printf("c2d::sdl2audio::thread: want: %i, queued: %i\n", samples, audio->getSampleBufferQueued());
    if (audio->getSampleBufferQueued() >= samples) {
        audio->lock();
        audio->getSampleBuffer()->pull((int16_t *) stream, samples);
        audio->unlock();
        //printf("c2d::sdl2audio::thread: queued: %i\n", audio->getSampleBufferQueued());
    }
}

SDL2Audio::SDL2Audio(int freq, int samples, C2DAudioCallback cb) : Audio(freq, samples, cb) {
    if (!available) {
        return;
    }

#if ANDROID
    SDL_setenv("SDL_AUDIODRIVER", "openslES", 1);
#endif

    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            printf("SDL2Audio: Initialize failed: %s.\n", SDL_GetError());
            available = false;
            return;
        }
    }

    SDL_AudioSpec wanted, obtained;
    wanted.freq = freq;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = (Uint8) channels;
    wanted.samples = samples;
    wanted.callback = cb ? cb : audioThread;
    wanted.userdata = this;

    deviceID = SDL_OpenAudioDevice(nullptr, 0, &wanted, &obtained, 0);
    if (deviceID == 0u) {
        printf("SDL2Audio: unable to open audio: %s\n", SDL_GetError());
        available = false;
        return;
    }

    printf("SDL2Audio: format %d (wanted: %d)\n", obtained.format, wanted.format);
    printf("SDL2Audio: frequency %d (wanted: %d)\n", obtained.freq, wanted.freq);
    printf("SDL2Audio: samples %d (wanted: %d)\n", obtained.samples, wanted.samples);
    printf("SDL2Audio: channels %d (wanted: %d)\n", obtained.channels, wanted.channels);

    // adjust to obtained values
    m_samples = obtained.samples;
    m_samples_size = m_samples * channels * (int) sizeof(int16_t);
    m_buffer->resize(m_samples * channels * 10);

    printf("SDL2Audio: rate = %i, samples = %i, samples size = %i (sdl samples size: %i)\n",
           m_sample_rate, m_samples, m_samples_size, obtained.size);

    SDL_PauseAudioDevice(deviceID, 1);
    available = true;
}

SDL2Audio::~SDL2Audio() {
    if (deviceID != 0u) {
        SDL_PauseAudioDevice(deviceID, 1);
        SDL_CloseAudioDevice(deviceID);
    }

    if (SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}

void SDL2Audio::pause(int pause) {
    if (available) {
        SDL_PauseAudioDevice(deviceID, pause);
    }

    Audio::pause(pause);
}

void SDL2Audio::reset() {
    if (available) {
        SDL_PauseAudioDevice(deviceID, 1);
    }

    Audio::reset();
}

SDL_AudioDeviceID SDL2Audio::getDeviceID() {
    return deviceID;
}
