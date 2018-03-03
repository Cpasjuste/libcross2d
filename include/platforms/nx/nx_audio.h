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
        //unsigned char *buffer;
        int len;
        int size;
        int buffered;
        int write_pos;
        int read_pos;
        void *buffer[2];
        AudioOutBuffer source_buffer[2];
        AudioOutBuffer *released_buffer;
        u32 released_count;
        Mutex mutex;
    } NXAudioBuffer;

    class NXAudio : public Audio {

    public:

        NXAudio(int freq, int fps = 60);

        ~NXAudio();

        void play();

        void pause(int pause);

        void reset();

    };
}

#endif //_NX_AUDIO_H_
