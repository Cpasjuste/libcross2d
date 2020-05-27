//
// Created by cpasjuste on 12/12/16.
//

#ifndef _SDL2_AUDIO_H_
#define _SDL2_AUDIO_H_

//#include <SDL2/SDL.h>
#include "cross2d/skeleton/audio.h"

namespace c2d {

    class CTRAudio : public Audio {

    public:

        CTRAudio(int rate = 48000, float fps = 60, C2DAudioCallback cb = nullptr);

        virtual ~CTRAudio();

        virtual void play(const void *data, int samples, bool sync = false);

        virtual void pause(int pause);

        virtual void reset();

        uint32_t getQueuedSize();

        //SDL_AudioDeviceID getDeviceID();

    private:

    };
}

#endif //_SDL2_AUDIO_H_
