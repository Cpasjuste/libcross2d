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

        virtual void play(bool sync = false) {
            play(buffer, m_samples, sync);
        };

        virtual void play(const void *data, int samples, bool sync = false) {
            printf("c2d::Audio::play: not implemented\n");
        }

        virtual void pause(int pause);

        virtual void reset();

        virtual bool isPaused() {
            return paused;
        }

        bool lock() {
            return mutex->lock();
        }

        bool unlock() {
            return mutex->unlock();
        }

        [[nodiscard]] bool isAvailable() const;

        [[nodiscard]] int getSampleRate() const;

        [[nodiscard]] int getChannels() const;

        [[nodiscard]] int getSamples() const;

        [[nodiscard]] int getBufferSize() const;

        short *getBuffer();

        AudioBuffer *getAudioBuffer() {
            return audioBuffer;
        }

        virtual int getAudioBufferQueued() {
            return audioBuffer->space_filled() << 1;
        }

    protected:

        int sample_rate = 48000;
        int channels = 2;
        int16_t *buffer = nullptr;
        int buffer_size = 0;
        AudioBuffer *audioBuffer = nullptr;
        int m_samples = 0;
        bool paused = false;
        bool available = false;
        C2DAudioCallback callback = nullptr;

    private:
        c2d::Mutex *mutex = nullptr;
    };
}

#endif //_C2D_AUDIO_H_
