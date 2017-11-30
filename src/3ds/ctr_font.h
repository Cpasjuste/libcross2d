//
// Created by cpasjuste on 01/12/16.
//

#ifndef _CTR_FONT_H_
#define _CTR_FONT_H_

typedef struct {
    float position[3];
    float texcoord[2];
} textVertex_s;

class CTRFont : Font {

public:
    CTRFont(Renderer *renderer, const char *path, int size);

    ~CTRFont();

    void Draw(int x, int y, const char *fmt, ...);

    int GetWidth(const char *fmt, ...);

    int GetHeight(const char *fmt, ...);

private:
    void AddTextVertex(float vx, float vy, float tx, float ty);

    int textVtxArrayPos = 0;
    C3D_Tex *glyphSheets;
    textVertex_s *textVtxArray;
};

#endif //_CTR_FONT_H_
