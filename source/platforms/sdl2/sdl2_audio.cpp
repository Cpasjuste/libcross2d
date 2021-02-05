//
// Created by cpasjuste on 12/12/16.
//

#include <malloc.h>
#include "cross2d/platforms/sdl2/sdl2_audio.h"

using namespace c2d;

static void audioThread(void *data, Uint8 *stream, int len) {

    SDL2Audio *audio = (SDL2Audio *) data;

    SDL_LockMutex(audio->getMutex());
    if (audio->getAudioBuffer()->space_filled() >= len >> 1) {
        audio->getAudioBuffer()->pull((int16_t *) stream, len >> 1);
    }
    SDL_UnlockMutex(audio->getMutex());
}

SDL2Audio::SDL2Audio(int freq, float fps, C2DAudioCallback cb, bool sdlAudioBuffer) : Audio(freq, fps, cb) {

    if (!available) {
        return;
    }

    useSdlAudioBuffer = sdlAudioBuffer;
    mutex = SDL_CreateMutex();

    SDL_AudioSpec wanted, obtained;

    wanted.freq = freq;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = (Uint8) channels;
    wanted.samples = (Uint16) samples;
    wanted.callback = cb == nullptr ? (useSdlAudioBuffer ? nullptr : audioThread) : cb;
    wanted.userdata = this;

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

    SDL_DestroyMutex(mutex);
}

void SDL2Audio::play(const void *data, int samples, bool sync) {

    if (available && !paused) {

        if (SDL_GetAudioDeviceStatus(deviceID) == SDL_AUDIO_PAUSED) {
            SDL_PauseAudioDevice(deviceID, 0);
        }

        if (callback != nullptr) {
            return;
        }

        int size = samples * channels * (int) sizeof(int16_t);

        if (useSdlAudioBuffer) {
            if (sync) {
                while (SDL_GetQueuedAudioSize(deviceID) > (Uint32) size) {
                    SDL_Delay(10);
                }
            } else {
                if (SDL_GetQueuedAudioSize(deviceID) > (Uint32) size * 3) {
                    SDL_ClearQueuedAudio(deviceID);
                }
            }
            SDL_QueueAudio(deviceID, data, (Uint32) size);
        } else {
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

int SDL2Audio::getQueuedSize() {
    if (useSdlAudioBuffer) {
        return SDL_GetQueuedAudioSize(deviceID);
    } else {
        int queued = 0;
        SDL_LockMutex(mutex);
        queued = audioBuffer->space_filled() << 1;
        SDL_UnlockMutex(mutex);
        return queued;
    }
}

SDL_AudioDeviceID SDL2Audio::getDeviceID() {
    return deviceID;
}
