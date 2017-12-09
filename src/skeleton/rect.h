//
// Created by cpasjuste on 02/12/16.
//

#ifndef _RECT_H_
#define _RECT_H_

struct Rect {
    int x;
    int y;
    int w;
    int h;

    void Scale(int size) {
        x -= size;
        y -= size;
        w += size * 2;
        h += size * 2;
    }
};

#endif //_RECT_H_
