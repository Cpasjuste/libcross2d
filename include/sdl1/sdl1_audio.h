//
// Created by cpasjuste on 12/12/16.
//

#ifndef _SDL1_AUDIO_H_
#define _SDL1_AUDIO_H_

#include "../skeleton/audio.h"

class SDL1Audio : Audio {

public:
    SDL1Audio(int freq, int fps = 60);

    virtual ~SDL1Audio();

    virtual void Play();

    virtual void Pause(int pause);

};

#endif //_SDL2_AUDIO_H_
