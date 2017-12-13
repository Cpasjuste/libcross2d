//
// Created by cpasjuste on 12/12/16.
//

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <cstring>

class Audio {

public:

    Audio(int freq, int fps = 60);
    virtual ~Audio();

    virtual void Play() {};
    virtual void Pause(int pause);

    int frequency = 48000;
    int channels = 2;
    short *buffer = NULL;
    int buffer_size = 0;
    int buffer_len = 0;
    int paused = 0;
    int available = 0;
};

#endif //_AUDIO_H_
