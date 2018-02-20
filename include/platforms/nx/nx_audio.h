//
// Created by cpasjuste on 12/12/16.
//

#ifndef _NX_AUDIO_H_
#define _NX_AUDIO_H_

#include "skeleton/audio.h"

namespace c2d {

    class NXAudio : Audio {

    public:

        NXAudio(int freq, int fps = 60);

        virtual ~NXAudio();

        virtual void Play();

        virtual void Pause(int pause);

    };
}

#endif //_NX_AUDIO_H_
