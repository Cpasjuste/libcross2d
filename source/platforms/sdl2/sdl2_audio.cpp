//
// Created by cpasjuste on 12/12/16.
//

#include <malloc.h>
#include "cross2d/c2d.h"

using namespace c2d;

static void audioThread(void *data, Uint8 *stream, int size) {

    auto audio = (SDL2Audio *) data;

    //printf("c2d::sdl2audio::thread: want: %i, filled: %i\n",
    //       size, audio->getSampleBufferQueued());

    if (audio->getSampleBufferQueued() >= size >> 1) {
        audio->lock();
        audio->getSampleBuffer()->pull((int16_t *) stream, size >> 1);
        audio->unlock();
    }
}

SDL2Audio::SDL2Audio(int freq, int samples, C2DAudioCallback cb) : Audio(freq, samples, cb) {

    if (!available) {
        return;
    }

#ifdef __WINDOWS__
    SDL_setenv("SDL_AUDIODRIVER", "winmm", true);
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

    deviceID = SDL_OpenAudioDevice(nullptr, 0, &wanted, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (deviceID == 0u) {
        printf("SDL2Audio: unable to open audio: %s\n", SDL_GetError());
        available = false;
        return;
    }

    printf("SDL2Audio: format %d (wanted: %d)\n", wanted.format, obtained.format);
    printf("SDL2Audio: frequency %d (wanted: %d)\n", wanted.freq, obtained.freq);
    printf("SDL2Audio: samples %d (wanted: %d)\n", wanted.samples, obtained.samples);
    printf("SDL2Audio: channels %d (wanted: %d)\n", wanted.channels, obtained.channels);


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
