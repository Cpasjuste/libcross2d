//
// Created by cpasjuste on 12/12/16.
//

#ifndef _NX_AUDIO_H_
#define _NX_AUDIO_H_

#include <switch.h>
#include "skeleton/audio.h"

namespace c2d {

    typedef struct NXAudioBuffer {
        unsigned char *read_buffer;
        int read_buffer_size;
        unsigned char *buffer;
        int len;
        int size;
        int buffered;
        int write_pos;
        int read_pos;
        AudioOutBuffer source_buffer;
        AudioOutBuffer *released_buffer;
    } NXAudioBuffer;

    class NXAudio : public Audio {

    public:

        NXAudio(int freq, int fps = 60);

        virtual ~NXAudio();

        virtual void Play();

        virtual void Pause(int pause);

    };
}

#endif //_NX_AUDIO_H_
