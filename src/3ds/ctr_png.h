//
// Created by cpasjuste on 24/11/17.
//

#ifndef _CTR_PNG_H_
#define _CTR_PNG_H_

class CTRPng {

public:
    static u8* Load(int *width, int *height, const char *filename);

    static u8* Load(int *width, int *height, const void *buffer);
};

#endif //_CTR_PNG_H_
