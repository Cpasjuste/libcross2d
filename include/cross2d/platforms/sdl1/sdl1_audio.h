//
// Created by cpasjuste on 12/12/16.
//

#ifndef _SDL1_AUDIO_H_
#define _SDL1_AUDIO_H_

#include "cross2d/skeleton/audio.h"

namespace c2d {

    class SDL1Audio : public Audio {

    public:
        SDL1Audio(int rate = 48000, float fps = 60, C2DAudioCallback cb = nullptr);

        ~SDL1Audio() override;

        void play(bool sync = false) override;

        //void play(const void *data, int len, bool sync = false) override;

        void pause(int pause) override;

        void reset() override;
    };
}

#endif //_SDL2_AUDIO_H_
