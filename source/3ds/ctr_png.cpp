//
// Created by cpasjuste on 24/11/17.
//

#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <png.h>

#include "../../include/3ds/ctr_png.h"

#define PNG_SIGSIZE (8)

static u16 pow2(int v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return (u16)(v >= 64 ? v : 64);
}

static void read_png_file_fn(png_structp png_ptr, png_bytep data, png_size_t length) {

    FILE *fp = (FILE *) png_get_io_ptr(png_ptr);
    fread(data, 1, length, fp);
}

static void read_png_buffer_fn(png_structp png_ptr, png_bytep data, png_size_t length) {

    unsigned int *address = (unsigned int *) png_get_io_ptr(png_ptr);
    memcpy(data, (void *) *address, length);
    *address += length;
}

static u8 *load_PNG_generic(int *width, int *height,
                            const void *io_ptr, png_rw_ptr read_data_fn) {

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        printf("CTRPng::Load: png_ptr == NULL\n");
        return NULL;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, (png_infopp) 0, (png_infopp) 0);
        printf("CTRPng::Load: info_ptr == NULL\n");
        return NULL;
    }

    png_bytep *row_ptrs = NULL;

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) 0);
        printf("CTRPng::Load: !setjmp\n");
        return NULL;
    }

    png_set_read_fn(png_ptr, (png_voidp) io_ptr, read_data_fn);
    png_set_sig_bytes(png_ptr, PNG_SIGSIZE);
    png_read_info(png_ptr, info_ptr);

    int bit_depth, color_type;

    png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *) width, (png_uint_32 *) height, &bit_depth,
                 &color_type, NULL, NULL, NULL);

    if ((color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8)
        || (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        || png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)
        || (bit_depth == 16)) {
        png_set_expand(png_ptr);
    }

    if (bit_depth == 16)
        png_set_scale_16(png_ptr);

    if (bit_depth == 8 && color_type == PNG_COLOR_TYPE_RGB)
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

    if (color_type == PNG_COLOR_TYPE_GRAY) {
        png_set_gray_to_rgb(png_ptr);
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
    } else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr);
    }


    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
    }

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    if (bit_depth < 8)
        png_set_packing(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    row_ptrs = (png_bytep *) malloc(sizeof(png_bytep) * (*height));
    if (!row_ptrs) {
        png_destroy_info_struct(png_ptr, &info_ptr);
        png_destroy_read_struct(&png_ptr, (png_infopp) 0, (png_infopp) 0);
        printf("CTRPng::Load: !row_ptrs\n");
        return NULL;
    }

    u8 *pixels = (u8 *) linearAlloc((size_t)(*width) * (*height) * 4);
    int stride = (*width) * 4;
    for (int i = 0; i < (*height); i++) {
        row_ptrs[i] = (png_bytep)(pixels + i * stride);
    }

    png_read_image(png_ptr, row_ptrs);
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) 0);
    free(row_ptrs);

    return pixels;
}

u8 *CTRPng::Load(int *width, int *height, const char *filename) {

    png_byte pngsig[PNG_SIGSIZE];
    FILE *fp;

    if ((fp = fopen(filename, "rb")) == NULL) {
        printf("CTRPng::Load(%s): fopen failed\n", filename);
        return NULL;
    }

    if (fread(pngsig, 1, PNG_SIGSIZE, fp) != PNG_SIGSIZE) {
        printf("CTRPng::Load: fread failed\n");
        fclose(fp);
        return NULL;
    }

    if (png_sig_cmp(pngsig, 0, PNG_SIGSIZE) != 0) {
        printf("CTRPng::Load: !png_sig_cmp\n");
        fclose(fp);
        return NULL;
    }

    u8 *pixels = load_PNG_generic(width, height, (void *) fp, read_png_file_fn);
    fclose(fp);
    return pixels;
}

u8 *CTRPng::Load(int *width, int *height, const void *buffer) {

    if (png_sig_cmp((png_byte *) buffer, 0, PNG_SIGSIZE) != 0) {
        printf("CTRPng::Load: !png_sig_cmp\n");
        return NULL;
    }

    unsigned int buffer_address = (unsigned int) buffer + PNG_SIGSIZE;

    return load_PNG_generic(width, height, (void *) &buffer_address, read_png_buffer_fn);
}
