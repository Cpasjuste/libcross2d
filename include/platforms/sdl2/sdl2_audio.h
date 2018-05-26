//
// Created by cpasjuste on 12/12/16.
//

#ifndef _SDL2_AUDIO_H_
#define _SDL2_AUDIO_H_

#include "skeleton/audio.h"

namespace c2d {

    class SDL2Audio : public Audio {

    public:

        SDL2Audio(int freq, int fps = 60, C2DAudioCallback cb = NULL);

        virtual ~SDL2Audio();

        virtual void play();

        virtual void pause(int pause);

        virtual void reset();

        virtual void lock();

        virtual void unlock();
    };
}

#endif //_SDL2_AUDIO_H_
