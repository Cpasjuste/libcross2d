//
// Created by cpasjuste on 12/12/16.
//

#ifndef _DC_AUDIO_H_
#define _DC_AUDIO_H_

#include "cross2d/skeleton/audio.h"

namespace c2d {

    class DCAudio : public Audio {

    public:
        explicit DCAudio(int freq = 48000, float fps = 60, C2DAudioCallback cb = nullptr);

        ~DCAudio() override;

        void play(const void *data, int samples_count, bool sync = false) override;

        void pause(int pause) override;

        void reset() override;

        int getQueuedSize() override;

        c2d::Mutex *getMutex() {
            return mutex;
        }

    private:

        c2d::Mutex *mutex = nullptr;
        snd_stream_hnd_t stream_hnd;
    };
}

#endif //_DC_AUDIO_H_
