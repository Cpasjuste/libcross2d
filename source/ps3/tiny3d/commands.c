#include "commands.h"
#include "nv40v1.h"
#include "buffer.h"

void internal_reality_SetClearColor(gcmContextData *context, uint32_t color) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_CLEAR_COLOR_VALUE, color);
}

void internal_reality_SetClearDepthValue(gcmContextData *context, uint32_t value) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_CLEAR_DEPTH_VALUE, value);
}

void internal_reality_Nop(gcmContextData *context) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, 0x100, 0);
}

void internal_reality_ClearBuffers(gcmContextData *context, uint32_t buffers) {
	INTERNAL_COMMAND_LENGTH(context, 4);
	internal_commandBufferPutCmd1(context, NV30_3D_CLEAR_BUFFERS, buffers);
	internal_commandBufferPutCmd1(context, 0x100, 0); // Nop
}	

const static uint32_t offset_cmds[] = {NV30_3D_COLOR0_OFFSET, NV30_3D_COLOR1_OFFSET,
		NV40_3D_COLOR2_OFFSET, NV40_3D_COLOR3_OFFSET, NV30_3D_ZETA_OFFSET};
const static uint32_t dma_cmds[] = {NV30_3D_DMA_COLOR0, NV30_3D_DMA_COLOR1,
		NV40_3D_DMA_COLOR2, NV40_3D_DMA_COLOR3, NV30_3D_DMA_ZETA};
const static uint32_t pitch_cmds[] = {NV30_3D_COLOR0_PITCH, NV30_3D_COLOR1_PITCH,
		NV40_3D_COLOR2_PITCH, NV40_3D_COLOR3_PITCH, NV40_3D_ZETA_PITCH};

void internal_reality_SetRenderSurface(gcmContextData *context, uint8_t surface, uint8_t location,
				uint32_t offset, uint32_t pitch) {
	INTERNAL_COMMAND_LENGTH(context, 6);
	internal_commandBufferPutCmd1(context, dma_cmds[surface], location | 0xFEED0000);
	internal_commandBufferPutCmd1(context, offset_cmds[surface], offset);
	internal_commandBufferPutCmd1(context, pitch_cmds[surface], pitch);
}

void internal_reality_SelectRenderTarget(gcmContextData *context, uint8_t target, uint32_t format, 
				uint16_t width, uint16_t height, uint16_t x, uint16_t y) {
	INTERNAL_COMMAND_LENGTH(context, 11);

	format |= (31 - __builtin_clz(width)) << 16;
	format |= (31 - __builtin_clz(height)) << 24;
	uint32_t conventions = height | 0x1000; // COORD_CONVENTIONS_ORIGIN_INVERTED
	
	internal_commandBufferPutCmd1(context, NV30_3D_RT_FORMAT, format); 
	internal_commandBufferPutCmd1(context, NV30_3D_RT_ENABLE, target);
	internal_commandBufferPutCmd1(context, NV30_3D_VIEWPORT_TX_ORIGIN, x | y << 16);
	internal_commandBufferPutCmd1(context, NV30_3D_COORD_CONVENTIONS, conventions);
	internal_commandBufferPutCmd2(context, NV30_3D_RT_HORIZ, x | width << 16, y | height << 16);
}

void internal_reality_Viewport(gcmContextData *context, uint16_t width, uint16_t height) {
	INTERNAL_COMMAND_LENGTH(context, 3);
	internal_commandBufferPutCmd2(context, NV30_3D_VIEWPORT_HORIZ, width << 16, height << 16);
}

void internal_reality_ViewportClip(gcmContextData *context, u8 sel, uint16_t width, uint16_t height) {
    INTERNAL_COMMAND_LENGTH(context, 3);
	internal_commandBufferPutCmd2(context, NV30_3D_VIEWPORT_CLIP_HORIZ(sel), (width-1) << 16, (height-1) << 16);
}

void internal_reality_VertexBegin(gcmContextData *context, uint32_t type) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_VERTEX_BEGIN_END, type);
}

void internal_reality_VertexEnd(gcmContextData *context) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_VERTEX_BEGIN_END, NV30_3D_VERTEX_BEGIN_END_STOP);
}

void internal_reality_Vertex4f(gcmContextData *context, float x, float y, float z, float w) {
	INTERNAL_COMMAND_LENGTH(context, 5);
	internal_commandBufferPutCmd4(context, NV30_3D_VTX_ATTR_4F(0),
				 ((ieee32) x).u, 
				 ((ieee32) y).u,
				 ((ieee32) z).u, 
				 ((ieee32) w).u);
}

void internal_reality_TexCoord2f(gcmContextData *context, float s, float t) {
	INTERNAL_COMMAND_LENGTH(context, 3);
	internal_commandBufferPutCmd2(context, NV30_3D_VTX_ATTR_2F(8),
				 ((ieee32) s).u, 
				 ((ieee32) t).u);
}

void internal_reality_LoadVertexProgram_old(gcmContextData *context, internal_reality_VertexProgram_old *prog) {
	int inst, command_size = prog->size * 2 + 7;
	INTERNAL_COMMAND_LENGTH(context, command_size);
	
	internal_commandBufferPutCmd1(context, NV30_3D_VP_UPLOAD_FROM_ID, 0);
	
	for(inst = 0; inst < prog->size; inst += 4) {
		internal_commandBufferPutCmd4(context, NV30_3D_VP_UPLOAD_INST(inst), 
					prog->data[inst + 0],
					prog->data[inst + 1],
					prog->data[inst + 2],
					prog->data[inst + 3]);
	}

	internal_commandBufferPutCmd1(context, NV30_3D_VP_START_FROM_ID, 0);
	internal_commandBufferPutCmd2(context, NV40_3D_VP_ATTRIB_EN, prog->in_reg, prog->out_reg);
}

void internal_reality_LoadVertexProgram(gcmContextData *context, internal_reality_VertexProgram *prog) {
	int inst, command_size = prog->NumInsts * 5 + 7;
	unsigned int *ucode = (unsigned int*)internal_reality_VertexProgramGetUCode(prog);
	internal_reality_VertexProgramConstant *constants;
	INTERNAL_COMMAND_LENGTH(context, command_size);
	
	internal_commandBufferPutCmd1(context, NV30_3D_VP_UPLOAD_FROM_ID, 0);
	
	for(inst = 0; inst < prog->NumInsts*4; inst += 4) {
		internal_commandBufferPutCmd4(context, NV30_3D_VP_UPLOAD_INST(inst), 
					ucode[inst + 0],
					ucode[inst + 1],
					ucode[inst + 2],
					ucode[inst + 3]);
	}

	internal_commandBufferPutCmd1(context, NV30_3D_VP_START_FROM_ID, 0);
	internal_commandBufferPutCmd2(context, NV40_3D_VP_ATTRIB_EN, prog->InputMask, prog->OutputMask);

	constants = internal_reality_VertexProgramGetConstants(prog);

	if(constants)
	{
		int c;
		for(c=0;c<prog->NumConstants;++c)
		{
			internal_reality_SetVertexProgramConstant4f(context,constants[c].Index,(float*)constants[c].Values);
		}
	}
}

void internal_reality_SetVertexProgramConstant4f(gcmContextData *context, int num, float values[4]){

	INTERNAL_COMMAND_LENGTH(context, 6);

	internal_commandBufferPutCmd5(context, NV30_3D_VP_UPLOAD_CONST_ID, 
							num,
							((ieee32) (values[0])).u,
							((ieee32) (values[1])).u,
							((ieee32) (values[2])).u,
							((ieee32) (values[3])).u
							);
}

void internal_reality_SetVertexProgramConstant4fBlock(gcmContextData *context, int constant, int num4fConstants, float *values)
{
	int n;
	for(n=0;n<num4fConstants;++n)
	{
		internal_reality_SetVertexProgramConstant4f(context,constant+n,values+4*n);
	}
}

void internal_reality_InstallFragmentProgram(gcmContextData *context, internal_reality_FragmentProgram *prog, uint32_t *addr) {
	// We don't actually need context, but if we leave it out people will forget.
	int i;
	for( i = 0; i < prog->size; ++i ) {
		addr[i] = (((prog->data[i] >> 16 ) & 0xffff) << 0) |
			    (((prog->data[i] >> 0 ) & 0xffff) << 16);
	}
	assert(rsxAddressToOffset(addr, &prog->offset) == 0);
}

void internal_reality_LoadFragmentProgram(gcmContextData *context, internal_reality_FragmentProgram *prog) {
	INTERNAL_COMMAND_LENGTH(context, 4);
	assert(prog->offset != 0);
	internal_commandBufferPutCmd1(context, NV30_3D_FP_ACTIVE_PROGRAM,
				prog->offset | NV30_3D_FP_ACTIVE_PROGRAM_DMA0);
	internal_commandBufferPutCmd1(context, NV30_3D_FP_CONTROL, 
 				prog->num_regs << NV40_3D_FP_CONTROL_TEMP_COUNT__SHIFT);
}


void internal_reality_SetTexture(gcmContextData *context, uint32_t unit, internal_reality_Texture *tex) {
	INTERNAL_COMMAND_LENGTH(context, 11);
	internal_commandBufferPutCmd8(context, NV30_3D_TEX_OFFSET(unit), tex->offset, tex->format,
		tex->wrap, tex->enable, tex->swizzle, tex->filter,
		tex->width << 16 | tex->height, tex->borderColor);
	internal_commandBufferPutCmd1(context, NV40_3D_TEX_SIZE1(unit), tex->stride | 
		(1 << NV40_3D_TEX_SIZE1_DEPTH__SHIFT));
}

void internal_reality_ViewportTranslate(gcmContextData *context, float x, float y, float z, float w) {
	INTERNAL_COMMAND_LENGTH(context, 5);
	internal_commandBufferPutCmd4(context, NV30_3D_VIEWPORT_TRANSLATE, 
				 ((ieee32) x).u, 
				 ((ieee32) y).u,
				 ((ieee32) z).u, 
				 ((ieee32) w).u);
}

void internal_reality_ViewportScale(gcmContextData *context, float x, float y, float z, float w) {
	INTERNAL_COMMAND_LENGTH(context, 5);
	internal_commandBufferPutCmd4(context, NV30_3D_VIEWPORT_SCALE, 
				 ((ieee32) x).u,
				 ((ieee32) y).u,
				 ((ieee32) z).u, 
				 ((ieee32) w).u);
};

void internal_reality_CullFace(gcmContextData *context, uint32_t face) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_CULL_FACE, face);
}

void internal_reality_FrontFace(gcmContextData *context, uint32_t face) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_FRONT_FACE, face);
}

void internal_reality_CullEnable(gcmContextData *context, uint32_t enable) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_CULL_FACE_ENABLE, enable);
}

void internal_reality_AlphaEnable(gcmContextData *context, uint32_t enable) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_ALPHA_FUNC_ENABLE, enable);
}

void internal_reality_AlphaRef(gcmContextData *context, uint32_t ref) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_ALPHA_FUNC_REF, ref);
}

void internal_reality_AlphaFunc(gcmContextData *context, uint32_t func) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_ALPHA_FUNC_FUNC, func);
}

void internal_reality_BlendEnable(gcmContextData *context, uint32_t enable) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_BLEND_FUNC_ENABLE, enable);
}

void internal_reality_BlendEquation(gcmContextData *context, uint32_t equation) {
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV40_3D_BLEND_EQUATION, equation);
}

void internal_reality_BlendFunc(gcmContextData *context, uint32_t src, uint32_t dest) {
	INTERNAL_COMMAND_LENGTH(context, 3);
	internal_commandBufferPutCmd2(context, NV30_3D_BLEND_FUNC_SRC, src, dest);
}

void internal_reality_ZControl(gcmContextData *context, uint8_t cullNearFar, uint8_t zClampEnable, uint8_t cullIngnoreW) { // todo: documenent in nouveau
	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_DEPTH_CONTROL, cullNearFar | 
				zClampEnable << 4 |
				cullIngnoreW << 8);
}

void internal_reality_BindVertexBufferAttribute(gcmContextData *context, uint8_t attribute, uint32_t offset, uint8_t stride, uint8_t elements, uint8_t dataType, uint8_t location)
{
	INTERNAL_COMMAND_LENGTH(context, 2+2);
	
	internal_commandBufferPutCmd1(context, NV30_3D_VTXBUF(attribute), offset|(location<<31));
	internal_commandBufferPutCmd1(context, NV30_3D_VTXFMT(attribute), (stride<<NV30_3D_VTXFMT_STRIDE__SHIFT)|(elements<<NV30_3D_VTXFMT_SIZE__SHIFT)|dataType);

}

void internal_reality_DrawVertexBuffer(gcmContextData *context, uint32_t type, uint32_t start, uint32_t count)
{
	INTERNAL_COMMAND_LENGTH(context, 4);

	internal_commandBufferPutCmd1(context, NV40_3D_VTX_CACHE_INVALIDATE, 0);
	internal_commandBufferPutCmd1(context, NV30_3D_VERTEX_BEGIN_END, type);

	while(count)
	{
		int num=count; //number of elements this call
		if(num>256)	//max 256 elements per call
			num=256;
		INTERNAL_COMMAND_LENGTH(context, 2);
		internal_commandBufferPutCmd1(context, NV30_3D_VB_VERTEX_BATCH, ((num-1)<<24)|start);
		count-=num;
		start+=num;
	}

	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_VERTEX_BEGIN_END, NV30_3D_VERTEX_BEGIN_END_STOP);
}

void internal_reality_DrawVertexBufferIndex(gcmContextData *context, uint32_t type, uint32_t offset, uint32_t count, uint8_t dataType, uint8_t location)
{
	unsigned int current=0;

	INTERNAL_COMMAND_LENGTH(context, 7);

	internal_commandBufferPutCmd1(context, NV40_3D_VTX_CACHE_INVALIDATE, 0);

	internal_commandBufferPutCmd2(context, NV30_3D_IDXBUF_OFFSET, offset, (uint32_t)(dataType|location));

	internal_commandBufferPutCmd1(context, NV30_3D_VERTEX_BEGIN_END, type);

	while(count)
	{
		int num=count; //number of elements this call
		if(num>256)	//max 256 elements per call
			num=256;		
		INTERNAL_COMMAND_LENGTH(context, 2);
		internal_commandBufferPutCmd1(context, NV30_3D_VB_INDEX_BATCH, ((num-1)<<24)|current);
		count-=num;
		current+=num;
	}

	INTERNAL_COMMAND_LENGTH(context, 2);
	internal_commandBufferPutCmd1(context, NV30_3D_VERTEX_BEGIN_END, NV30_3D_VERTEX_BEGIN_END_STOP);
}

void internal_reality_DepthTestEnable(gcmContextData *context, uint32_t enable)
{
	INTERNAL_COMMAND_LENGTH(context,2);
	internal_commandBufferPutCmd1(context, NV30_3D_DEPTH_TEST_ENABLE, enable);
}

void internal_reality_DepthTestFunc(gcmContextData *context, uint32_t function)
{
	INTERNAL_COMMAND_LENGTH(context,2);
	internal_commandBufferPutCmd1(context, NV30_3D_DEPTH_FUNC, function);
}

void internal_reality_DepthWriteEnable(gcmContextData *context, uint32_t enable)
{
	INTERNAL_COMMAND_LENGTH(context,2);
	internal_commandBufferPutCmd1(context, NV30_3D_DEPTH_WRITE_ENABLE, enable);
}
