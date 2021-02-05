//
// Created by cpasjuste on 12/12/16.
//

#ifndef _C2D_AUDIO_H_
#define _C2D_AUDIO_H_

#include <cstring>
#include "audio_buffer.h"

namespace c2d {

    class Audio {

    public:

        typedef void (*C2DAudioCallback)(void *data, unsigned char *stream, int len);

        Audio(int rate = 48000, float fps = 60, C2DAudioCallback cb = nullptr);

        virtual ~Audio();

        virtual void play(bool sync = false) {
            play(buffer, samples, sync);
        };

        virtual void play(const void *data, int samples_count, bool sync = false) {};

        virtual void pause(int pause);

        virtual void reset();

        virtual int getQueuedSize() {
            return 0;
        }

        int getSampleRate();

        int getChannels();

        int getSamples();

        short *getBuffer();

        int getBufferSize();

        AudioBuffer *getAudioBuffer() {
            return audioBuffer;
        }

        bool isAvailable();

    protected:

        int sample_rate = 48000;
        int channels = 2;
        int16_t *buffer = nullptr;
        int buffer_size = 0;
        AudioBuffer *audioBuffer = nullptr;
        int samples = 0;
        bool paused = false;
        bool available = false;
        C2DAudioCallback callback = nullptr;
    };
}

#endif //_C2D_AUDIO_H_
