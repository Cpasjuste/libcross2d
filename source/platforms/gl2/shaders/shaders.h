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

// crt shaders

extern const char *c2d_crt_aperture_shader;     // scaling: integer | filter: point     |
extern const char *c2d_crt_caligari_shader;     // scaling: integer | filter: point     |
extern const char *c2d_crt_cgwg_fast_shader;    // scaling: integer | filter: point     |
extern const char *c2d_crt_easymode_shader;     // scaling: integer | filter: point     |
extern const char *c2d_crt_fakelottes_shader;   // scaling: integer | filter: linear    |
extern const char *c2d_crt_geom_shader;         // scaling: integer | filter: point     |
extern const char *c2d_crt_geom_flat_shader;    // scaling: integer | filter: point     |
extern const char *c2d_crt_hyllian_shader;      // scaling: integer | filter: point     |
extern const char *c2d_crt_lottes_fast_shader;  // scaling: integer | filter: linear    |
extern const char *c2d_crt_lottes_shader;       // scaling: integer | filter: point     |
extern const char *c2d_crt_mattias_shader;      // scaling: integer | filter: point     |
extern const char *c2d_crt_nes_mini_shader;     // scaling: integer | filter: any       |
extern const char *c2d_crt_pi_shader;           // scaling: integer | filter: linear    |
extern const char *c2d_crt_pi_flat_shader;      // scaling: integer | filter: linear    |
extern const char *c2d_crt_zfast_shader;        // scaling: integer | filter: linear    |

// handheld shaders

extern const char *c2d_handheld_bevel_shader;       // scaling: integer | filter: point     |
extern const char *c2d_handheld_dot_shader;         // scaling: integer | filter: point     |
extern const char *c2d_handheld_lcd1x_shader;       // scaling: integer | filter: point     |
extern const char *c2d_handheld_lcd3x_shader;       // scaling: integer | filter: point     |
extern const char *c2d_handheld_retro_v2_shader;    // scaling: integer | filter: point     |
extern const char *c2d_handheld_zfast_lcd_shader;   // scaling: integer | filter: linear    |

// interpolation shaders
extern const char *c2d_interpolation_aann_shader;                       // scaling: free    | filter: linear    |
extern const char *c2d_interpolation_pixellate_shader;                  // scaling: free    | filter: point     |
extern const char *c2d_interpolation_quilez_shader;                     // scaling: free    | filter: linear    |
extern const char *c2d_interpolation_sharp_bilinear_shader;             // scaling: free    | filter: linear    |
extern const char *c2d_interpolation_sharp_bilinear_scanlines_shader;   // scaling: integer | filter: linear    |

// scanlines
extern const char *c2d_scanline_simple_shader;          // scaling: free    | filter: point    |

// "sharp"
extern const char *c2d_sharp_2xsal_shader;              // scaling: free    | filter: point     |
extern const char *c2d_sharp_sabr_v30_shader;           // scaling: free    | filter: point    |
extern const char *c2d_sharp_supereagle_shader;         // scaling: free    | filter: point    |
extern const char *c2d_sharp_xbrz_freescale_shader;     // scaling: free    | filter: point    |

#endif //__C2D_SHADERS_H__
