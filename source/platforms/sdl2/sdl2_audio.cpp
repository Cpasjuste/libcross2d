//
// Created by cpasjuste on 12/12/16.
//

#include <malloc.h>
#include "cross2d/platforms/sdl2/sdl2_audio.h"

using namespace c2d;

SDL2Audio::SDL2Audio(int freq, float fps, C2DAudioCallback cb) : Audio(freq, fps, cb) {

    if (!available) {
        return;
    }

    SDL_AudioSpec wanted, obtained;

    wanted.freq = freq;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = (Uint8) channels;
    wanted.samples = (Uint16) samples;
    wanted.callback = cb;
    wanted.userdata = nullptr;

#ifdef __WINDOWS__
    SDL_setenv("SDL_AUDIODRIVER", "directsound", true);
#endif

    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            printf("SDL2Audio: Initialize failed: %s.\n", SDL_GetError());
            available = false;
            return;
        }
    }

    deviceID = SDL_OpenAudioDevice(nullptr, 0, &wanted, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (deviceID == 0u) {
        printf("SDL2Audio: Unable to open audio: %s\n", SDL_GetError());
        available = false;
        return;
    }

    printf("SDL2Audio: frequency %d\n", obtained.freq);
    printf("SDL2Audio: samples %d\n", obtained.samples);
    printf("SDL2Audio: channels %d\n", obtained.channels);

    SDL_PauseAudioDevice(deviceID, 1);
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

void SDL2Audio::play(const void *data, int samples, bool sync) {

    if (available && !paused) {

        if (SDL_GetAudioDeviceStatus(deviceID) == SDL_AUDIO_PAUSED) {
            SDL_PauseAudioDevice(deviceID, 0);
        }

        if (callback != nullptr) {
            //printf("SDL2Audio::play: can't manually play, a callback was defined\n");
            return;
        }

        int size = samples * channels * (int) sizeof(int16_t);

        if (sync) {
            while (SDL_GetQueuedAudioSize(deviceID) > (Uint32) (size)) {
                SDL_Delay(1);
            }
        } else {
            // Clear the audio queue arbitrarily to avoid it backing up too far
            if (SDL_GetQueuedAudioSize(deviceID) > size * 3) {
                //printf("SDL2Audio::play: queue is full (buffer len = %i, queued = %u)\n",
                //len, SDL_GetQueuedAudioSize(deviceID));
                SDL_ClearQueuedAudio(deviceID);
            }
        }

        //printf("SDL2Audio::play: len = %i, queued = %i\n", len, SDL_GetQueuedAudioSize(deviceID));
        SDL_QueueAudio(deviceID, data, (Uint32) size);
    }
}

void SDL2Audio::reset() {

    if (available) {
        SDL_PauseAudioDevice(deviceID, 1);
    }

    Audio::reset();
}

void SDL2Audio::pause(int pause) {

    if (!available) {
        return;
    }

    Audio::pause(pause);
    SDL_PauseAudioDevice(deviceID, pause);
}

uint32_t SDL2Audio::getQueuedSize() {
    return SDL_GetQueuedAudioSize(deviceID);
}

SDL_AudioDeviceID SDL2Audio::getDeviceID() {
    return deviceID;
}
