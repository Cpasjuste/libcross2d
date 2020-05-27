//
// Created by cpasjuste on 12/12/16.
//

#ifndef _SDL2_AUDIO_H_
#define _SDL2_AUDIO_H_

#include <SDL2/SDL.h>
#include "cross2d/skeleton/audio.h"

namespace c2d {

    class SDL2Audio : public Audio {

    public:

        SDL2Audio(int rate = 48000, float fps = 60, C2DAudioCallback cb = nullptr, bool useSdlAudioBuffer = false);

        ~SDL2Audio() override;

        void play(const void *data, int samples, bool sync = false) override;

        void pause(int pause) override;

        void reset() override;

        int getQueuedSize() override;

        SDL_AudioDeviceID getDeviceID();

        SDL_mutex *getMutex() {
            return mutex;
        }

    private:

        SDL_AudioDeviceID deviceID;
        SDL_mutex *mutex = nullptr;
        bool useSdlAudioBuffer = false;
    };
}

#endif //_SDL2_AUDIO_H_
