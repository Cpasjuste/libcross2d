//
// Created by cpasjuste on 12/12/16.
//

#ifndef _NX_AUDIO_H_
#define _NX_AUDIO_H_

#include <switch.h>
#include "skeleton/audio.h"

namespace c2d {

    class NXAudio : public Audio {

    public:

        NXAudio(int freq, int fps = 60);

        virtual ~NXAudio();

        virtual void Play();

        virtual void Pause(int pause);

    private:

        AudioOutBuffer source_buffer;
        AudioOutBuffer released_buffer;

    };
}

#endif //_NX_AUDIO_H_
