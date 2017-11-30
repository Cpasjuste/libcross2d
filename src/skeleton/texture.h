//
// Created by cpasjuste on 01/12/16.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#define TEXTURE_FILTER_POINT 0
#define TEXTURE_FILTER_LINEAR 1

class Renderer;
class Rect;

class Texture {

public:

    // START - to implement, device specific code
    Texture(Renderer *renderer, const char *path);
    Texture(Renderer *renderer, int w, int h);
    virtual ~Texture() {};

    virtual Texture *Load(const char *file) { return NULL; };
    virtual void Draw(int x, int y, int w, int h, float rotation) {};
    virtual int Lock(const Rect &rect, void **pixels, int *pitch) { return 0; };
    virtual void Unlock() {};
    virtual void SetFiltering(int filter) {};
    // END - to implement, device specific code

    void Draw(int x, int y, int w, int h);
    void Draw(int x, int y);
    Rect Draw(const Rect &rect, bool fit = true);

    int width = 0;
    int height = 0;
    char path[512];
    Renderer *renderer;
};

#endif //_TEXTURE_H_
