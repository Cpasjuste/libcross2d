//
// Created by cpasjuste on 12/12/16.
//

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <cstring>

namespace c2d {

    class Audio {

    public:

        typedef void (*C2DAudioCallback)(void *userdata, Uint8 *stream, int len);

        Audio(int freq, int fps = 60, C2DAudioCallback cb = NULL);

        virtual ~Audio();

        virtual void play();

        virtual void pause(int pause);

        virtual void reset();

        virtual void lock() {};

        virtual void unlock() {};

        int frequency = 48000;
        int channels = 2;
        short *buffer = NULL;
        int buffer_size = 0;
        int buffer_len = 0;
        int paused = 0;
        int available = 0;
        C2DAudioCallback callback = NULL;
    };
}

#endif //_AUDIO_H_
