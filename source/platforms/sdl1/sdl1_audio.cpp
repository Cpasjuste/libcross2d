//
// Created by cpasjuste on 12/12/16.
//

#include <malloc.h>
#include <SDL/SDL.h>
#include "cross2d/platforms/sdl1/sdl1_audio.h"

using namespace c2d;

static void audioThread(void *data, Uint8 *stream, int size) {

    auto *audio = (SDL1Audio *) data;

    //printf("c2d::sdl1audio::thread: want: %i, filled: %i\n",
    //       size >> 1, audio->getSampleBufferQueued());

    if (audio->getSampleBufferQueued() >= size >> 1) {
        audio->lock();
        audio->getSampleBuffer()->pull((int16_t *) stream, size >> 1);
        audio->unlock();
    } else {
        SDL_Delay(1);
    }
}

SDL1Audio::SDL1Audio(int rate, int samples, C2DAudioCallback cb) : Audio(rate, samples, cb) {

    if (!available) {
        return;
    }

    SDL_AudioSpec wanted, obtained;

    wanted.freq = rate;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = (Uint8) channels;
    wanted.samples = (Uint16) samples;
    wanted.callback = cb == nullptr ? audioThread : cb;
    wanted.userdata = this;

    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE)) {
            printf("SDL1Audio: Initialize failed: %s.\n", SDL_GetError());
            available = false;
            return;
        }
    }

    if (SDL_OpenAudio(&wanted, &obtained) < 0) {
        printf("SDL1Audio: Unable to open audio: %s\n", SDL_GetError());
        available = false;
        return;
    }

    printf("SDL1Audio: format %d (wanted: %d)\n", wanted.format, obtained.format);
    printf("SDL1Audio: frequency %d (wanted: %d)\n", wanted.freq, obtained.freq);
    printf("SDL1Audio: samples %d (wanted: %d)\n", wanted.samples, obtained.samples);
    printf("SDL1Audio: channels %d (wanted: %d)\n", wanted.channels, obtained.channels);

    SDL_PauseAudio(0);
}

SDL1Audio::~SDL1Audio() {

    if (!available) {
        return;
    }

    if (SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_PauseAudio(1);
        SDL_CloseAudio();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}

void SDL1Audio::play(const void *data, int samples, bool sync) {

    if (available) {
        if (paused) {
            pause(0);
        }

        if (callback != nullptr) {
            return;
        }

        int size = samples * channels * (int) sizeof(int16_t);
        if (sync) {
            while (getSampleBufferQueued() > size >> 1) {
                SDL_Delay(1);
            }
        }

        lock();
        m_buffer->push((int16_t *) data, size >> 1);
        unlock();
    }
}

void SDL1Audio::pause(int pause) {

    if (available) {
        SDL_PauseAudio(pause);
    }

    Audio::pause(pause);
}

void SDL1Audio::reset() {

    if (available) {
        SDL_PauseAudio(1);
    }

    Audio::reset();
}
