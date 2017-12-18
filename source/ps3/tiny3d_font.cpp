//
// Created by cpasjuste on 01/12/16.
//

#include "c2d.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "tiny3d/include/libfont.h"

static FT_Library ft_lib;
static FT_Face ft_face;

static void TTF_to_Bitmap(u8 chr, u8 *bitmap, short *w, short *h, short *y_correction) {

    FT_Set_Pixel_Sizes(ft_face, (FT_UInt) (*w), (FT_UInt) (*h));
    FT_GlyphSlot slot = ft_face->glyph;

    memset(bitmap, 0, (size_t) (*w) * (*h));

    if (FT_Load_Char(ft_face, (char) chr, FT_LOAD_RENDER)) {
        (*w) = 0;
        return;
    }

    int n, m, ww = 0;
    *y_correction = (short) ((*h) - 1 - slot->bitmap_top);
    for (n = 0; n < slot->bitmap.rows; n++) {
        for (m = 0; m < slot->bitmap.width; m++) {
            if (m >= (*w) || n >= (*h)) continue;
            bitmap[m] = (u8) slot->bitmap.buffer[ww + m];
        }
        bitmap += *w;
        ww += slot->bitmap.width;
    }

    *w = (short) (((slot->advance.x + 31) >> 6) + ((slot->bitmap_left < 0) ? -slot->bitmap_left : 0));
    *h = (short) (slot->bitmap.rows);
}

TINY3DFont::TINY3DFont(Renderer *r, const char *p, int s) : Font(r, p, s) {

    FT_Init_FreeType(&ft_lib);

    if (FT_New_Face(ft_lib, path, 0, &ft_face)) {
        printf("TINY3DFont: could not load font: %s\n", path);
        return;
    }

    pixels = (u32 *) tiny3d_AllocTexture((u32) (255 - 32) * (size * size) * 4); // num chars * w * h * bpp
    if (!pixels) {
        printf("TINY3DFont: could not allocated memory\n");
        return;
    }

    AddFontFromTTF((u8 *) pixels, 32, 255, size, size, TTF_to_Bitmap);

    FT_Done_FreeType(ft_lib);
}

void TINY3DFont::Draw(int x, int y, const char *fmt, ...) {

    if (!pixels) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    SetFontColor(color.ToRGBA(), 0);
    SetFontSize((int) ((float) size * scaling), (int) ((float) size * scaling));
    DrawString(x, y - (2.0f * scaling), msg);
}

int TINY3DFont::GetWidth(const char *fmt, ...) {

    if (!pixels) {
        return 0;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    return (int) ((float) GetFontWidth(msg) * scaling);
}

int TINY3DFont::GetHeight(const char *fmt, ...) {

    if (!pixels) {
        return 0;
    }

    return (int) ((float) size * scaling);
}

TINY3DFont::~TINY3DFont() {

    if (pixels) {
        tiny3d_FreeTexture(pixels);
    }
}
