//
// Created by cpasjuste on 12/12/16.
//

#include <malloc.h>
#include <SDL/SDL.h>
#include <audio/audio.h>
#include "ps3_audio.h"

static bool use_mutex = false;

static int buf_size;
static unsigned char *buffer_sdl;
static unsigned int buf_read_pos = 0;
static unsigned int buf_write_pos = 0;
static int buffered_bytes = 0;

static SDL_mutex *sound_mutex;
static SDL_cond *sound_cv;

static void write_buffer(unsigned char *data, int len) {

    printf("write_buffer: len: %i\n", len);

    if (use_mutex) {
        SDL_LockMutex(sound_mutex);
    } else {
        //SDL_LockAudio();
    }

    for (int i = 0; i < len; i += 4) {
        if (!use_mutex) {
            if (buffered_bytes == buf_size) {
                printf("audio drop: write_pos=%i - buffered=%i (write_len=%i)\n",
                       buf_write_pos, buffered_bytes, len);
                break; // drop samples
            }
        } else {
            while (buffered_bytes == buf_size) {
                SDL_CondWait(sound_cv, sound_mutex);
            }
        }

        *(int *) ((char *) (buffer_sdl + buf_write_pos)) = *(int *) ((char *) (data + i));
        buf_write_pos = (buf_write_pos + 4) % buf_size;
        buffered_bytes += 4;
    }

    if (use_mutex) {
        SDL_CondSignal(sound_cv);
        SDL_UnlockMutex(sound_mutex);
    } else {
        //SDL_UnlockAudio();
    }
}

static SDL_AudioCVT cvt;

static void read_buffer(void *unused, unsigned char *data, int len) {

    if (use_mutex) {
        SDL_LockMutex(sound_mutex);
    }

    //if (buffered_bytes >= len) {

        printf("read_buffer: len: %i\n", len);

        if (buf_read_pos + len <= buf_size) {
            /*
            cvt.len = len;
            memcpy(cvt.buf, buffer_sdl + buf_read_pos, len);
            int res = SDL_ConvertAudio(&cvt);
            printf("SDL_ConvertAudio1: %i\n", res);
            memcpy(data, cvt.buf, (size_t) ((double) cvt.len * cvt.len_ratio));
            */
            memcpy(data, buffer_sdl + buf_read_pos, len);
        } else {
            int tail = buf_size - buf_read_pos;
            memcpy(data, buffer_sdl + buf_read_pos, tail);
            memcpy(data + tail, buffer_sdl, len - tail);
            /*
            int tail = buf_size - buf_read_pos;
            cvt.len = tail + (len - tail);
            memcpy(cvt.buf, buffer_sdl + buf_read_pos, tail);
            memcpy(cvt.buf + tail, buffer_sdl, len - tail);
            int res = SDL_ConvertAudio(&cvt);
            printf("SDL_ConvertAudio2: %i\n", res);
            memcpy(data, cvt.buf, (size_t) ((double) cvt.len * cvt.len_ratio));
            */
        }

        buf_read_pos = (buf_read_pos + len) % buf_size;
        buffered_bytes -= len;
    //}

    if (use_mutex) {
        SDL_CondSignal(sound_cv);
        SDL_UnlockMutex(sound_mutex);
    }
}

PS3Audio::PS3Audio(int freq, int fps) : Audio(freq, fps) {

    if (!available) {
        return;
    }

    int sample_size;
    SDL_AudioSpec aspec, obtained;

    // Find the value which is slighly bigger than buffer_len*2
    //for (sample_size = 512; sample_size < (buffer_len * 2); sample_size <<= 1);
    //sample_size /= 4; // fix audio delay
    sample_size = AUDIO_BLOCK_SAMPLES;
    buf_size = sample_size * channels * sizeof(float) * 8;
    buffer_sdl = (unsigned char *) malloc((size_t) buf_size);
    memset(buffer_sdl, 0, (size_t) buf_size);

    buffered_bytes = 0;
    buf_read_pos = 0;
    buf_write_pos = 0;

    aspec.format = AUDIO_S16;
    aspec.freq = freq;
    aspec.channels = (Uint8) channels;
    aspec.samples = (Uint16) sample_size;
    aspec.callback = read_buffer;
    aspec.userdata = NULL;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE)) {
        printf("PS3Audio: Initialize failed: %s.\n", SDL_GetError());
        available = false;
        return;
    }

    if (SDL_OpenAudio(&aspec, &obtained) < 0) {
        printf("PS3Audio: Unable to open audio: %s\n", SDL_GetError());
        available = false;
        return;
    }

    if (use_mutex) {
        sound_mutex = SDL_CreateMutex();
        sound_cv = SDL_CreateCond();
        printf("PS3Audio: using mutexes for synchro\n");
    }

    printf("PS3Audio: format %d (wanted: %d, %d)\n", obtained.format, aspec.format, AUDIO_F32MSB);
    printf("PS3Audio: frequency %d (wanted: %d)\n", obtained.freq, aspec.freq);
    printf("PS3Audio: samples %d (wanted: %d)\n", obtained.samples, aspec.samples);
    printf("PS3Audio: channels %d (wanted: %d)\n", obtained.channels, aspec.channels);

    int res = SDL_BuildAudioCVT(&cvt, AUDIO_S16, 2, 48000, AUDIO_F32MSB, 2, 48000);
    printf("SDL_BuildAudioCVT: %i, needed: %i, len_mult: %i, len_ratio: %lf\n",
           res, cvt.needed, cvt.len_mult, cvt.len_ratio);
    //cvt.buf = NULL;
    cvt.buf = (Uint8 *) SDL_malloc((size_t) ((double) buf_size * cvt.len_ratio));

    SDL_PauseAudio(0);
}

PS3Audio::~PS3Audio() {

    if (!available) {
        return;
    }
    SDL_PauseAudio(1);

    if (use_mutex) {
        SDL_LockMutex(sound_mutex);
        buffered_bytes = buf_size;
        SDL_CondSignal(sound_cv);
        SDL_UnlockMutex(sound_mutex);
        SDL_Delay(100);

        SDL_DestroyCond(sound_cv);
        SDL_DestroyMutex(sound_mutex);
    }

    SDL_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    if (buffer_sdl != NULL) {
        free(buffer_sdl);
        buffer_sdl = NULL;
    }
}

void PS3Audio::Play() {

    if (!available) {
        return;
    }

    write_buffer((unsigned char *) buffer, buffer_size);

}

void PS3Audio::Pause(int pause) {

    if (!available) {
        return;
    }

    Audio::Pause(pause);
    SDL_PauseAudio(pause);

    if (use_mutex) {
        if (pause) {
            SDL_LockMutex(sound_mutex);
            buffered_bytes = 0;
            SDL_CondSignal(sound_cv);
            SDL_UnlockMutex(sound_mutex);
            SDL_Delay(100);

            SDL_DestroyCond(sound_cv);
            sound_cv = NULL;
            SDL_DestroyMutex(sound_mutex);
            sound_mutex = NULL;
        } else {
            sound_mutex = SDL_CreateMutex();
            sound_cv = SDL_CreateCond();
        }
    }
}
