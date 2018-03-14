//
// Created by cpasjuste on 12/12/16.
//

#include <malloc.h>
#include <SDL2/SDL.h>
#include "platforms/sdl2/sdl2_audio.h"

#ifdef __PSP2_DEBUG__
#include <psp2/kernel/clib.h>
#define printf sceClibPrintf
#endif

using namespace c2d;

static int buf_size;
static unsigned char *buffer_sdl;
static unsigned int buf_read_pos = 0;
static unsigned int buf_write_pos = 0;
static int buffered_bytes = 0;

static void write_buffer(const unsigned char *data, int len) {

    SDL_LockAudio();

    for (int i = 0; i < len; i += 4) {

        if (buffered_bytes >= buf_size) {
            // drop samples
            break;
        }

        memcpy(buffer_sdl + buf_write_pos, data + i, 4);
        buf_write_pos = (buf_write_pos + 4) % buf_size;
        buffered_bytes += 4;
    }

    //printf("write_buffer(%i): buffered=%i, rpos=%i, wpos=%i\n",
    //       len, buffered_bytes, buf_read_pos, buf_write_pos);

    SDL_UnlockAudio();
}

static void read_buffer(void *unused, unsigned char *data, int len) {

    //printf("read_buffer(%i): buffered=%i, rpos=%i, wpos=%i\n",
    //       len, buffered_bytes, buf_read_pos, buf_write_pos);

    if (buffered_bytes >= len) {
        if ((int) (buf_read_pos + len) <= buf_size) {
            memcpy(data, buffer_sdl + buf_read_pos, (size_t) len);
        } else {
            int tail = buf_size - buf_read_pos;
            memcpy(data, buffer_sdl + buf_read_pos, (size_t) tail);
            memcpy(data + tail, buffer_sdl, (size_t) (len - tail));
        }
        buf_read_pos = (buf_read_pos + len) % buf_size;
        buffered_bytes -= len;
    }
}

SDL2Audio::SDL2Audio(int freq, int fps) : Audio(freq, fps) {

    if (!available) {
        return;
    }

    int sample_size;
    SDL_AudioSpec aspec, obtained;

    // Find the value which is slighly bigger than buffer_len*2
    for (sample_size = 512; sample_size < (buffer_len * 2); sample_size <<= 1);
#ifdef __PSP2__
    sample_size /= 4; // fix audio delay
#endif
    buf_size = sample_size * channels * 2 * 8;
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
        printf("SDL2Audio: Initialize failed: %s.\n", SDL_GetError());
        available = false;
        return;
    }

    if (SDL_OpenAudio(&aspec, &obtained) < 0) {
        printf("SDL2Audio: Unable to open audio: %s\n", SDL_GetError());
        available = false;
        return;
    }

    printf("SDL2Audio: frequency %d\n", obtained.freq);
    printf("SDL2Audio: samples %d\n", obtained.samples);
    printf("SDL2Audio: channels %d\n", obtained.channels);

    SDL_PauseAudio(1);
}

SDL2Audio::~SDL2Audio() {

    if (!available) {
        return;
    }

    SDL_PauseAudio(1);
    SDL_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    if (buffer_sdl) {
        free(buffer_sdl);
    }
}

void SDL2Audio::play() {

    if (available && !paused) {
        if (SDL_GetAudioStatus() == SDL_AUDIO_PAUSED) {
            SDL_PauseAudio(0);
        }
        write_buffer((unsigned char *) buffer, buffer_size);
    }
}

void SDL2Audio::reset() {

    SDL_PauseAudio(1);

    buffered_bytes = 0;
    buf_write_pos = 0;
    buf_read_pos = 0;
    memset(buffer_sdl, 0, (size_t) buf_size);

    Audio::reset();
}

void SDL2Audio::pause(int pause) {

    if (!available) {
        return;
    }

    Audio::pause(pause);
    SDL_PauseAudio(pause);
}
