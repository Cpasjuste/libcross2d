//
// Created by cpasjuste on 12/12/16.
//

#ifndef _SDL2_AUDIO_H_
#define _SDL2_AUDIO_H_

#include "../skeleton/audio.h"

class SDL2Audio : Audio {

public:
    SDL2Audio(int freq, int fps = 60);
    virtual ~SDL2Audio();

    virtual void Play();
    virtual void Pause(int pause);

};

#endif //_SDL2_AUDIO_H_
