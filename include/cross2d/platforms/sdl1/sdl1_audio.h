//
// Created by cpasjuste on 12/12/16.
//

#ifndef _SDL1_AUDIO_H_
#define _SDL1_AUDIO_H_

#include "cross2d/skeleton/audio.h"

namespace c2d {

    class SDL1Audio : public Audio {

    public:
        SDL1Audio(int freq, int fps = 60);

        virtual ~SDL1Audio();

        virtual void play();

        virtual void pause(int pause);

        virtual void reset();

    };
}

#endif //_SDL2_AUDIO_H_
