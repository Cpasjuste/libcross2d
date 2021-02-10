//
// Created by cpasjuste on 12/12/16.
//

#ifndef _C2D_AUDIO_H_
#define _C2D_AUDIO_H_

#include <cstring>
#include "cross2d/skeleton/mutex.h"
#include "audio_buffer.h"

namespace c2d {

    class Audio {

    public:

        typedef void (*C2DAudioCallback)(void *data, unsigned char *stream, int len);

        Audio(int rate = 48000, int samples = 2048, C2DAudioCallback cb = nullptr);

        virtual ~Audio();

        virtual void play(const void *data, int samples, bool sync = false) {
            printf("c2d::Audio::play: not implemented\n");
        }

        virtual void pause(int pause);

        virtual void reset();

        virtual bool isPaused() {
            return paused;
        }

        bool lock();

        bool unlock();

        bool isAvailable() const;

        int getChannels() const;

        int getSampleRate() const;

        int getSamples() const;

        int getSamplesSize() const;

        SampleBuffer *getSampleBuffer();

        int getSampleBufferQueued() const;

        int getSampleBufferCapacity() const;

        int getSampleBufferAvailable() const;

    protected:

        int m_sample_rate = 48000;
        int channels = 2;
        SampleBuffer *m_buffer = nullptr;
        int m_samples = 0;
        int m_samples_size = 0;
        bool paused = false;
        bool available = false;
        C2DAudioCallback callback = nullptr;

    private:
        c2d::Mutex *mutex = nullptr;
    };
}

#endif //_C2D_AUDIO_H_
