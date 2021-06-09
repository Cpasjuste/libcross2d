//
// Created by cpasjuste on 12/12/16.
//

#ifndef _SDL2_AUDIO_H_
#define _SDL2_AUDIO_H_

#include "cross2d/skeleton/audio.h"

namespace c2d {

    class CTRAudio : public Audio {

    public:

        CTRAudio(int rate = 48000, float fps = 60, C2DAudioCallback cb = nullptr);

        ~CTRAudio() override;

        void play(const void *data, int samples, bool sync = false) override;

        void pause(int pause) override;

        void reset() override;
    };
}

#endif //_SDL2_AUDIO_H_
