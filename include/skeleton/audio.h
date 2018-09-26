//
// Created by cpasjuste on 12/12/16.
//

#ifndef _C2D_AUDIO_H_
#define _C2D_AUDIO_H_

#include <cstring>

namespace c2d {

    class Audio {

    public:

        typedef void (*C2DAudioCallback)(void *data, unsigned char *stream, int len);

        Audio(int rate = 48000, int fps = 60, C2DAudioCallback cb = nullptr);

        virtual ~Audio();

        virtual void play() {};

        virtual void play(const void *data, int len) {};

        virtual void pause(int pause);

        virtual void reset();

        int getSampleRate();

        int getChannels();

        short *getBuffer();

        int getBufferSize();

        int getBufferLen();

        bool isAvailable();

    protected:

        int sample_rate = 48000;
        int channels = 2;
        short *buffer = nullptr;
        int buffer_size = 0;
        int buffer_len = 0;
        int paused = 0;
        int available = 0;
        C2DAudioCallback callback = nullptr;
    };
}

#endif //_C2D_AUDIO_H_
