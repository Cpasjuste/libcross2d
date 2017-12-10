#ifndef REALITYVP_H
#define REALITYVP_H


// libreality vertex program


//Param types
#define PARAM_FLOAT		0
#define PARAM_FLOAT2	1
#define PARAM_FLOAT3	2
#define PARAM_FLOAT4	3


#define PARAM_FLOAT4x4	4

#pragma pack(push,1)
//#pragma pack(1)
typedef struct _VPAttribute
{
	unsigned int NameOffset;
	unsigned int Index;
} internal_reality_VertexProgramAttribute;

union _VPFloatUInt
{
	unsigned int u;
	float f;
};

typedef struct _VPConstant
{
	unsigned int NameOffset;
	unsigned int Index;
	unsigned char Type;
	unsigned char Internal;	//Is an internal constant? (unnamed but needs to be set)
	union _VPFloatUInt Values[4];
} internal_reality_VertexProgramConstant;

typedef struct _internal_reality_VertexProgram
{
	unsigned short Magic;		//Magic text 'VP'
	unsigned short NumInsts;	//Number of vertex program instructions
	unsigned int UCodeOffset;	//Microcode offset (16 bytes per instruction)
	unsigned short NumConstants;	//Number of named constants (parameters)
	unsigned short NumAttributes;	//Number of named attributes (inputs)
	unsigned int ConstantsOffset;
	unsigned int AttributesOffset;
	unsigned int InputMask;			//Used input attributes
	unsigned int OutputMask;		//Used output registers

	//data (constants, attributes, ucode)
} internal_reality_VertexProgram;

#pragma pack(pop)




void *internal_reality_VertexProgramGetUCode(internal_reality_VertexProgram *vertexprogram);

unsigned int internal_reality_VertexProgramGetInputMask(internal_reality_VertexProgram *vertexprogram);

unsigned int internal_reality_VertexProgramGetOutputMask(internal_reality_VertexProgram *vertexprogram);

internal_reality_VertexProgramAttribute *internal_reality_VertexProgramGetAttributes(internal_reality_VertexProgram *vertexprogram);

int internal_reality_VertexProgramGetInputAttribute(internal_reality_VertexProgram *vertexprogram,const char *name);

internal_reality_VertexProgramConstant *internal_reality_VertexProgramGetConstants(internal_reality_VertexProgram *vertexprogram);

int internal_reality_VertexProgramGetConstant(internal_reality_VertexProgram *vertexprogram,const char *name);

#endif
