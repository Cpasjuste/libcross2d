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

        Audio(int rate = 48000, float fps = 60, C2DAudioCallback cb = nullptr);

        virtual ~Audio();

        virtual void play(bool sync = false) {
            play(buffer, samples, sync);
        };

        virtual void play(const void *data, int samples, bool sync = false) {};

        virtual void pause(int pause);

        virtual void reset();

        int getSampleRate();

        int getChannels();

        short *getBuffer();

        int getBufferSize();

        int getSamples();

        bool isAvailable();

    protected:

        int sample_rate = 48000;
        int channels = 2;
        int16_t *buffer = nullptr;
        int buffer_size = 0;
        int samples = 0;
        bool paused = false;
        bool available = false;
        C2DAudioCallback callback = nullptr;
    };
}

#endif //_C2D_AUDIO_H_
