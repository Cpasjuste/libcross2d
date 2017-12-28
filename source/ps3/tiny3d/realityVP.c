#include "realityVP.h"
#include "string.h"

void *internal_reality_VertexProgramGetUCode(internal_reality_VertexProgram *vertexprogram) {
    unsigned char *ptr = (unsigned char *) vertexprogram;

    return (void *) (ptr + vertexprogram->UCodeOffset);
}

unsigned int internal_reality_VertexProgramGetInputMask(internal_reality_VertexProgram *vertexprogram) {
    return vertexprogram->InputMask;
}

unsigned int internal_reality_VertexProgramGetOutputMask(internal_reality_VertexProgram *vertexprogram) {
    return vertexprogram->OutputMask;
}

internal_reality_VertexProgramAttribute *
internal_reality_VertexProgramGetAttributes(internal_reality_VertexProgram *vertexprogram) {
    return (internal_reality_VertexProgramAttribute *) (((unsigned char *) vertexprogram) +
                                                        vertexprogram->AttributesOffset);
}

int internal_reality_VertexProgramGetInputAttribute(internal_reality_VertexProgram *vertexprogram, const char *name) {
    int i;
    internal_reality_VertexProgramAttribute *attributes = internal_reality_VertexProgramGetAttributes(vertexprogram);
    for (i = 0; i < vertexprogram->NumAttributes; ++i) {
        char *namePtr;
        if (attributes[i].NameOffset == 0)
            continue;
        namePtr = ((char *) vertexprogram) + attributes[i].NameOffset;
        if (strcasecmp(name, namePtr) == 0)
            return attributes[i].Index;
    }

    return -1;
}

internal_reality_VertexProgramConstant *
internal_reality_VertexProgramGetConstants(internal_reality_VertexProgram *vertexprogram) {
    return (internal_reality_VertexProgramConstant *) (((unsigned char *) vertexprogram) +
                                                       vertexprogram->ConstantsOffset);
}

int internal_reality_VertexProgramGetConstant(internal_reality_VertexProgram *vertexprogram, const char *name) {
    int i;
    internal_reality_VertexProgramConstant *constants = internal_reality_VertexProgramGetConstants(vertexprogram);
    for (i = 0; i < vertexprogram->NumConstants; ++i) {
        char *namePtr;
        if (constants[i].NameOffset == 0)
            continue;
        namePtr = ((char *) vertexprogram) + constants[i].NameOffset;
        if (strcasecmp(name, namePtr) == 0)
            return constants[i].Index;
    }

    return -1;
}