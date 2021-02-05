//
// Created by cpasjuste on 12/12/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

// https://github.com/DC-SWAT/DreamShell/blob/53c4ad5407f5d0fe2467cd81e12d1e494daa3ca4/modules/mp3/libmp3/libmp3/snddrv.c#L79
static void *audioThread(snd_stream_hnd_t hnd, int smp_req, int *smp_recv) {

    auto *audio = (DCAudio *) snd_stream_get_userdata(hnd);

    if(smp_req > audio->getBufferSize()) {
        *smp_recv = 0;
        return nullptr;
    }

    audio->getMutex()->lock();
    if (audio->getAudioBuffer()->space_filled() >= smp_req >> 1) {
        audio->getAudioBuffer()->pull((int16_t *) audio->getBuffer(), smp_req >> 1);
        *smp_recv = smp_req;
    }
    audio->getMutex()->unlock();

    printf("audioThread: req: %i, recv: %i\n", smp_req, *smp_recv);

    return audio->getBuffer();
}

DCAudio::DCAudio(int freq, float fps, C2DAudioCallback cb) : Audio(freq, fps, cb) {

    if (!available) {
        return;
    }

    mutex = new C2DMutex();
    snd_stream_init();
    stream_hnd = snd_stream_alloc(audioThread, getBufferSize());
    paused = true;

    printf("DCAudio: available = %i\n", available);
}

DCAudio::~DCAudio() {

    if (!available) {
        return;
    }

    snd_stream_stop(stream_hnd);
    snd_stream_destroy(stream_hnd);
    snd_stream_shutdown();

    delete (mutex);
}

void DCAudio::play(const void *data, int samples_count, bool sync) {

    if (available) {
        if (paused) {
            snd_stream_start(stream_hnd, sample_rate, channels - 1);
            paused = false;
        }

        if (callback != nullptr) {
            return;
        }

        int size = samples * channels * (int) sizeof(int16_t);
        /*
        if (sync) {
            while (audioBuffer->space_filled() > size >> 1) {
                SDL_Delay(10);
            }
        }
        */

        mutex->lock();
        audioBuffer->push((int16_t *) data, size >> 1);
        mutex->unlock();
        snd_stream_poll(stream_hnd);
    }
}

void DCAudio::pause(int pause) {

    if (available) {
        snd_stream_stop(stream_hnd);
    }

    Audio::pause(pause);
}

void DCAudio::reset() {

    if (available) {
        snd_stream_stop(stream_hnd);
    }

    Audio::reset();
}

int DCAudio::getQueuedSize() {
    int queued;

    mutex->lock();
    queued = audioBuffer->space_filled() << 1;
    mutex->unlock();

    return queued;
}
