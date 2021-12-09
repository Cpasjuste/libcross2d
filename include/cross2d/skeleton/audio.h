//
// Created by cpasjuste on 12/12/16.
//

#ifndef _C2D_AUDIO_H_
#define _C2D_AUDIO_H_

#include <string>
#include "cross2d/skeleton/mutex.h"
#include "audio_buffer.h"

#ifdef __WAV_LOADER__

#include "cross2d/skeleton/dr_libs/dr_wav.h"

#endif

namespace c2d {

    class Audio {

    public:
#ifdef __WAV_LOADER__

        class Wav {
        public:
            explicit Wav(const std::string &path);

            ~Wav();

            int read(void *buffer, int samples);

        private:
            drwav wav;
            bool available = false;
        };

#endif

        typedef void (*C2DAudioCallback)(void *data, unsigned char *stream, int len);

        Audio(int rate = 48000, int samples = 2048, C2DAudioCallback cb = nullptr);

        virtual ~Audio();

        virtual void play(const void *data, int samples, bool sync = false);

        virtual void pause(int pause);

        virtual void reset();

        virtual bool isPaused() {
            return paused;
        }

        bool lock();

        bool unlock();

        bool isAvailable();

        int getChannels();

        int getSampleRate();

        int getSamples();

        int getSamplesSize();

        SampleBuffer *getSampleBuffer();

        int getSampleBufferQueued();

        int getSampleBufferCapacity();

        int getSampleBufferAvailable();

        static int toSamples(int rate, float fps);

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
