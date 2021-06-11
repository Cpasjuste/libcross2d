#!/bin/bash

# TODO
wine psp2cgc -profile sce_fp_psp2 lcd3x_f.cg -Wperf
wine psp2cgc -profile sce_vp_psp2 lcd3x_v.cg -Wperf
bin2c lcd3x_f_cg.gxp lcd3x_f.h lcd3x_f
bin2c lcd3x_v_cg.gxp lcd3x_v.h lcd3x_v
rm lcd3x_f_cg.gxp lcd3x_v_cg.gxp
