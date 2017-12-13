#pragma once

#include <rsx/gcm_sys.h>
#include <rsx/rsx.h>
#include "realityVP.h"

#ifdef __cplusplus
extern "C" {
#endif

// Set the colour for clearing buffers, should be in ARGB format.
void realitySetClearColor(gcmContextData *context, uint32_t color);
void realitySetClearDepthValue(gcmContextData *context, uint32_t value);

// For actually clearing the bufders
#define REALITY_CLEAR_BUFFERS_DEPTH				0x00000001
#define REALITY_CLEAR_BUFFERS_STENCIL				0x00000002
#define REALITY_CLEAR_BUFFERS_COLOR_R				0x00000010
#define REALITY_CLEAR_BUFFERS_COLOR_G				0x00000020
#define REALITY_CLEAR_BUFFERS_COLOR_B				0x00000040
#define REALITY_CLEAR_BUFFERS_COLOR_A				0x00000080
void realityClearBuffers(gcmContextData *context, uint32_t buffers);

// For when you need to do nothing
void realityNop(gcmContextData *context);

#define REALITY_RSX_MEMORY	0
#define REALITY_LOCATION_CELL_MEMORY	1

enum surfaces {REALITY_SURFACE_COLOR0, REALITY_SURFACE_COLOR1, REALITY_SURFACE_COLOR2, 
		REALITY_SURFACE_COLOR3, REALITY_SURFACE_ZETA};
void internal_reality_SetRenderSurface(gcmContextData *context, uint8_t sufrace, uint8_t location, uint32_t offset, uint32_t pitch);


#define REALITY_TARGET_NONE	0x00 /* Don't render anything */
#define REALITY_TARGET_0	0x01 /* Render to color surface 0 */
#define REALITY_TARGET_1	0x02 /* Render to color surface 1 */
#define	REALITY_TARGET_MRT1	0x13 /* Render to both color surfaces 0 and 1 */
#define	REALITY_TARGET_MRT2	0x17 /* Render to color surface 0, 1 and 2 */
#define	REALITY_TARGET_MRT3	0x1F /* Render to color surface 0, 1, 2 and 3 */

#define REALITY_TARGET_FORMAT_COLOR_X1R5G5B5			0x00000001
#define REALITY_TARGET_FORMAT_COLOR_R5G6B5			0x00000003
#define REALITY_TARGET_FORMAT_COLOR_X8R8G8B8			0x00000005
#define REALITY_TARGET_FORMAT_COLOR_A8R8G8B8			0x00000008
#define REALITY_TARGET_FORMAT_COLOR_B8				0x00000009
#define REALITY_TARGET_FORMAT_COLOR_A16B16G16R16_FLOAT		0x0000000b
#define REALITY_TARGET_FORMAT_COLOR_A32B32G32R32_FLOAT		0x0000000c
#define REALITY_TARGET_FORMAT_COLOR_R32_FLOAT			0x0000000d
#define REALITY_TARGET_FORMAT_COLOR_X8B8G8R8			0x0000000f
#define REALITY_TARGET_FORMAT_COLOR_A8B8G8R8			0x00000010

#define REALITY_TARGET_FORMAT_ZETA_Z16				0x00000020
#define REALITY_TARGET_FORMAT_ZETA_Z24S8			0x00000040

#define REALITY_TARGET_FORMAT_TYPE_LINEAR			0x00000100
#define REALITY_TARGET_FORMAT_TYPE_SWIZZLED			0x00000200

#define REALITY_TARGET_FORMAT_MSAA_NONE				0x00000000
#define REALITY_TARGET_FORMAT_MSAA_2X				0x00003000
#define REALITY_TARGET_FORMAT_MSAA_4X				0x00004000
#define REALITY_TARGET_FORMAT_MSAA_4X_ROTATED			0x00005000

void internal_reality_SelectRenderTarget(gcmContextData *context, uint8_t target, uint32_t format, 
				uint16_t width, uint16_t height, uint16_t x, uint16_t y);

void internal_reality_Viewport(gcmContextData *context, uint16_t width, uint16_t height);
void internal_reality_ViewportClip(gcmContextData *context, u8 sel, uint16_t width, uint16_t height);
void internal_reality_ViewportTranslate(gcmContextData *context, float x, float y, float z, float w);
void internal_reality_ViewportScale(gcmContextData *context, float x, float y, float z, float w);


// Vertex begin, end and 4f, excatly like glbegin, glend and glvertex.
// You really should be using vertex buffers instead.
#define REALITY_POINTS				0x00000001
#define REALITY_LINES				0x00000002
#define REALITY_LINE_LOOP			0x00000003
#define REALITY_LINE_STRIP			0x00000004
#define REALITY_TRIANGLES			0x00000005
#define REALITY_TRIANGLE_STRIP			0x00000006
#define REALITY_TRIANGLE_FAN			0x00000007
#define REALITY_QUADS				0x00000008
#define REALITY_QUAD_STRIP			0x00000009
#define REALITY_POLYGON				0x0000000a
void internal_reality_VertexBegin(gcmContextData *context, uint32_t type);
void internal_reality_VertexEnd(gcmContextData *context);
void internal_reality_Vertex4f(gcmContextData *context, float x, float y, float z, float w);

void internal_reality_TexCoord2f(gcmContextData *context, float s, float t);

//Vertex buffers
//Bind an input attribute to a vertex buffer array

#define REALITY_BUFFER_DATATYPE_FLOAT	2
#define REALITY_BUFFER_DATATYPE_BYTE	4

void internal_reality_BindVertexBufferAttribute(gcmContextData *context, uint8_t attribute, uint32_t offset, uint8_t stride, uint8_t elements, uint8_t dataType, uint8_t location);

void internal_reality_DrawVertexBuffer(gcmContextData *context, uint32_t type, uint32_t start, uint32_t count);


#define REALITY_INDEX_DATATYPE_U32	0x00
#define REALITY_INDEX_DATATYPE_U16	0x10
void internal_reality_DrawVertexBufferIndex(gcmContextData *context, uint32_t type, uint32_t offset, uint32_t count, uint8_t dataType, uint8_t location);

typedef struct {
	uint32_t size;
	uint32_t in_reg;
	uint32_t out_reg;
	uint32_t data[];
} internal_reality_VertexProgram_old; 

typedef struct {
	uint32_t offset;
	uint32_t size;
	uint32_t num_regs;
	uint32_t data[];
} internal_reality_FragmentProgram; 

void internal_reality_LoadVertexProgram(gcmContextData *context, internal_reality_VertexProgram *prog);
void internal_reality_LoadVertexProgram_old(gcmContextData *context, internal_reality_VertexProgram_old *prog);
void internal_reality_SetVertexProgramConstant4f(gcmContextData *context, int constant, float values[4]);
void internal_reality_SetVertexProgramConstant4fBlock(gcmContextData *context, int constant, int num4fConstants, float *values);
void internal_reality_InstallFragmentProgram(gcmContextData *context, internal_reality_FragmentProgram *prog, uint32_t *addr);
void internal_reality_LoadFragmentProgram(gcmContextData *context, internal_reality_FragmentProgram *prog);

typedef struct {
	uint32_t offset;
	uint32_t format;
	uint32_t wrap;
	uint32_t enable;
	uint32_t swizzle;
	uint32_t filter;
	uint16_t width;
	uint16_t height;
	uint32_t borderColor;
	uint32_t stride;
} internal_reality_Texture;

void internal_reality_SetTexture(gcmContextData *context, uint32_t unit, internal_reality_Texture *tex);

#define REALITY_FRONT_FACE_CW 			0x00000900
#define REALITY_FRONT_FACE_CCW			0x00000901

#define REALITY_CULL_FACE_FRONT			0x00000404
#define REALITY_CULL_FACE_BACK			0x00000405
#define REALITY_CULL_FACE_FRONT_AND_BACK	0x00000408

void internal_reality_CullFace(gcmContextData *context, uint32_t face);
void internal_reality_FrontFace(gcmContextData *context, uint32_t face);
void internal_reality_CullEnable(gcmContextData *context, uint32_t enable);

void internal_reality_AlphaFunc(gcmContextData *context, uint32_t func);
void internal_reality_AlphaRef(gcmContextData *context, uint32_t ref);
void internal_reality_AlphaEnable(gcmContextData *context, uint32_t enable);

void internal_reality_BlendFunc(gcmContextData *context, uint32_t src, uint32_t dest);
void internal_reality_BlendEquation(gcmContextData *context, uint32_t equation);
void internal_reality_BlendEnable(gcmContextData *context, uint32_t enable);

#define REALITY_ZFUNC_NEVER				0x00000200
#define REALITY_ZFUNC_LESS					0x00000201
#define REALITY_ZFUNC_EQUAL				0x00000202
#define REALITY_ZFUNC_LESSOREQUAL				0x00000203
#define REALITY_ZFUNC_GREATER				0x00000204
#define REALITY_ZFUNC_NOTEQUAL				0x00000205
#define REALITY_ZFUNC_GREATEROREQUAL				0x00000206
#define REALITY_ZFUNC_ALWAYS				0x00000207

void internal_reality_DepthTestEnable(gcmContextData *context, uint32_t enable);
void internal_reality_DepthTestFunc(gcmContextData *context, uint32_t function);
void internal_reality_DepthWriteEnable(gcmContextData *context, uint32_t enable);

void internal_reality_ZControl(gcmContextData *context, uint8_t cullNearFar, uint8_t zClampEnable, uint8_t cullIngnoreW);

#ifdef __cplusplus
}
#endif
