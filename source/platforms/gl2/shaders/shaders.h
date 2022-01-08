//
// Created by cpasjuste on 17/09/18.
//

#ifndef __C2D_SHADERS_H__
#define __C2D_SHADERS_H__

// length of 0 means it's not a precompiled/binary shader (see gl_shaders.cpp)

extern const char *color_v;
const int color_f_length = 0;
extern const char *color_f;
const int color_v_length = 0;

extern const char *texture_v;
const int texture_v_length = 0;
extern const char *texture_f;
const int texture_f_length = 0;

//////////////////////////////

extern const char *bevel_v;
const int bevel_v_length = 0;
extern const char *bevel_f;
const int bevel_f_length = 0;

extern const char *crt_aperture_v;
const int crt_aperture_v_length = 0;
extern const char *crt_aperture_f;
const int crt_aperture_f_length = 0;

extern const char *crt_caligari_v;
const int crt_caligari_v_length = 0;
extern const char *crt_caligari_f;
const int crt_caligari_f_length = 0;

extern const char *crt_cgwg_fast_v;
const int crt_cgwg_fast_v_length = 0;
extern const char *crt_cgwg_fast_f;
const int crt_cgwg_fast_f_length = 0;

extern const char *crt_easymode_v;
const int crt_easymode_v_length = 0;
extern const char *crt_easymode_f;
const int crt_easymode_f_length = 0;

extern const char *crt_geom_v;
const int crt_geom_v_length = 0;
extern const char *crt_geom_f;
const int crt_geom_f_length = 0;

extern const char *crt_geom_flat_v;
const int crt_geom_flat_v_length = 0;
extern const char *crt_geom_flat_f;
const int crt_geom_flat_f_length = 0;

extern const char *crt_hyllian_v;
const int crt_hyllian_v_length = 0;
extern const char *crt_hyllian_f;
const int crt_hyllian_f_length = 0;

extern const char *dot_v;
const int dot_v_length = 0;
extern const char *dot_f;
const int dot_f_length = 0;

extern const char *lcd3x_v;
const int lcd3x_v_length = 0;
extern const char *lcd3x_f;
const int lcd3x_f_length = 0;

extern const char *retro_v2_v;
const int retro_v2_v_length = 0;
extern const char *retro_v2_f;
const int retro_v2_f_length = 0;

extern const char *scanlines_v;
const int scanlines_v_length = 0;
extern const char *scanlines_f;
const int scanlines_f_length = 0;

extern const char *sharp_bilinear_v;
const int sharp_bilinear_v_length = 0;
extern const char *sharp_bilinear_f;
const int sharp_bilinear_f_length = 0;

extern const char *sharp_bilinear_scanlines_v;
const int sharp_bilinear_scanlines_v_length = 0;
extern const char *sharp_bilinear_scanlines_f;
const int sharp_bilinear_scanlines_f_length = 0;

extern const char *supereagle_v;
const int supereagle_v_length = 0;
extern const char *supereagle_f;
const int supereagle_f_length = 0;

extern const char *sal2x_v;
const int sal2x_v_length = 0;
extern const char *sal2x_f;
const int sal2x_f_length = 0;

extern const char *sabr_v3_v;
const int sabr_v3_v_length = 0;
extern const char *sabr_v3_f;
const int sabr_v3_f_length = 0;

#endif //__C2D_SHADERS_H__
