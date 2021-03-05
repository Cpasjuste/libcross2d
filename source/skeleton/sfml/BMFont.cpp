////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "cross2d/c2d.h"
#include "cross2d/skeleton/sfml/BMFont.hpp"

namespace c2d {

////////////////////////////////////////////////////////////
    BMFont::~BMFont() {
        BMFont::cleanup();
    }

    void BMFont::cleanup() {
        m_bmfont.glyphs.clear();
        m_bmfont.kerningPairs.clear();
        if (m_texture) {
            delete (m_texture);
            m_texture = nullptr;
        }
    }

////////////////////////////////////////////////////////////
    bool BMFont::loadFromFile(const std::string &fntPath) {

        char *fileData = nullptr;

        size_t size = c2d_renderer->getIo()->read(fntPath, &fileData);
        if (size < 4) {
            printf("BMFont::loadFromFile(%s): could not read file...\n", fntPath.c_str());
            if (fileData) {
                free(fileData);
                fileData = nullptr;
            }
            return false;
        }

        if (!((fileData[0] == 'B' && fileData[1] == 'M' && fileData[2] == 'F' && fileData[3] == 3))) {
            printf("BMFont::loadFromFile(%s): BMF header not found...\n", fntPath.c_str());
            free(fileData);
            fileData = nullptr;
            return false;
        }

        bmfont_stream stream((uint8_t *) fileData, size);
        stream.offsetBy(4);
        while (!stream.isEOF()) {
            uint8_t blockID = stream.getU8();
            int32_t blockSize = stream.getU32();
            switch (blockID) {
                case BMFONT_BLOCK_TYPE_INFO: {
                    m_bmfont.info.fontSize = stream.getS16();
                    m_bmfont.info.bitField = stream.getU8();
                    m_bmfont.info.charSet = stream.getU8();
                    m_bmfont.info.stretchH = stream.getU16();
                    m_bmfont.info.aa = stream.getU8();
                    m_bmfont.info.paddingUp = stream.getU8();
                    m_bmfont.info.paddingRight = stream.getU8();
                    m_bmfont.info.paddingDown = stream.getU8();
                    m_bmfont.info.paddingLeft = stream.getU8();
                    m_bmfont.info.spacingHoriz = stream.getU8();
                    m_bmfont.info.spacingVert = stream.getU8();
                    m_bmfont.info.outline = stream.getU8();
                    const char *name = (const char *) stream.getPtr();
                    stream.offsetBy(strlen(name) + 1);
                    break;
                }
                case BMFONT_BLOCK_TYPE_COMMON: {
                    m_bmfont.common.lineHeight = stream.getU16();
                    m_bmfont.common.base = stream.getU16();
                    m_bmfont.common.scaleW = stream.getU16();
                    m_bmfont.common.scaleH = stream.getU16();
                    m_bmfont.common.pages = stream.getU16();
                    m_bmfont.common.bitField = stream.getU8();
                    m_bmfont.common.alphaChnl = stream.getU8();
                    m_bmfont.common.redChnl = stream.getU8();
                    m_bmfont.common.greenChnl = stream.getU8();
                    m_bmfont.common.blueChnl = stream.getU8();
                    break;
                }
                case BMFONT_BLOCK_TYPE_PAGES: {
                    uint16_t pageCount = m_bmfont.common.pages;
                    for (uint16_t index = 0; index < pageCount; ++index) {
                        const char *pageName = (const char *) stream.getPtr();
                        uint8_t len = 0;
                        while (pageName[len] != 0) {
                            m_bmfont.pages[index].name[len] = pageName[len];
                            len++;
                        }
                        m_bmfont.pages[index].length = len;
                        stream.offsetBy(len + 1);
                    }
                    break;
                }
                case BMFONT_BLOCK_TYPE_CHARS: {
                    int32_t charCount = blockSize / 20;
                    for (int32_t index = 0; index < charCount; ++index) {
                        bmfont_char bmfChar = {};
                        bmfChar.id = stream.getU32();
                        bmfChar.x = stream.getU16();
                        bmfChar.y = stream.getU16();
                        bmfChar.width = stream.getU16();
                        bmfChar.height = stream.getU16();
                        bmfChar.xoffset = stream.getS16();
                        bmfChar.yoffset = stream.getS16();
                        bmfChar.xadvance = stream.getS16();
                        bmfChar.page = stream.getU8();
                        bmfChar.chnl = stream.getU8();

                        Glyph glyph = {
                                (float) bmfChar.xadvance + ((float) m_bmfont.info.outline / 2),
                                {
                                        bmfChar.xoffset + m_bmfont.info.outline,
                                        (bmfChar.yoffset - m_bmfont.common.lineHeight) + m_bmfont.info.outline,
                                        bmfChar.width,
                                        bmfChar.height},
                                {bmfChar.x, bmfChar.y, bmfChar.width, bmfChar.height}
                        };
                        m_bmfont.glyphs.insert(std::map<int, Glyph>::value_type(bmfChar.id, glyph));
                    }
                    break;
                }
                case BMFONT_BLOCK_TYPE_KERNINGS: {
                    int32_t kerningPairsCount = blockSize / 10;
                    for (int32_t index = 0; index < kerningPairsCount; ++index) {
                        m_bmfont.kerningPairs.push_back({stream.getU32(), stream.getU32(), stream.getS16()});
                    }
                    break;
                }
                default:
                    break;
            }
        }

        // cleanup
        free(fileData);
        fileData = nullptr;

        std::string texPath = Utility::remove(fntPath, Utility::baseName(fntPath));
        m_texture = new C2DTexture(texPath + m_bmfont.pages[0].name);
        if (!m_texture || !m_texture->available) {
            printf("BMFont::loadFromFile(%s%s): could not create texture...\n",
                   texPath.c_str(), m_bmfont.pages[0].name);
            return false;
        }

        printf("BMFont::loadFromFile(%s): loaded %lu chars (font size: %i, outline size: %i)\n",
               fntPath.c_str(), m_bmfont.glyphs.size(), m_bmfont.info.fontSize, m_bmfont.info.outline);

        return true;
    }

////////////////////////////////////////////////////////////
    bool BMFont::loadFromMemory(const void *data, std::size_t sizeInBytes) {
        // TODO
        printf("BMFont::loadFromMemory: not implemented\n");
        return false;
    }

    bool BMFont::loadDefault() {
        printf("BMFont::loadDefault: not implemented\n");
        return false;
    }

////////////////////////////////////////////////////////////
    Glyph BMFont::getGlyph(uint32_t codePoint, unsigned int characterSize,
                           bool bold, float outlineThickness) {

        if (m_bmfont.glyphs.count(codePoint)) {
            if ((int16_t) characterSize == m_bmfont.info.fontSize) {
                return m_bmfont.glyphs.at(codePoint);
            } else {
                Glyph glyph = m_bmfont.glyphs.at(codePoint);
                float scaling = (float) characterSize / (float) m_bmfont.info.fontSize;
                return {
                        glyph.advance * scaling,
                        {
                                glyph.bounds.left,
                                glyph.bounds.top,
                                glyph.bounds.width * scaling,
                                glyph.bounds.height * scaling
                        },
                        glyph.textureRect
                };
            }
        }

        return m_bmfont.glyphs.begin()->second;
    }

////////////////////////////////////////////////////////////
    float BMFont::getKerning(uint32_t first, uint32_t second, unsigned int characterSize) const {
        if (!m_bmfont.kerningPairs.empty()) {
            float scaling = (float) characterSize / (float) m_bmfont.info.fontSize;
            for (const auto &kerningPair : m_bmfont.kerningPairs) {
                if (kerningPair.first == first && kerningPair.second == second) {
                    return (float) kerningPair.amount * scaling;
                }
            }
        }
        return 0;
    }

////////////////////////////////////////////////////////////
    float BMFont::getLineSpacing(unsigned int characterSize) const {
        float scaling = (float) characterSize / (float) m_bmfont.info.fontSize;
        return (float) m_bmfont.common.lineHeight * scaling;
    }


////////////////////////////////////////////////////////////
    float BMFont::getUnderlinePosition(unsigned int characterSize) const {
        return 0;
    }


////////////////////////////////////////////////////////////
    float BMFont::getUnderlineThickness(unsigned int characterSize) const {
        return 0;
    }


////////////////////////////////////////////////////////////
    Texture *BMFont::getTexture(unsigned int characterSize) {
        return m_texture;
    }

    void BMFont::setFilter(Texture::Filter filter) {
        if (m_texture) {
            m_texture->setFilter(filter);
        }
    }

} // namespace c2d
