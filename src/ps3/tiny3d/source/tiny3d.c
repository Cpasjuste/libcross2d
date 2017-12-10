/* 
   TINY3D (c) 2010 Hermes  <www.elotrolado.net>
   This work is based in ElSemi, Phire, AerialX and may be, others PSL1GHT contributors

*/

#include "tiny3d.h"
#include "matrix.h"

#include <sysutil/sysutil.h>

#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>

#include "rsxutil.h"
#include "nv_shaders.h"

#include "vshader_text_normal.vcg.h"

#include "buffer.h"
#include "commands.c"
#include <rsx/rsx.h>
#include <rsx/commands.h>
#include <rsx/mm.h>
#include "realityVP.h"

static struct {
    int target;
    u32 w, h, p;
} render_target;

static int tiny_3d_alarm = 0;
static int use_2d = 0; // MODE 2D/3D
static int enable_yuv = 0;

#define Z_SCALE 1.0/65536.0

int Video_currentBuffer = 0;

static u8 *rsx_vertex = NULL;           // vertex memory

static int size_rsx_vertex = 1024*1024; // size of vertex memory

static int pos_rsx_vertex = 0;          // offset to vertex memory

static int polygon = -1;


static u32 off_head_vertex = 0;  // index from the first vertex from shader list
static u32 off_start_vertex = 0; // memory offset of the first vertex of the polygon

static int min_vertex = 0x7fffffff; // min vertex for polygon requested
static int mod_vertex = 0x7fffffff; // module for the next vertex for polygon requested

static int n_vertex = 0; // number of vertex counted for the current polygon

#define VERTEX_LOCK      65536

#define VERTEX_SETCAMERA  4096
#define VERTEX_SETAMBIENT 2048
#define VERTEX_SETLIGHT 1024
#define VERTEX_SETMAT    512
#define VERTEX_SETPROJ   256
#define VERTEX_SETMATRIX 128

#define VERTEX_SETPOS      1
#define VERTEX_SETFCOL     2
#define VERTEX_SETCOL      4
#define VERTEX_NORMAL      8
#define VERTEX_SETTEXT     16
#define VERTEX_SETTEXT2    32


#define VERTEX_MASK (VERTEX_SETAMBIENT | VERTEX_SETCAMERA | VERTEX_SETLIGHT | VERTEX_SETMAT | VERTEX_SETPROJ | VERTEX_SETMATRIX)
#define VERTEX_MASKMATRIX (VERTEX_SETPROJ | VERTEX_SETMATRIX)

static int select_fp = 0; //  for alternative Pixel Shader

static int flag_vertex = 0; // flags to determine the shader and the datas used

static struct
{
    float x, y, z, w;

    float r, g, b, a;

    u32 rgba;

    float u, v, u2, v2;

    float nx, ny, nz;

} vertex_data;   // temp vertex data


static int current_shader = -1; // current shader ID

static struct _data_shader {
    
    int off_project;
    int off_modelv;
    int off_position;
    int off_color;
    int off_texture;
    int off_texture2;
    int off_normal;
    
    int off_lightAmbient;
    int off_lightColor;
    int off_lightPosition;
    
    int off_cameraPosition;
    int off_emissive;
    int off_ambient;
    int off_diffuse;
    int off_specular;

    void *vp;

    void *fp;
    void *fp_alt[2];
    void *fp_yuv[2];

    int size_vertex;
    int fixed_color;

} data_shader[12];

// marcador 

static struct {

    float emissive[4];
    float ambient [4];
    float diffuse [4];
    float specular[4];

} material;

static struct {

    float pos  [4][4];
    float color[4][4];
    float ambient [4];
    float camera  [4];

} light;


static MATRIX matrix_ident =
{
    {
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
    }
};

// Projection Matrix (used in 3D)

static MATRIX project_mat;

// Model/View matrix

static MATRIX model_view;

#include "list_code.h"

#define CASE_POLYGON1(x, min, mod) case x: \
       min_vertex = min; mod_vertex = mod; break;

static gcmControlRegister *CtrlReg = NULL;


static void *fpshader_list[] = {
    &nv30_fp_color, 
    &nv30_fp_texture, 
    &nv30_fp_texture_color, 
    &nv30_fp_texture2, 
    &nv30_fp_texture2_alt,
    &nv30_fp_texture2_alt2,
    &nv30_fp_texture_color2,
    &nv30_fp_texture_color2_alt,
    &nv30_fp_texture_color2_alt2,
    &nv30_fp_yuv,
    &nv30_fp_yuv8,
    &nv30_fp_yuv_color,
    &nv30_fp_yuv_color8,
    NULL
};

static volatile int tiny3d_drawing_status = 0;
static volatile int tiny3d_menu_open = 0;


static void tiny3d_callback(u64 status,	u64 param, void * userdata)
{
    switch (status) {
        case SYSUTIL_EXIT_GAME:
                    
            sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT3);
            exit(0);
            //sysProcessExit(1);
            break;
        case SYSUTIL_DRAW_BEGIN:
            tiny3d_drawing_status = 1;
            break;
        case SYSUTIL_DRAW_END:
            tiny3d_drawing_status = 0;
            break;
        case SYSUTIL_MENU_OPEN:
            tiny3d_menu_open = 1;
            break;
        case SYSUTIL_MENU_CLOSE:
            tiny3d_menu_open = 0;
            break;
        default:
           break;
             
        }
}



/***********************************************************************************************************/
/* INIT                                                                                                    */
/***********************************************************************************************************/

int tiny3d_Init(u32 vertex_buff_size)
{
    static int inited= 0;

    int n;
    
    int use_Z32 = 1;

    if(vertex_buff_size & TINY3D_Z16) use_Z32 = 0;

    vertex_buff_size &= 0x3fffffff;

    flag_vertex = VERTEX_LOCK;

    if(inited) return TINY3D_CANNOTINIT;

    inited = 1;

    if(vertex_buff_size <= 1024*1024) vertex_buff_size = 1024*1024;
    
    size_rsx_vertex = vertex_buff_size;

    init_screen(1024*1024*2, use_Z32);

    CtrlReg = gcmGetControlRegister();
    
    n = 0;

    while(fpshader_list[n]) {

        // install fragments shaders in rsx memory
        u32 *frag_mem = rsxMemalign(256, (((realityFragmentProgram_internal *) fpshader_list[n])->size * 4 + 255) & ~255);
    
        if(!frag_mem) return TINY3D_OUTMEMORY;

        internal_reality_InstallFragmentProgram(context, fpshader_list[n], frag_mem);
        
        n++;
    }

    // shaders datas
    for(n = 0; n < 12; n++) {
        data_shader[n].off_project  = internal_reality_VertexProgramGetConstant((internal_reality_VertexProgram*)       vshader_text_normal_bin, "ProjMatrix");
        data_shader[n].off_modelv   = internal_reality_VertexProgramGetConstant((internal_reality_VertexProgram*)       vshader_text_normal_bin, "WorldMatrix");
        data_shader[n].off_position = internal_reality_VertexProgramGetInputAttribute((internal_reality_VertexProgram*) vshader_text_normal_bin, "inputvertex.vertex");
        data_shader[n].off_texture  = internal_reality_VertexProgramGetInputAttribute((internal_reality_VertexProgram*) vshader_text_normal_bin, "inputvertex.texcoord");
        data_shader[n].off_texture2 = internal_reality_VertexProgramGetInputAttribute((internal_reality_VertexProgram*) vshader_text_normal_bin, "inputvertex.texcoord2");
        data_shader[n].off_color    = internal_reality_VertexProgramGetInputAttribute((internal_reality_VertexProgram*) vshader_text_normal_bin, "inputvertex.color");
        data_shader[n].off_normal   = internal_reality_VertexProgramGetInputAttribute((internal_reality_VertexProgram*) vshader_text_normal_bin, "inputvertex.normal");
        data_shader[n].vp           = (void *) vshader_text_normal_bin;
        data_shader[n].fp_alt[0]    = (void *) NULL;
        data_shader[n].fp_alt[1]    = (void *) NULL;
        data_shader[n].fp_yuv[0]    = (void *) NULL;
        data_shader[n].fp_yuv[1]    = (void *) NULL;

        data_shader[n].off_lightAmbient   = internal_reality_VertexProgramGetConstant((internal_reality_VertexProgram*) vshader_text_normal_bin, "lightAmbient");
        data_shader[n].off_lightColor     = internal_reality_VertexProgramGetConstant((internal_reality_VertexProgram*) vshader_text_normal_bin, "lightColor");
        data_shader[n].off_lightPosition  = internal_reality_VertexProgramGetConstant((internal_reality_VertexProgram*) vshader_text_normal_bin, "lightPosition");
        data_shader[n].off_cameraPosition = internal_reality_VertexProgramGetConstant((internal_reality_VertexProgram*) vshader_text_normal_bin, "CameraPosition");
        data_shader[n].off_emissive       = internal_reality_VertexProgramGetConstant((internal_reality_VertexProgram*) vshader_text_normal_bin, "Memissive");
        data_shader[n].off_ambient        = internal_reality_VertexProgramGetConstant((internal_reality_VertexProgram*) vshader_text_normal_bin, "Mambient");
        data_shader[n].off_diffuse        = internal_reality_VertexProgramGetConstant((internal_reality_VertexProgram*) vshader_text_normal_bin, "Mdiffuse");
        data_shader[n].off_specular       = internal_reality_VertexProgramGetConstant((internal_reality_VertexProgram*) vshader_text_normal_bin, "Mspecular");

        data_shader[n].fixed_color = 0;
    }
    
    data_shader[0].off_texture  = -1; // colorf
    data_shader[0].off_texture2 = -1;
    data_shader[0].off_normal   = -1;
    data_shader[0].fp           = &nv30_fp_color;
    data_shader[0].size_vertex  = 16+16;

    data_shader[1].fixed_color  = 1; // colori
    data_shader[1].off_texture  = -1;
    data_shader[1].off_texture2 = -1;
    data_shader[1].off_normal   = -1;
    data_shader[1].fp           = &nv30_fp_color;
    data_shader[1].size_vertex  = 16+4;

    data_shader[2].off_texture2 = -1; // texture
    data_shader[2].off_color    = -1;
    data_shader[2].off_normal   = -1;
    data_shader[2].fp           = &nv30_fp_texture;
    data_shader[2].fp_yuv[0]    = &nv30_fp_yuv;
    data_shader[2].fp_yuv[1]    = &nv30_fp_yuv8;
    data_shader[2].size_vertex  = 16+8;

    data_shader[3].off_color    = -1; // texture + texture2
    data_shader[3].off_normal   = -1;
    data_shader[3].fp           = &nv30_fp_texture2;
    data_shader[3].fp_alt[0]    = &nv30_fp_texture2_alt;
    data_shader[3].fp_alt[1]    = &nv30_fp_texture2_alt2;
    data_shader[3].size_vertex  = 16+8+8;

    data_shader[4].off_texture2 = -1; // texture + colorf
    data_shader[4].off_normal   = -1;
    data_shader[4].fp           = &nv30_fp_texture_color;
    data_shader[4].fp_yuv[0]    = &nv30_fp_yuv_color;
    data_shader[4].fp_yuv[1]    = &nv30_fp_yuv_color8;
    data_shader[4].size_vertex  = 16+16+8;

    data_shader[5].off_normal   = -1; // texture + texture2 + colorf
    data_shader[5].fp           = &nv30_fp_texture_color2;
    data_shader[5].fp_alt[0]    = &nv30_fp_texture_color2_alt;
    data_shader[5].fp_alt[1]    = &nv30_fp_texture_color2_alt2;
    data_shader[5].size_vertex  = 16+16+8+8;
    
    data_shader[6].fixed_color  = 1; // texture + colori
    data_shader[6].off_texture2 = -1;
    data_shader[6].off_normal   = -1;
    data_shader[6].fp           = &nv30_fp_texture_color;
    data_shader[6].fp_yuv[0]    = &nv30_fp_yuv_color;
    data_shader[6].fp_yuv[1]    = &nv30_fp_yuv_color8;
    data_shader[6].size_vertex  = 16+4+8;
    
    data_shader[7].fixed_color  = 1; // texture + texture2 + colori
    data_shader[7].off_normal   = -1;
    data_shader[7].fp           = &nv30_fp_texture_color2;
    data_shader[7].fp_alt[0]    = &nv30_fp_texture_color2_alt;
    data_shader[7].fp_alt[1]    = &nv30_fp_texture_color2_alt2;
    data_shader[7].size_vertex  = 16+4+8+8;

    data_shader[8].off_texture  = -1; // normal
    data_shader[8].off_texture2 = -1;
    data_shader[8].off_color    = -1;
    data_shader[8].fp           = &nv30_fp_color;
    data_shader[8].size_vertex  = 16+12;

    data_shader[9].off_texture2 = -1; // normal + texture
    data_shader[9].off_color    = -1;
    data_shader[9].fp           = &nv30_fp_texture_color;
    data_shader[9].size_vertex  = 16+12+8;

    data_shader[10].off_color    = -1; // normal + texture + texture2
    data_shader[10].fp           = &nv30_fp_texture_color2;
    data_shader[10].fp_alt[0]    = &nv30_fp_texture_color2_alt;
    data_shader[10].fp_alt[1]    = &nv30_fp_texture_color2_alt2;
    data_shader[10].size_vertex  = 16+12+8+8;

    current_shader = -1;

    rsx_vertex = rsxMemalign(64, vertex_buff_size);

    pos_rsx_vertex = 0;

    polygon = -1;
    off_head_vertex = off_start_vertex = 0;

    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT3);
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT3, tiny3d_callback, NULL);
    
    return TINY3D_OK;
}

/***********************************************************************************************************/
/* CHANGE SHADER CONTEXT / PUT_VERTEX                                                                      */
/***********************************************************************************************************/

static void internal_reality_SetVertexProgramConstant3f(gcmContextData *context, int constant, float values[3])
{
    float _values[4];
    _values[0] = values[0]; _values[1] = values[1]; _values[2] = values[2]; _values[3] = 0.0f; 
    internal_reality_SetVertexProgramConstant4f(context, constant, _values);
}

/*
static void internal_reality_SetVertexProgramConstant1f(gcmContextData *context, int constant, float values)
{
    float _values[4];
    _values[0] = values; _values[1] = _values[2] = _values[3] = 0.0f; 
    internal_reality_SetVertexProgramConstant4f(context, constant, _values);
}
*/



static void internal_reality_LoadVertexProgram_good(gcmContextData *context, internal_reality_VertexProgram *prog) {
	int inst;
	unsigned int *ucode = (unsigned int*)internal_reality_VertexProgramGetUCode(prog);
	internal_reality_VertexProgramConstant *constants;

    INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_VP_UPLOAD_FROM_ID, 0);
	
    for(inst = 0; inst < prog->NumInsts*4; inst += 4)
    {
    
        INTERNAL_COMMAND_LENGTH(context, 5);
		internal_commandBufferPutCmd4(context, NV30_3D_VP_UPLOAD_INST(inst & 31), 
					ucode[inst + 0],
					ucode[inst + 1],
					ucode[inst + 2],
					ucode[inst + 3]);
	}
    
    INTERNAL_COMMAND_LENGTH(context, 2+3);
    internal_commandBufferPutCmd1(context, NV30_3D_VP_START_FROM_ID, 0);
    internal_commandBufferPutCmd2(context, NV40_3D_VP_ATTRIB_EN, prog->InputMask, prog->OutputMask);
	constants = internal_reality_VertexProgramGetConstants(prog);

	if(constants)
	{
		int c;

		for(c = 0; c < prog->NumConstants; c++)
		{
            if(constants[c].Internal)
			    internal_reality_SetVertexProgramConstant4f(context,constants[c].Index,(float*)constants[c].Values);
		}
	}
    
}


static void Update_With_Normal()
{
    //if(current_shader < 5 || current_shader > 6) return;

    if(data_shader[current_shader].off_normal < 0) return;

    if(flag_vertex & VERTEX_SETMAT) {
        internal_reality_SetVertexProgramConstant4fBlock(context, data_shader[current_shader].off_emissive, 4, (float *) &material.emissive[0]);
        flag_vertex &= ~VERTEX_SETMAT;
    }

    if(flag_vertex & VERTEX_SETAMBIENT) {
        light.ambient[3] = 1.0f;
        internal_reality_SetVertexProgramConstant4f(context, data_shader[current_shader].off_lightAmbient, (float *) light.ambient);
        flag_vertex &= ~VERTEX_SETAMBIENT;
    }

    if(flag_vertex & VERTEX_SETLIGHT) {
        int i;
        for(i = 0; i < 4; i++) {
           
            internal_reality_SetVertexProgramConstant4f(context, data_shader[current_shader].off_lightPosition + i, (float *) &light.pos[i][0]);
            if(light.pos[i][3] == 0) continue;
            internal_reality_SetVertexProgramConstant3f(context, data_shader[current_shader].off_lightColor + i, (float *) &light.color[i][0]);
            
        }

        flag_vertex &= ~VERTEX_SETLIGHT;
    }

    if(flag_vertex & VERTEX_SETCAMERA) {
        internal_reality_SetVertexProgramConstant3f(context, data_shader[current_shader].off_cameraPosition, (float *) &light.camera[0]);
        flag_vertex &= ~VERTEX_SETCAMERA;
    }
}

static void set_shader_context(int old_shader)
{
    u32 offset;
    u32 text_off = 16;
   
    if(old_shader == -1)
        internal_reality_LoadVertexProgram_good(context, (internal_reality_VertexProgram*) data_shader[current_shader].vp);


    //Pass the matrix to the shader
    
    if(use_2d)
        {
        internal_reality_SetVertexProgramConstant4fBlock(context, data_shader[current_shader].off_modelv,  4, (float*)(model_view.data));
        internal_reality_SetVertexProgramConstant4fBlock(context, data_shader[current_shader].off_project, 4, (float*)(matrix_ident.data));
        }
     else
        { 
        internal_reality_SetVertexProgramConstant4fBlock(context, data_shader[current_shader].off_modelv,  4, (float*)(model_view.data));
        internal_reality_SetVertexProgramConstant4fBlock(context, data_shader[current_shader].off_project,  4, (float*)(project_mat.data));
        }
    
    if(data_shader[current_shader].off_normal >= 0) {
        
        flag_vertex |= VERTEX_MASK; Update_With_Normal();
    
    } else {
        
        light.ambient[3] = 0.0f;
        internal_reality_SetVertexProgramConstant4f(context, data_shader[current_shader].off_lightAmbient, (float *) light.ambient);
    }

    flag_vertex &= ~VERTEX_MASKMATRIX;

    off_head_vertex = 0;
    //Bind the memory array to the input attributes
    //rsx requires the offset in his memory area
    rsxAddressToOffset(&rsx_vertex[off_start_vertex],&offset);

    //stride is the distance (in bytes) from the attribute in a vertex to the same attribute in the next vertex (that is, the size of a single vertex struct)
    //elements is the number of components of this attribute that will be passed to this input parameter in the vertex program (max 4)
    internal_reality_BindVertexBufferAttribute(context, data_shader[current_shader].off_position, offset, data_shader[current_shader].size_vertex, 4, 
        REALITY_BUFFER_DATATYPE_FLOAT, REALITY_RSX_MEMORY);

    if(data_shader[current_shader].off_normal >= 0) {
        text_off += 12;
        rsxAddressToOffset(&rsx_vertex[off_start_vertex + 16],&offset);
            internal_reality_BindVertexBufferAttribute(context, data_shader[current_shader].off_normal, offset, data_shader[current_shader].size_vertex, 3,
                REALITY_BUFFER_DATATYPE_FLOAT, REALITY_RSX_MEMORY);
    } else
    if(data_shader[current_shader].off_color >= 0) {
        //now the color
        rsxAddressToOffset(&rsx_vertex[off_start_vertex + 16],&offset);
        if(data_shader[current_shader].fixed_color) {
            text_off += 4;
            internal_reality_BindVertexBufferAttribute(context, data_shader[current_shader].off_color, offset, data_shader[current_shader].size_vertex, 4,
                REALITY_BUFFER_DATATYPE_BYTE, REALITY_RSX_MEMORY);
        } else {
            text_off += 16;
            internal_reality_BindVertexBufferAttribute(context, data_shader[current_shader].off_color, offset, data_shader[current_shader].size_vertex, 4,
                REALITY_BUFFER_DATATYPE_FLOAT, REALITY_RSX_MEMORY);
        }
    }

    if(data_shader[current_shader].off_texture >= 0) {
        //now the texture coords
        rsxAddressToOffset(&rsx_vertex[off_start_vertex + text_off], &offset);
        internal_reality_BindVertexBufferAttribute(context, data_shader[current_shader].off_texture, offset, data_shader[current_shader].size_vertex, 2,
            REALITY_BUFFER_DATATYPE_FLOAT, REALITY_RSX_MEMORY);
      
        if(data_shader[current_shader].off_texture2 >= 0) {
            //now the texture coords2
            rsxAddressToOffset(&rsx_vertex[off_start_vertex + text_off + 8], &offset);
            internal_reality_BindVertexBufferAttribute(context, data_shader[current_shader].off_texture2, offset, data_shader[current_shader].size_vertex, 2,
                REALITY_BUFFER_DATATYPE_FLOAT, REALITY_RSX_MEMORY);
        }
    }
    
    if(data_shader[current_shader].fp_yuv[0] && enable_yuv)
        internal_reality_LoadFragmentProgram(context, data_shader[current_shader].fp_yuv[enable_yuv - 1]);
    else if(data_shader[current_shader].fp_alt[0] == NULL)
        internal_reality_LoadFragmentProgram(context, data_shader[current_shader].fp);
    else {

        select_fp &= 15;

        if(!select_fp)
            internal_reality_LoadFragmentProgram(context, data_shader[current_shader].fp);
        else
            internal_reality_LoadFragmentProgram(context, data_shader[current_shader].fp_alt[select_fp-1]);
    }
}


static void put_vertex()
{
    
//    if(n_vertex == 0 && off_head_vertex == 0 && off_start_vertex == pos_rsx_vertex) pos_rsx_vertex = (pos_rsx_vertex + 63) & ~63;

    if(pos_rsx_vertex > (size_rsx_vertex - 1024)) {tiny_3d_alarm = 1;flag_vertex =(flag_vertex & ~VERTEX_SETPOS) | VERTEX_LOCK; return;}

    if(flag_vertex & VERTEX_SETPOS) {

        // POSITION
        memcpy((void *) &rsx_vertex[pos_rsx_vertex], (void *) &vertex_data.x, 16);
        pos_rsx_vertex += 16;
        
        if(flag_vertex & VERTEX_NORMAL) {

            memcpy((void *) &rsx_vertex[pos_rsx_vertex], (void *) &vertex_data.nx, 12);
            pos_rsx_vertex += 12;

        } else 
        // FLOAT COLOR
        if(flag_vertex & VERTEX_SETFCOL) {

            memcpy((void *) &rsx_vertex[pos_rsx_vertex], (void *) &vertex_data.r, 16);
            pos_rsx_vertex += 16;

        } else if(flag_vertex & VERTEX_SETCOL) {

        // UINT32 COLOR

            memcpy(&rsx_vertex[pos_rsx_vertex], (void *) &vertex_data.rgba, 4);
            pos_rsx_vertex += 4;

        } else if(!(flag_vertex & VERTEX_SETTEXT)){ // default

        // UINT32 COLOR (default auto color)
            vertex_data.rgba = 0xffffffff;
            flag_vertex |= VERTEX_SETCOL;
            memcpy(&rsx_vertex[pos_rsx_vertex], (void *) &vertex_data.rgba, 4);
            pos_rsx_vertex += 4;
            
        }
        
        // TEXTURE
        if(flag_vertex & VERTEX_SETTEXT) {
            memcpy((void *) &rsx_vertex[pos_rsx_vertex], (void *) &vertex_data.u, 8);
            pos_rsx_vertex += 8;

             if(flag_vertex & VERTEX_SETTEXT2) {
                memcpy((void *) &rsx_vertex[pos_rsx_vertex], (void *) &vertex_data.u2, 8);
                pos_rsx_vertex += 8;
            }
        }

        n_vertex++;
    }
   
   flag_vertex &= ~VERTEX_SETPOS;

}

/***********************************************************************************************************/
/* POLYGON / VERTEX                                                                                        */
/***********************************************************************************************************/

int tiny3d_SetPolygon(type_polygon type)
{
    n_vertex = 0;
    
    flag_vertex = (flag_vertex &  VERTEX_MASK) | VERTEX_LOCK;

    if(polygon > 0) return TINY3D_BUSY;

    if(type < TINY3D_POINTS || type > TINY3D_POLYGON) return TINY3D_INVALID;

    if(grab_list) {
        push_list_cmd(LIST_SETPOLYGON, 1); push_list_int(type);
        flag_vertex &= ~VERTEX_LOCK;
        return TINY3D_OK;
    }

    polygon = type;

    switch(polygon) {
        
        CASE_POLYGON1(TINY3D_POINTS,         1, 1);
        CASE_POLYGON1(TINY3D_LINES,          2, 2);
        CASE_POLYGON1(TINY3D_LINE_LOOP,      2, 1);
        CASE_POLYGON1(TINY3D_LINE_STRIP,     2, 1);
        CASE_POLYGON1(TINY3D_TRIANGLES,      3, 3);
        CASE_POLYGON1(TINY3D_TRIANGLE_STRIP, 3, 1);
        CASE_POLYGON1(TINY3D_TRIANGLE_FAN,   3, 1);
        CASE_POLYGON1(TINY3D_QUADS,          4, 4);
        CASE_POLYGON1(TINY3D_QUAD_STRIP,     4, 2);
        CASE_POLYGON1(TINY3D_POLYGON,        3, 1)
    }
    
    off_start_vertex = pos_rsx_vertex;

    flag_vertex &= ~VERTEX_LOCK;

    return TINY3D_OK;

}

void tiny3d_VertexPos(float x, float y, float z)
{
    
    if(flag_vertex & VERTEX_LOCK) return;

    if(grab_list) {
        push_position_list(x, y, z, 1.0f);
        return;
    }

    put_vertex(); // previous vertex;
    
    vertex_data.x = x; vertex_data.y = y; vertex_data.z = z; vertex_data.w = 1.0f;

    flag_vertex |= VERTEX_SETPOS;
}

void tiny3d_VertexPos4(float x, float y, float z, float w)
{
    if(flag_vertex & VERTEX_LOCK) return;

    if(grab_list) {
        push_position_list(x, y, z, w);
        return;
    }

    put_vertex(); // previous vertex;
    
    vertex_data.x = x; vertex_data.y = y; vertex_data.z = z; vertex_data.w = w;

    flag_vertex |= VERTEX_SETPOS;
}

void tiny3d_VertexPosVector(VECTOR v)
{
    
    if(flag_vertex & VERTEX_LOCK) return;

    if(grab_list) {
        push_position_list(v.x, v.y, v.z, 1.0f);
        return;
    }

    put_vertex(); // previous vertex;
    
    vertex_data.x = v.x; vertex_data.y = v.y; vertex_data.z = v.z; vertex_data.w = 1.0f;

    flag_vertex |= VERTEX_SETPOS;
}

void tiny3d_VertexColor(u32 rgba)
{
    
    vertex_data.rgba = rgba;

    if(flag_vertex & VERTEX_NORMAL) return;
    
    if(grab_list) {
        push_list_cmd(LIST_VERTEXCOLOR, 1); push_list_int(rgba);
        return;
    }

    flag_vertex |= VERTEX_SETCOL;

}

void tiny3d_VertexFcolor(float r, float g, float b, float a)
{
    
    vertex_data.r = r; vertex_data.g = g; vertex_data.b = b; vertex_data.a = a;

    if(flag_vertex & VERTEX_NORMAL) return;

     if(grab_list) {
        push_list_cmd(LIST_VERTEXFCOLOR, 4); push_list_float(r); push_list_float(g); push_list_float(b); push_list_float(a);
        return;
    }

    flag_vertex |= VERTEX_SETFCOL;
}

void tiny3d_VertexTexture(float u, float v)
{

    if(grab_list) {
        push_list_cmd(LIST_VERTEXTEXTURE, 2); push_list_float(u); push_list_float(v);
        return;
    }

    vertex_data.u = u; vertex_data.v = v;

    flag_vertex |= VERTEX_SETTEXT;

}

void tiny3d_VertexTexture2(float u, float v)
{
    
    if(!(flag_vertex & VERTEX_SETTEXT)) return;

    if(grab_list) {
        push_list_cmd(LIST_VERTEXTEXTURE2, 2); push_list_float(u); push_list_float(v);
        return;
    }

    vertex_data.u2 = u; vertex_data.v2 = v;

    flag_vertex |= VERTEX_SETTEXT2;
}

void tiny3d_Normal(float x, float y, float z)
{
    if(grab_list) {
        push_normal_list(x, y, z);
        return;
    }
    
    vertex_data.nx = x; vertex_data.ny = y; vertex_data.nz = z;

    flag_vertex |= VERTEX_NORMAL;
}

void tiny3d_NormalVector(VECTOR v)
{
    if(grab_list) {
        push_normal_list(v.x, v.y, v.z);
        return;
    }
    
    vertex_data.nx = v.x; vertex_data.ny = v.y; vertex_data.nz = v.z;

    flag_vertex |= VERTEX_NORMAL;
}


int tiny3d_End()
{
     if(grab_list) {

        push_list_cmd(LIST_POLYGONEND, 0);

        n_vertex = 0;

        polygon = -1;
        flag_vertex = (flag_vertex & VERTEX_MASK) |  VERTEX_LOCK;

        return TINY3D_OK;
    }

    put_vertex(); // set the last vertex
    
    if(polygon > 0 && n_vertex >= min_vertex) {
        
        int temp_shader = 0;

        n_vertex -= ((n_vertex - min_vertex) % mod_vertex);
        
        if(flag_vertex & VERTEX_SETTEXT) { // texture
            temp_shader = 2;
            if(flag_vertex & VERTEX_NORMAL) { // with normal and texture
                temp_shader = 9;
                flag_vertex &= ~ (VERTEX_SETFCOL | VERTEX_SETCOL);
            } else
            if(flag_vertex & VERTEX_SETFCOL) { // with color float
                temp_shader = 4; 
            } else if(flag_vertex & VERTEX_SETCOL) { // with color u32
                temp_shader = 6;
            }
           
        if(flag_vertex & VERTEX_SETTEXT2) temp_shader++;

        } else if(flag_vertex & VERTEX_NORMAL) { // with normal
                temp_shader = 8;
                flag_vertex &= ~ (VERTEX_SETFCOL | VERTEX_SETCOL);
        } else if(flag_vertex & VERTEX_SETCOL) {

            temp_shader = 1;  // with color u32
        }
   
        // force to change the sahders under YUV

        if(enable_yuv) {current_shader = -1;}

        if(temp_shader != current_shader) { // set the shader
            
            int old_shader = current_shader;
            current_shader = temp_shader;
            set_shader_context(old_shader);
        } else if(data_shader[current_shader].fp_yuv[0] && enable_yuv)
            internal_reality_LoadFragmentProgram(context, data_shader[current_shader].fp_yuv[enable_yuv - 1]);
        
        else if(data_shader[current_shader].fp_alt[0] != NULL && (select_fp & 128)!=0){    
             
            select_fp &= 15; // disable Pixel Shader Update

            if(!select_fp)
                internal_reality_LoadFragmentProgram(context, data_shader[current_shader].fp);
            else
                internal_reality_LoadFragmentProgram(context, data_shader[current_shader].fp_alt[select_fp-1]);
        }
 

        select_fp &= 15; // disable Pixel Shader Update
    
        if(flag_vertex & VERTEX_SETMATRIX) { // update matrix

            internal_reality_SetVertexProgramConstant4fBlock(context, data_shader[current_shader].off_modelv,  4, (float*)(model_view.data));
            flag_vertex &= ~VERTEX_SETMATRIX;
        }

        if(flag_vertex & VERTEX_SETPROJ) { // update matrix
        
             if(!use_2d)
                internal_reality_SetVertexProgramConstant4fBlock(context, data_shader[current_shader].off_project,  4, (float*)(project_mat.data));
             else
                internal_reality_SetVertexProgramConstant4fBlock(context, data_shader[current_shader].off_project, 4, (float*)(matrix_ident.data));

            flag_vertex &= ~VERTEX_SETPROJ;
        }

        Update_With_Normal();

        internal_reality_DrawVertexBuffer(context, polygon, off_head_vertex, n_vertex);

        off_head_vertex += n_vertex;

        n_vertex = 0;

        polygon = -1;
        flag_vertex = (flag_vertex & VERTEX_MASK) |  VERTEX_LOCK;

    }


    return TINY3D_OK;

}

/***********************************************************************************************************/
/* MATRIX                                                                                                  */
/***********************************************************************************************************/

void tiny3d_SetMatrixModelView(MATRIX *mat)
{
    if(grab_list) {

        set_apply_matrix = 0;

        push_list_cmd(LIST_MATRIX, 16);

        if(!mat) 
            memcpy((void *) &curr_list[curr_list_index], (void *) &matrix_ident, 4*4*4);
        else
            memcpy((void *) &curr_list[curr_list_index], (void *) mat, 4*4*4);

        curr_list_index += 16;

        return;
    }

    if(!mat) 
        model_view = matrix_ident;
    else
        model_view = *mat;

    flag_vertex |= VERTEX_SETMATRIX;
}

void tiny3d_SetProjectionMatrix(MATRIX *mat)
{
    if(!mat) 
        project_mat= matrix_ident;
    else
        project_mat = *mat;

    if(!use_2d)
        flag_vertex |= VERTEX_SETPROJ;
}


/***********************************************************************************************************/
/* SCREEN CLEAR / FLIP / CHANGE 2D/3D MODE                                                                 */
/***********************************************************************************************************/

static void tiny3d_WaitRSX()
{
    int i = 0;

    tiny3d_End();

    
	while (CtrlReg->get != CtrlReg->put  && (i < 5000)) {
		usleep(200);CtrlReg = gcmGetControlRegister();
        __asm __volatile__("sync");
		i++;
	}
    
  
}

void tiny3d_Flip()
{
    
    tiny3d_End();

    flip(Video_currentBuffer); // Flip buffer onto screen
    tiny3d_WaitRSX();
    Video_currentBuffer = !Video_currentBuffer;
    sysUtilCheckCallback();
    waitFlip();
    

    pos_rsx_vertex = 0;
    current_shader = -1;
    polygon = -1;
    off_head_vertex = off_start_vertex = 0;
    flag_vertex = VERTEX_LOCK;

}

static int user_viewport = 0;
static float x_viewport = 0.0f;
static float y_viewport = 0.0f;
static float scale2D_x_viewport = 1.0f;
static float scale2D_y_viewport = 1.0f;
static float scale3D_x_viewport = 1.0f;
static float scale3D_y_viewport = 1.0f;

void tiny3d_UserViewport(int onoff, float pos_x, float pos_y, float scale2D_x, float scale2D_y, float scale3D_x, float scale3D_y)
{
    user_viewport = onoff;
    x_viewport = pos_x;
    y_viewport = pos_y;
    scale2D_x_viewport = scale2D_x;
    scale2D_y_viewport = scale2D_y;
    scale3D_x_viewport = scale3D_x;
    scale3D_y_viewport = scale3D_y;
}

void tiny3d_Project2D()
{
    use_2d = 1;

    flag_vertex |= VERTEX_SETPROJ;

    if(render_target.target) {
        
        internal_reality_ViewportTranslate(context, 0.0, 0.0, 0.0, 0.0);
        internal_reality_ViewportScale(context, ((float) render_target.w)/848.0 ,  (float) (render_target.h)/512.0f, Z_SCALE, 1.0);

        return;
    }
    
    if(user_viewport) {
        
        internal_reality_ViewportTranslate(context, x_viewport, y_viewport, 0.0, 0.0);
        internal_reality_ViewportScale(context, scale2D_x_viewport,  scale2D_y_viewport, Z_SCALE, 1.0); 
        
        return;
    }

    // 480P / 576P
    if(Video_Resolution.width < 1280) {

        internal_reality_ViewportTranslate(context, 38.0 , 16.0, 0.0, 0.0);

        internal_reality_ViewportScale(context, (float) (Video_Resolution.width - 72) / 848.0, 
            (Video_Resolution.height == 480) ? (512.0) / 576.0 : 548.0 / 512.0, Z_SCALE, 1.0);
    
    } else if(Video_Resolution.width == 1280) {
    // 720P
        
        internal_reality_ViewportTranslate(context, 54.0, 24.0, 0.0, 0.0);

        internal_reality_ViewportScale(context, 848.0 / 611.0 ,  674.0 / 512.0, Z_SCALE, 1.0);
    
    } else {
    // 1080i
        
        internal_reality_ViewportTranslate(context, 63.0, 40.0, 0.0, 0.0);

        internal_reality_ViewportScale(context, 848.0 / 400.0 ,  952.0 / 512.0, Z_SCALE, 1.0);
    
    }
}

void tiny3d_Project3D()
{
    use_2d = 0;

    flag_vertex |= VERTEX_SETPROJ;

    if(render_target.target) {
        
        internal_reality_ViewportTranslate(context, ((float) render_target.w)/2.0f, (float) (render_target.h)/2.0f, 0.0, 0.0);
        internal_reality_ViewportScale(context, ((float) render_target.w)/ 1920.0f ,  (float) (render_target.h)/ 1080.0f, Z_SCALE, 1.0);

        return;
    }

    internal_reality_ViewportTranslate(context, (float) Video_Resolution.width / 2.0f, (float) Video_Resolution.height / 2.0, 0.0, 0.0);
    
    if(user_viewport) {
        
        internal_reality_ViewportScale(context, scale3D_x_viewport,  scale3D_y_viewport, Z_SCALE, 1.0); 
        
    } else {

        internal_reality_ViewportScale(context, (float) Video_Resolution.width / 1920.0f, (float) Video_Resolution.height / 1080.0f , Z_SCALE, 1.0);
    }
  
}


void tiny3d_Clear(u32 color, clear_flags flags)
{

    flag_vertex |= VERTEX_LOCK;

    render_target.target = 0;

    if(flags & TINY3D_CLEAR_COLOR) {
       
        tiny3d_Project2D();

        tiny3d_SetProjectionMatrix(&matrix_ident);
        tiny3d_SetMatrixModelView(&matrix_ident);
        
        internal_reality_ZControl(context, 0, 1, 1); // disable viewport culling
 
        internal_reality_DepthTestFunc(context, REALITY_ZFUNC_LESSOREQUAL);
        internal_reality_DepthWriteEnable(context, 1);
        internal_reality_DepthTestEnable(context, 1);

        internal_reality_AlphaEnable(context, 0);

        internal_reality_BlendEnable(context, 0);

        internal_reality_Viewport(context, Video_Resolution.width, Video_Resolution.height);
        
        
      
        int n;
       
        for(n = 0; n < 8; n++)
            {
            
            internal_reality_ViewportClip(context, n, Video_Resolution.width, Video_Resolution.height);
            }

        setupRenderTarget(Video_currentBuffer);

        rsxSetClearColor(context, color);

    }

    if(flags & TINY3D_CLEAR_ZBUFFER) {

        rsxSetClearDepthValue(context, 0xffffffff);
    }

    internal_reality_ClearBuffers(context, flags);

    if((flags & TINY3D_CLEAR_COLOR) && tiny_3d_alarm) {
        
        tiny3d_SetPolygon(TINY3D_QUADS);

        tiny3d_VertexPos(848/2  , 0  ,  65535);
        tiny3d_VertexColor(0xff0000ff);

        tiny3d_VertexPos(847, 0  , 65535);

        tiny3d_VertexPos(847, 511, 65535);

        tiny3d_VertexPos(848/2 , 511, 65535);
        tiny3d_End();

        internal_reality_Viewport(context, Video_Resolution.width*15/16, Video_Resolution.height);
        int n;
        for(n = 0; n < 8; n++)
            internal_reality_ViewportClip(context, n, Video_Resolution.width*15/16, Video_Resolution.height);
        internal_reality_ClearBuffers(context, flags);
    }
}

void tiny3d_ClearSurface(u32 color, clear_flags flags, u32 rsx_offset, u32 width, u32 height, u32 stride, u32 format)
{

    flag_vertex |= VERTEX_LOCK;

    render_target.target = 1;
    render_target.w = width;
    render_target.p = stride;
    render_target.h = height;


    if(flags & TINY3D_CLEAR_COLOR) {

        tiny3d_Project2D();

        tiny3d_SetProjectionMatrix(&matrix_ident);
        tiny3d_SetMatrixModelView(&matrix_ident);
        
        internal_reality_ZControl(context, 0, 1, 1); // disable viewport culling
 
        internal_reality_DepthTestFunc(context, REALITY_ZFUNC_LESSOREQUAL);
        internal_reality_DepthWriteEnable(context, 1);
        internal_reality_DepthTestEnable(context, 1);

        internal_reality_AlphaEnable(context, 0);

        internal_reality_BlendEnable(context, 0);

        internal_reality_Viewport(context, width, height);
        
        int n;

        for(n = 0; n < 8; n++)
            internal_reality_ViewportClip(context, n, width, height);

        setupRenderTarget2(rsx_offset, stride, width, height, format);

        rsxSetClearColor(context, color);

    }

    if(flags & TINY3D_CLEAR_ZBUFFER) {

        rsxSetClearDepthValue(context, 0xffffffff);
    }

    internal_reality_ClearBuffers(context, flags);
}

/***********************************************************************************************************/
/* ALPHA / BLEND func                                                                                      */
/***********************************************************************************************************/

void tiny3d_AlphaTest (int enable, u8 ref, alpha_func func)
{
    internal_reality_AlphaFunc(context, func);

    internal_reality_AlphaRef(context, (u32) ref);

    internal_reality_AlphaEnable(context, enable!=0);
}

void tiny3d_BlendFunc (int enable, blend_src_func src_fun, blend_dst_func dst_func, blend_func func)
{
    internal_reality_BlendFunc(context, src_fun, dst_func);
    internal_reality_BlendEquation(context, func);
    internal_reality_BlendEnable(context, enable!=0);
}

/***********************************************************************************************************/
/* TEXTURES                                                                                                */
/***********************************************************************************************************/

void tiny3d_SetTexture(u32 unit, u32 offset, u32 width, u32 height, u32 stride, text_format fmt, int smooth)
{

    if(grab_list) {

        push_list_cmd(LIST_SETTEXTURE, 9); push_list_int(unit); push_list_int(offset); push_list_int(width); push_list_int(height);
        push_list_int(stride);push_list_int(fmt); push_list_int(TEXTWRAP_REPEAT); push_list_int(TEXTWRAP_REPEAT); push_list_int(smooth);

        return;
    }
    
    tiny3d_SetTextureWrap(unit, offset, width, height, stride, fmt, TEXTWRAP_REPEAT, TEXTWRAP_REPEAT, smooth);
    
}


void tiny3d_SetTextureWrap(u32 unit, u32 offset, u32 width, u32 height, u32 stride, text_format fmt, int wrap_u, int wrap_v, int smooth)
{
    internal_reality_Texture tex;

    if(grab_list) {

        push_list_cmd(LIST_SETTEXTURE, 9); push_list_int(unit); push_list_int(offset); push_list_int(width); push_list_int(height);
        push_list_int(stride);push_list_int(fmt); push_list_int(wrap_u); push_list_int(wrap_v); push_list_int(smooth);
        return;
    }

    tex.swizzle =
    NV30_3D_TEX_SWIZZLE_S0_X_S1 | NV30_3D_TEX_SWIZZLE_S0_Y_S1 |
    NV30_3D_TEX_SWIZZLE_S0_Z_S1 | NV30_3D_TEX_SWIZZLE_S0_W_S1 |
    NV30_3D_TEX_SWIZZLE_S1_X_X | NV30_3D_TEX_SWIZZLE_S1_Y_Y |
    NV30_3D_TEX_SWIZZLE_S1_Z_Z | NV30_3D_TEX_SWIZZLE_S1_W_W ;

    tex.offset = offset;

    tex.format = fmt |
    NV40_3D_TEX_FORMAT_LINEAR  | 
    NV30_3D_TEX_FORMAT_DIMS_2D |
    NV30_3D_TEX_FORMAT_DMA0 |
    NV30_3D_TEX_FORMAT_NO_BORDER | (0x8000) |
    (1 << NV40_3D_TEX_FORMAT_MIPMAP_COUNT__SHIFT);

    tex.wrap = NV30_3D_TEX_WRAP_R_REPEAT;

    switch(wrap_u) {

        case TEXTWRAP_CLAMP:
            tex.wrap |= NV30_3D_TEX_WRAP_S_CLAMP_TO_EDGE;
            break;
        case TEXTWRAP_MIRROR:
            tex.wrap |= NV40_3D_TEX_WRAP_S_MIRROR_CLAMP_TO_EDGE;
            break;
        default:
            tex.wrap |= NV30_3D_TEX_WRAP_S_REPEAT;
            break;
    }

    switch(wrap_v) {

        case TEXTWRAP_CLAMP:
            tex.wrap |= NV30_3D_TEX_WRAP_T_CLAMP_TO_EDGE;
            break;
        case TEXTWRAP_MIRROR:
            tex.wrap |= NV40_3D_TEX_WRAP_T_MIRROR_CLAMP_TO_EDGE;
            break;
        default:
            tex.wrap |= NV30_3D_TEX_WRAP_T_REPEAT;
            break;
    }

   // if(clamp_v) tex.wrap |= NV30_3D_TEX_WRAP_T_CLAMP_TO_EDGE; else tex.wrap |= NV30_3D_TEX_WRAP_T_REPEAT;

    tex.enable = NV40_3D_TEX_ENABLE_ENABLE;

    if(smooth)
    tex.filter = NV30_3D_TEX_FILTER_MIN_LINEAR |
           NV30_3D_TEX_FILTER_MAG_LINEAR | 0x3fd6;
    else
    tex.filter = NV30_3D_TEX_FILTER_MIN_NEAREST |
           NV30_3D_TEX_FILTER_MAG_NEAREST | 0x3fd6;

    tex.width = width;
    tex.height = height;
    tex.stride = stride;

    internal_reality_SetTexture(context, unit, &tex);
}

void * tiny3d_AllocTexture(u32 size)
{
   return rsxMemalign(64, size);
}

void tiny3d_FreeTexture(void *ptr)
{
   rsxFree(ptr);
}

u32 tiny3d_TextureOffset(void * text)
{
    u32 offset = 0;

    assert(rsxAddressToOffset(text, &offset)==0);
    
    return offset;
}

/***********************************************************************************************************/
/* LIGHTS & MATERIALS                                                                                      */
/***********************************************************************************************************/

void tiny3d_EmissiveMaterial(float r, float g, float b, float a)
{
    if(grab_list) {

        push_list_cmd(LIST_EMISSIVE_MAT, 4); push_list_float(r); push_list_float(g); push_list_float(b); push_list_float(a);

        return;
    }

    material.emissive[0] = r;
    material.emissive[1] = g;
    material.emissive[2] = b;
    material.emissive[3] = a;

    flag_vertex |= VERTEX_SETMAT;
}

void tiny3d_AmbientMaterial(float r, float g, float b, float a)
{
    if(grab_list) {

        push_list_cmd(LIST_AMBIENT_MAT, 4); push_list_float(r); push_list_float(g); push_list_float(b); push_list_float(a);

        return;
    }

    material.ambient[0] = r;
    material.ambient[1] = g;
    material.ambient[2] = b;
    material.ambient[3] = a;

    flag_vertex |= VERTEX_SETMAT;
}

void tiny3d_DiffuseMaterial(float r, float g, float b, float a)
{
    if(grab_list) {

        push_list_cmd(LIST_DIFFUSE_MAT, 4); push_list_float(r); push_list_float(g); push_list_float(b); push_list_float(a);

        return;
    }

    material.diffuse[0] = r;
    material.diffuse[1] = g;
    material.diffuse[2] = b;
    material.diffuse[3] = a;

    flag_vertex |= VERTEX_SETMAT;
}

void tiny3d_SpecularMaterial(float r, float g, float b, float shininess)
{
    if(grab_list) {

        push_list_cmd(LIST_SPECULAR_MAT, 4); push_list_float(r); push_list_float(g); push_list_float(b); push_list_float(shininess);

        return;
    }

    material.specular[0] = r;
    material.specular[1] = g;
    material.specular[2] = b;
    material.specular[3] = shininess;

    flag_vertex |= VERTEX_SETMAT;
}

void tiny3d_SetLightCameraPosition(float x, float y, float z)
{

    light.camera[0] = x;
    light.camera[1] = y;
    light.camera[2] = z;
    light.camera[3] = 0.0f;

    flag_vertex |= VERTEX_SETCAMERA;    
}

void tiny3d_SetLight(int indx, float x, float y, float z, float r, float g, float b, int mode)
{

    if(indx < 0 || indx > 3) return;

    light.pos[indx][0] = x;
    light.pos[indx][1] = y;
    light.pos[indx][2] = z;
    light.pos[indx][3] = mode ? ((mode == LIGHT_DIFFUSE) ? 0.5f : 1.0f) : 0.0f;

    light.color[indx][0] = r;
    light.color[indx][1] = g;
    light.color[indx][2] = b;
    light.color[indx][3] = 0.0f;

    flag_vertex |= VERTEX_SETLIGHT;    
}

void tiny3d_SetLightsOff()
{
    int n;

    for(n = 0; n < 4; n++)
        light.pos[n][3] = 0.0f;

    flag_vertex |= VERTEX_SETLIGHT;
}

void tiny3d_SetAmbientLight(float r, float g, float b)
{

    light.ambient[0] = r;
    light.ambient[1] = g;
    light.ambient[2] = b;
    light.ambient[3] = 0.0f;

    flag_vertex |= VERTEX_SETAMBIENT;    
}

/***********************************************************************************************************/
/* SHADERS CONTROL                                                                                         */
/***********************************************************************************************************/

void tiny3d_SelMultiTexturesMethod(u32 method)
{
    if(grab_list) {

        push_list_cmd(LIST_MULTITEXTURE, 1); push_list_int(method); 

        return;
    }

    if(method > 2) method = 2;

    select_fp = 128 | (method & 15);
}

/***********************************************************************************************************/
/* MISC                                                                                                    */
/***********************************************************************************************************/

int tiny3d_MenuActive() {

    return tiny3d_menu_open;

}

void tiny3d_Enable_YUV(int select) {

    current_shader = -1;   
    enable_yuv =1 + (select & 1);
}

void tiny3d_Disable_YUV() {

    current_shader = -1;   
    enable_yuv = 0;
}

void tiny3d_Dirty_Status() {

    tiny3d_Disable_YUV();

    n_vertex = 0;

    polygon = -1;

    flag_vertex = (flag_vertex & VERTEX_MASK) |  VERTEX_LOCK;

    tiny3d_Project2D();
    
}
