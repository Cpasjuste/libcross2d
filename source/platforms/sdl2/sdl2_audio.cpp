//
// Created by cpasjuste on 12/12/16.
//

#include <malloc.h>
#include "platforms/sdl2/sdl2_audio.h"

using namespace c2d;

SDL2Audio::SDL2Audio(int freq, int fps, C2DAudioCallback cb) : Audio(freq, fps, cb) {

    if (!available) {
        return;
    }

    SDL_AudioSpec wanted, obtained;

    wanted.freq = freq;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = (Uint8) channels;
    wanted.samples = (Uint16) buffer_len;
    wanted.callback = nullptr;
    wanted.userdata = nullptr;

    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            printf("SDL2Audio: Initialize failed: %s.\n", SDL_GetError());
            available = false;
            return;
        }
    }

    deviceID = SDL_OpenAudioDevice(nullptr, 0, &wanted, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (!deviceID) {
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

    if (deviceID) {
        SDL_PauseAudioDevice(deviceID, 1);
        SDL_CloseAudioDevice(deviceID);
    }

    if (SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}

void SDL2Audio::play() {

    if (available && !paused) {

        if (SDL_GetAudioStatus() == SDL_AUDIO_PAUSED) {
            SDL_PauseAudioDevice(deviceID, 0);
        }

        while (SDL_GetQueuedAudioSize(deviceID) > (Uint32) buffer_size) {
            SDL_Delay(1);
        }

        SDL_QueueAudio(deviceID, (const void *) buffer, buffer_size);
        // Clear the audio queue arbitrarily to avoid it backing up too far
        if (SDL_GetQueuedAudioSize(deviceID) > (Uint32) (buffer_size * 3)) { SDL_ClearQueuedAudio(deviceID); }
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
