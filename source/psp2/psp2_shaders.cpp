//
// Created by cpasjuste on 27/01/17.
//

#include "libvita2d/include/vita2d.h"
#include "psp2/psp2_shaders.h"

#include "lcd3x_v.h"
#include "lcd3x_f.h"
#include "texture_v.h"
#include "texture_f.h"
#include "advanced_aa_v.h"
#include "advanced_aa_f.h"
#include "scale2x_f.h"
#include "scale2x_v.h"
#include "sharp_bilinear_f.h"
#include "sharp_bilinear_v.h"
#include "sharp_bilinear_simple_f.h"
#include "sharp_bilinear_simple_v.h"
//#include "xbr_2x_noblend_f.h"
//#include "xbr_2x_noblend_v.h"
//#include "fxaa_v.h"
//#include "fxaa_f.h"
//#include "crt_easymode_f.h"
//#include "gtu_v.h"
//#include "gtu_f.h"
//#include "opaque_v.h"
//#include "bicubic_f.h"
//#include "xbr_2x_v.h"
//#include "xbr_2x_f.h"
//#include "xbr_2x_fast_v.h"
//#include "xbr_2x_fast_f.h"

using namespace c2d;

PSP2Shaders::PSP2Shaders(const std::string &shadersPath) : Shaders(shadersPath) {

    // parent class add a "NONE" shader, we set it to a simple texture shader
    get(0)->data = vita2d_create_shader((SceGxmProgram *) texture_v, (SceGxmProgram *) texture_f);

    add("lcd3x",
        vita2d_create_shader((SceGxmProgram *) lcd3x_v, (SceGxmProgram *) lcd3x_f));
    add("sharp",
        vita2d_create_shader((SceGxmProgram *) sharp_bilinear_simple_v, (SceGxmProgram *) sharp_bilinear_simple_f));
    add("sharp+scan",
        vita2d_create_shader((SceGxmProgram *) sharp_bilinear_v, (SceGxmProgram *) sharp_bilinear_f));
    add("aaa",
        vita2d_create_shader((SceGxmProgram *) advanced_aa_v, (SceGxmProgram *) advanced_aa_f));
    add("scale2x",
        vita2d_create_shader((SceGxmProgram *) scale2x_v, (SceGxmProgram *) scale2x_f));

    printf("PSP2Shader: found %i shaders\n", getCount() - 1);
}

PSP2Shaders::~PSP2Shaders() {

    for (int i = 0; i < getCount(); i++) {
        if (get(i)->data != NULL) {
            vita2d_free_shader((vita2d_shader *) get(i)->data);
        }
    }
}
