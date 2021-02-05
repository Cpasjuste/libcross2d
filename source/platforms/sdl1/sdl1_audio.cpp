//
// Created by cpasjuste on 12/12/16.
//

#include <malloc.h>
#include <SDL/SDL.h>
#include "cross2d/platforms/sdl1/sdl1_audio.h"

using namespace c2d;

static void audioThread(void *data, Uint8 *stream, int len) {

    auto *audio = (SDL1Audio *) data;

    SDL_LockMutex(audio->getMutex());

    if (audio->getAudioBuffer()->space_filled() >= len >> 1) {
        audio->getAudioBuffer()->pull((int16_t *) stream, len >> 1);
    }

    SDL_UnlockMutex(audio->getMutex());
}

SDL1Audio::SDL1Audio(int freq, float fps, C2DAudioCallback cb) : Audio(freq, fps, cb) {

    if (!available) {
        return;
    }

    SDL_AudioSpec wanted, obtained;

    wanted.freq = freq;
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

    printf("SDL1Audio: format %d (wanted: %d)\n", obtained.format, obtained.format);
    printf("SDL1Audio: frequency %d (wanted: %d)\n", obtained.freq, obtained.freq);
    printf("SDL1Audio: samples %d (wanted: %d)\n", obtained.samples, obtained.samples);
    printf("SDL1Audio: channels %d (wanted: %d)\n", obtained.channels, obtained.channels);

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

    SDL_DestroyMutex(mutex);
}

void SDL1Audio::play(bool sync) {
}

void SDL1Audio::play(const void *data, int samples, bool sync) {

    if (available && !paused) {

        // TODO
        //if (SDL_GetAudioDeviceStatus(deviceID) == SDL_AUDIO_PAUSED) {
        //    SDL_PauseAudioDevice(deviceID, 0);
        //}

        if (callback != nullptr) {
            return;
        }

        int size = samples * channels * (int) sizeof(int16_t);

        if (sync) {
            while (audioBuffer->space_filled() > size >> 1) {
                SDL_Delay(10);
            }
        }
        SDL_LockMutex(mutex);
        audioBuffer->push((int16_t *) data, size >> 1);
        SDL_UnlockMutex(mutex);
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

int SDL1Audio::getQueuedSize() {
    int queued;

    SDL_LockMutex(mutex);
    queued = audioBuffer->space_filled() << 1;
    SDL_UnlockMutex(mutex);

    return queued;
}
