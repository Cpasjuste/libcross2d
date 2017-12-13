//
// Created by cpasjuste on 01/12/16.
//

#include "../../include/C2D.h"

#define TEXT_VTX_ARRAY_COUNT (4*1024)

CTRFont::CTRFont(Renderer *r, const char *p, int s) : Font(r, p, s) {

#ifdef __CITRO3D__
    Result res = fontEnsureMapped();
    if (R_FAILED(res)) {
        printf("fontEnsureMapped: %08lX\n", res);
        return;
    }

    int i;
    TGLP_s *glyphInfo = fontGetGlyphInfo();

    glyphSheets = (C3D_Tex *) malloc(sizeof(C3D_Tex) * glyphInfo->nSheets);
    if (!glyphSheets) {
        this->size = 0;
        return;
    }

    for (i = 0; i < glyphInfo->nSheets; i++) {
        C3D_Tex *tex = &glyphSheets[i];
        tex->data = fontGetGlyphSheetTex(i);
        tex->fmt = (GPU_TEXCOLOR) glyphInfo->sheetFmt;
        tex->size = glyphInfo->sheetSize;
        tex->width = glyphInfo->sheetWidth;
        tex->height = glyphInfo->sheetHeight;
        tex->param = GPU_TEXTURE_MAG_FILTER(GPU_LINEAR) | GPU_TEXTURE_MIN_FILTER(GPU_LINEAR)
                     | GPU_TEXTURE_WRAP_S(GPU_CLAMP_TO_EDGE) | GPU_TEXTURE_WRAP_T(GPU_CLAMP_TO_EDGE);
        tex->border = 0;
        tex->lodParam = 0;
    }

    // Create the text vertex array
    textVtxArray = (textVertex_s *) linearAlloc(sizeof(textVertex_s) * TEXT_VTX_ARRAY_COUNT);
#else

#endif

}

void CTRFont::AddTextVertex(float vx, float vy, float tx, float ty) {
#ifdef __CITRO3D__
    textVertex_s *vtx = &textVtxArray[textVtxArrayPos++];
    vtx->position[0] = vx;
    vtx->position[1] = vy;
    vtx->position[2] = 0.5f;
    vtx->texcoord[0] = tx;
    vtx->texcoord[1] = ty;
#endif
}

void CTRFont::Draw(int x, int y, const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

#ifdef __CITRO3D__
    ssize_t units;
    uint32_t code;

    ((CTRRenderer *) renderer)->StartDrawing(false);

    C3D_TexEnv *env = C3D_GetTexEnv(0);
    C3D_TexEnvSrc(env, C3D_RGB, GPU_CONSTANT, 0, 0);
    C3D_TexEnvSrc(env, C3D_Alpha, GPU_TEXTURE0, GPU_CONSTANT, 0);
    C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
    C3D_TexEnvFunc(env, C3D_RGB, GPU_REPLACE);
    C3D_TexEnvFunc(env, C3D_Alpha, GPU_MODULATE);

    u32 col = (u32) (((color.a) & 0xFF) << 24) | (((color.b) & 0xFF) << 16) | (((color.g) & 0xFF) << 8) |
              (((color.r) & 0xFF) << 0);

    C3D_TexEnvColor(env, col);

    // Configure buffers
    C3D_BufInfo *bufInfo = C3D_GetBufInfo();
    BufInfo_Init(bufInfo);
    BufInfo_Add(bufInfo, textVtxArray, sizeof(textVertex_s), 2, 0x10);

    float scaleX = scaling / 2.0f;
    float scaleY = scaling / 2.0f;

    const uint8_t *p = (const uint8_t *) msg;
    float firstX = x;
    u32 flags = GLYPH_POS_CALC_VTXCOORD;
    int lastSheet = -1;

    do {
        if (!*p) break;
        units = decode_utf8(&code, p);
        if (units == -1)
            break;
        p += units;
        if (code == '\n') {
            x = (int) firstX;
            y += scaleY * fontGetInfo()->lineFeed;
        } else if (code > 0) {
            int glyphIdx = fontGlyphIndexFromCodePoint(code);
            fontGlyphPos_s data;
            fontCalcGlyphPos(&data, glyphIdx, flags, scaleX, scaleY);

            // Bind the correct texture sheet
            if (data.sheetIndex != lastSheet) {
                lastSheet = data.sheetIndex;
                C3D_TexBind(0, &glyphSheets[lastSheet]);
            }

            int arrayIndex = textVtxArrayPos;
            if ((arrayIndex + 4) >= TEXT_VTX_ARRAY_COUNT) {
                arrayIndex = textVtxArrayPos = 0;
            }

            // Add the vertices to the array
            AddTextVertex(x + data.vtxcoord.left, y + data.vtxcoord.bottom, data.texcoord.left, data.texcoord.bottom);
            AddTextVertex(x + data.vtxcoord.right, y + data.vtxcoord.bottom, data.texcoord.right, data.texcoord.bottom);
            AddTextVertex(x + data.vtxcoord.left, y + data.vtxcoord.top, data.texcoord.left, data.texcoord.top);
            AddTextVertex(x + data.vtxcoord.right, y + data.vtxcoord.top, data.texcoord.right, data.texcoord.top);

            // Draw the glyph
            C3D_DrawArrays(GPU_TRIANGLE_STRIP, arrayIndex, 4);

            x += data.xAdvance;

        }
    } while (code > 0);
#else

#endif
}

int CTRFont::GetWidth(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

#ifdef __CITRO3D__
    ssize_t units;
    uint32_t code;

    int width = 0;
    float scaleX = scaling / 2.0f;
    float scaleY = scaling / 2.0f;

    const uint8_t *p = (const uint8_t *) msg;
    do {
        if (!*p) break;
        units = decode_utf8(&code, p);
        if (units == -1)
            break;
        p += units;
        if (code > 0) {
            int glyphIdx = fontGlyphIndexFromCodePoint(code);
            fontGlyphPos_s data;
            fontCalcGlyphPos(&data, glyphIdx, GLYPH_POS_CALC_VTXCOORD, scaleX, scaleY);
            width += data.xAdvance;
        }
    } while (code > 0);

    return width;
#else
    return 0;
#endif

}

int CTRFont::GetHeight(const char *fmt, ...) {

#ifdef __CITRO3D__
    return (int) ((float) fontGetInfo()->height * (scaling / 2.0f));
#else
    return 0;
#endif
}

CTRFont::~CTRFont() {

#ifdef __CITRO3D__
    if (glyphSheets != NULL) {
        free(glyphSheets);
        glyphSheets = NULL;
    }
#else

#endif
}
