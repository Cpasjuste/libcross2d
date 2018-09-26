//
// Created by cpasjuste on 12/12/16.
//

#ifndef _SDL2_AUDIO_H_
#define _SDL2_AUDIO_H_

#include <SDL2/SDL.h>
#include "skeleton/audio.h"

namespace c2d {

    class SDL2Audio : public Audio {

    public:

        SDL2Audio(int rate = 48000, int fps = 60, C2DAudioCallback cb = nullptr);

        virtual ~SDL2Audio();

        virtual void play();

        virtual void play(const void *data, int len);

        virtual void pause(int pause);

        virtual void reset();

    private:

        SDL_AudioDeviceID deviceID;
    };
}

#endif //_SDL2_AUDIO_H_
