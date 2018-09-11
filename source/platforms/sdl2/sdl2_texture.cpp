//
// Created by cpasjuste on 01/12/16.
//

#ifndef __SDL2_GL__
#ifndef __SDL2_GLES__

#include <png.h>
#include "c2d.h"
#include "skeleton/lodepng.h"

using namespace c2d;

SDL2Texture::SDL2Texture(const char *path) : Texture(path) {

    unsigned int w, h, error = 0;
    unsigned char *pixels;

    error = lodepng_decode32_file(&pixels, &w, &h, path);
    if (error) {
        printf("SDL2Texture: couldn't create texture: %s\n", lodepng_error_text(error));
        return;
    }

    setSize(Vector2f(w, h));
    setTextureRect(IntRect(0, 0, w, h));
    pitch = (int) (getSize().x * bpp);

    SDL_Surface *tmp =
            SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, 32, pitch, SDL_PIXELFORMAT_RGBA32);
    if (!tmp) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        free(pixels);
        return;
    }

    tex = SDL_CreateTextureFromSurface(((SDL2Renderer *) c2d_renderer)->renderer, tmp);
    SDL_FreeSurface(tmp);
    free(pixels);
    if (!tex) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        return;
    }

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    available = true;

    printf("SDL2Texture(%p)\n", this);
}

SDL2Texture::SDL2Texture(const unsigned char *buffer, int bufferSize) : Texture(buffer, bufferSize) {

    unsigned int w, h, error = 0;
    unsigned char *pixels;

    error = lodepng_decode32(&pixels, &w, &h, buffer, (size_t) bufferSize);
    if (error) {
        printf("SDL2Texture: couldn't create texture: %s\n", lodepng_error_text(error));
        return;
    }

    setSize(Vector2f(w, h));
    setTextureRect(IntRect(0, 0, w, h));
    pitch = (int) (getSize().x * bpp);

    SDL_Surface *tmp =
            SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, 32, pitch, SDL_PIXELFORMAT_RGBA32);
    if (!tmp) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        free(pixels);
        return;
    }

    tex = SDL_CreateTextureFromSurface(((SDL2Renderer *) c2d_renderer)->renderer, tmp);
    SDL_FreeSurface(tmp);
    free(pixels);
    if (!tex) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        return;
    }

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    available = true;

    printf("SDL2Texture(%p)\n", this);

}

SDL2Texture::SDL2Texture(const Vector2f &size, int format) : Texture(size, format) {

    tex = SDL_CreateTexture(
            ((SDL2Renderer *) c2d_renderer)->renderer,
            format == C2D_TEXTURE_FMT_RGBA8 ? SDL_PIXELFORMAT_RGBA32 : SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            (int) size.x, (int) size.y);

    if (!tex) {
        printf("SDL2Texture: couldn't create texture: %s\n", SDL_GetError());
        return;
    }

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    available = true;

    printf("SDL2Texture(%p)\n", this);
}

int SDL2Texture::lock(FloatRect *rect, void **pix, int *p) {

    int ret = 0;

    if (rect) {
        SDL_Rect r = {
                (Sint16) rect->left,
                (Sint16) rect->top,
                (Uint16) rect->width,
                (Uint16) rect->height
        };
        ret = SDL_LockTexture(tex, &r, pix, &pitch);
    } else {
        ret = SDL_LockTexture(tex, nullptr, pix, &pitch);
    }

    if (p) {
        *p = pitch;
    }

    return ret;
}

int SDL2Texture::save(const char *path) {

    unsigned char *pixels = nullptr;
    unsigned char *converted = nullptr;
    png_bytep *rows = nullptr;

    const char *szAuthor = "Cpasjuste";
    const char *szDescription = "Screenshot";
    const char *szCopyright = "Cpasjuste";
    const char *szSoftware = "libcross2d @ libpng";
    const char *szSource = "libcross2d";

    FILE *ff = nullptr;
    png_text text_ptr[7];
    int num_text = 7;
    time_t currentTime;
    png_time_struct png_time_now;

    int w = (int) getSize().x, h = (int) getSize().y;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return -1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp) nullptr);
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(ff);
        remove(path);
        return -1;
    }

    SDL_LockTexture(tex, nullptr, (void **) &pixels, nullptr);

    // Convert the image to 32-bit
    if (bpp < 4) {
        unsigned char *pTemp = (unsigned char *) malloc(w * h * sizeof(int));
        if (bpp == 2) {
            for (int i = 0; i < h * w; i++) {
                signed short nColour = ((signed short *) pixels)[i];
                // Red
                *(pTemp + i * 4 + 0) = (unsigned char) ((nColour & 0x1F) << 3);
                *(pTemp + i * 4 + 0) |= *(pTemp + 4 * i + 0) >> 5;
                // Green
                *(pTemp + i * 4 + 1) = (unsigned char) (((nColour >> 5) & 0x3F) << 2);
                *(pTemp + i * 4 + 1) |= *(pTemp + i * 4 + 1) >> 6;
                // Blue
                *(pTemp + i * 4 + 2) = (unsigned char) (((nColour >> 11) & 0x1F) << 3);
                *(pTemp + i * 4 + 2) |= *(pTemp + i * 4 + 2) >> 5;
            }
        } else {
            memset(pTemp, 0, w * h * sizeof(int));
            for (int i = 0; i < h * w; i++) {
                *(pTemp + i * 4 + 0) = *(pixels + i * 3 + 0);
                *(pTemp + i * 4 + 1) = *(pixels + i * 3 + 1);
                *(pTemp + i * 4 + 2) = *(pixels + i * 3 + 2);
            }
        }
        converted = pTemp;
        pixels = converted;
    }

    // Get the time
    time(&currentTime);
    png_convert_from_time_t(&png_time_now, currentTime);

    ff = fopen(path, "wb");
    if (ff == nullptr) {
        printf("C2DUINXVideo::save: fopen failed: `%s`\n", path);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        if (converted) {
            free(converted);
        }
        return -1;
    }

    // Fill the PNG text fields
    text_ptr[0].key = (png_charp) "Title";
    text_ptr[0].text = (png_charp) "ROM";
    text_ptr[1].key = (png_charp) "Author";
    text_ptr[1].text = (png_charp) szAuthor;
    text_ptr[2].key = (png_charp) "Description";
    text_ptr[2].text = (png_charp) szDescription;
    text_ptr[3].key = (png_charp) "Copyright";
    text_ptr[3].text = (png_charp) szCopyright;
    text_ptr[4].key = (png_charp) "Software";
    text_ptr[4].text = (png_charp) szSoftware;
    text_ptr[5].key = (png_charp) "Source";
    text_ptr[5].text = (png_charp) szSource;
    text_ptr[6].key = (png_charp) "Comment";
    text_ptr[6].text = (png_charp) "Created by running the game in an emulator";
    for (int i = 0; i < num_text; i++) {
        text_ptr[i].compression = PNG_TEXT_COMPRESSION_NONE;
    }
    png_set_text(png_ptr, info_ptr, text_ptr, num_text);
    png_init_io(png_ptr, ff);
    png_set_IHDR(png_ptr, info_ptr, (png_uint_32) w, (png_uint_32) h, 8,
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    png_set_filler(png_ptr, 0, PNG_FILLER_AFTER);
    png_set_bgr(png_ptr);
    rows = (png_bytep *) malloc(h * sizeof(png_bytep));
    for (int y = 0; y < h; y++) {
        rows[y] = pixels + (y * w * sizeof(int));
    }
    png_write_image(png_ptr, rows);
    png_write_end(png_ptr, info_ptr);
    if (rows) {
        free(rows);
    }
    fclose(ff);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    if (converted) {
        free(converted);
    }

    return 0;
}

void SDL2Texture::unlock() {

    SDL_UnlockTexture(tex);
}

void SDL2Texture::setFiltering(int filter) {

#if 0
    if (!tex || filter == filtering) {
        return;
    }

    int access;
    SDL_QueryTexture(tex, nullptr, &access, nullptr, nullptr);
    if (access != SDL_TEXTUREACCESS_STREAMING) {
        printf("SDL2Texture::setFiltering: access != SDL_TEXTUREACCESS_STREAMING\n");
        return;
    }

    printf("SDL2Texture::setFiltering(%i)\n", filter);
    filtering = filter;

    if (filter == C2D_TEXTURE_FILTER_LINEAR) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    } else if (tex) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    }

    // SDL2 needs to re-create a texture to apply filtering...
    SDL_Texture *newTex = SDL_CreateTexture(
            ((SDL2Renderer *) c2d_renderer)->renderer,
            format == C2D_TEXTURE_FMT_RGBA8 ? SDL_PIXELFORMAT_RGBA32 : SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            (int) getSize().x, (int) getSize().y);

    void *src_pixels, *dst_pixels;
    int src_pitch, dst_pitch;

    SDL_LockTexture(tex, nullptr, &src_pixels, &src_pitch);
    SDL_LockTexture(newTex, nullptr, &dst_pixels, &dst_pitch);

    memcpy(dst_pixels, src_pixels, (size_t) pitch * (size_t) getSize().y);

    SDL_UnlockTexture(newTex);
    //SDL_UnlockTexture(tex);
    SDL_DestroyTexture(tex);
    tex = newTex;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
#endif
}

SDL2Texture::~SDL2Texture() {

    printf("~SDL2Texture(%p)\n", this);
    if (tex) {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
}

#endif // __SDL2_GLES__
#endif // __SDL2_GL__
