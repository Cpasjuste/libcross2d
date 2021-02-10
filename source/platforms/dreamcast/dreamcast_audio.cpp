//
// Created by cpasjuste on 12/12/16.
//

#include "cross2d/c2d.h"

using namespace c2d;

static snd_stream_hnd_t stream_hnd;

static void *audioCb(snd_stream_hnd_t hnd, int smp_req, int *smp_recv) {

    auto *audio = (DCAudio *) snd_stream_get_userdata(hnd);
    audio->lock();
    audio->getAudioBuffer()->pull(audio->getBuffer(), smp_req >> 1);
    audio->unlock();
    *smp_recv = smp_req;

    return audio->getBuffer();
}

static int audioThread(void *data) {

    auto audio = (DCAudio *) data;

    snd_stream_init();
    stream_hnd = snd_stream_alloc(audioCb, audio->getBufferSize());
    snd_stream_set_userdata(stream_hnd, audio);
    snd_stream_start(stream_hnd, audio->getSampleRate(), audio->getChannels() - 1);

    while (audio->isAvailable()) {
        while (audio->isAvailable() && audio->getAudioBufferQueued() < audio->getBufferSize()) {
            //printf("audioCb: req: %i, filled: %i\n", smp_req, filled);
            thd_pass();
        }
        snd_stream_poll(stream_hnd);
    }

    return 0;
}

DCAudio::DCAudio(int freq, int samples, C2DAudioCallback cb) : Audio(freq, samples, cb) {

    if (!available) {
        return;
    }

    thread = new C2DThread(audioThread, this);
    paused = true;

    printf("DCAudio: available = %i, samples = %i, buffer size = %i, tid: %i\n",
           available, m_samples, buffer_size, thd_get_current()->tid);
}

DCAudio::~DCAudio() {

    if (!available) {
        return;
    }

    snd_stream_stop(stream_hnd);
    snd_stream_destroy(stream_hnd);
    snd_stream_shutdown();

    available = false;
    thread->join();
    delete (thread);
}

void DCAudio::play(const void *data, int samples, bool sync) {

    if (available) {
        if (paused) {
            snd_stream_volume(stream_hnd, 255);
            paused = false;
        }

        if (callback != nullptr) {
            return;
        }

        int size = samples * channels * (int) sizeof(int16_t);
        if (sync) {
            while (getAudioBufferQueued() > size >> 1) {
                thd_sleep(1);
            }
        }

        lock();
        audioBuffer->push((int16_t *) data, size >> 1);
        unlock();
    }
}

void DCAudio::pause(int pause) {
    snd_stream_volume(stream_hnd, pause ? 0 : 255);
    Audio::pause(pause);
}

void DCAudio::reset() {
    Audio::reset();
}

