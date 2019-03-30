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
#include <cmath>
#include "cross2d/c2d.h"

using namespace c2d;

namespace {

    // Add an underline or strikethrough line to the vertex array
    void addLine(c2d::VertexArray &vertices, float lineLength, float lineTop, const c2d::Color &color, float offset,
                 float thickness, float outlineThickness = 0) {
        float top = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
        float bottom = top + std::floor(thickness + 0.5f);

        vertices.append(
                c2d::Vertex(c2d::Vector2f(-outlineThickness, top - outlineThickness), color, c2d::Vector2f(1, 1)));
        vertices.append(c2d::Vertex(c2d::Vector2f(lineLength + outlineThickness, top - outlineThickness), color,
                                    c2d::Vector2f(1, 1)));
        vertices.append(
                c2d::Vertex(c2d::Vector2f(-outlineThickness, bottom + outlineThickness), color, c2d::Vector2f(1, 1)));
        vertices.append(
                c2d::Vertex(c2d::Vector2f(-outlineThickness, bottom + outlineThickness), color, c2d::Vector2f(1, 1)));
        vertices.append(c2d::Vertex(c2d::Vector2f(lineLength + outlineThickness, top - outlineThickness), color,
                                    c2d::Vector2f(1, 1)));
        vertices.append(c2d::Vertex(c2d::Vector2f(lineLength + outlineThickness, bottom + outlineThickness), color,
                                    c2d::Vector2f(1, 1)));
    }

    // Add a glyph quad to the vertex array
    void
    addGlyphQuad(c2d::VertexArray &vertices, c2d::Vector2f position, const c2d::Color &color, const c2d::Glyph &glyph,
                 float italic, float outlineThickness = 0) {
        float left = glyph.bounds.left;
        float top = glyph.bounds.top;
        float right = glyph.bounds.left + glyph.bounds.width;
        float bottom = glyph.bounds.top + glyph.bounds.height;

        float u1 = static_cast<float>(glyph.textureRect.left);
        float v1 = static_cast<float>(glyph.textureRect.top);
        float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
        float v2 = static_cast<float>(glyph.textureRect.top + glyph.textureRect.height);

        vertices.append(c2d::Vertex(
                c2d::Vector2f(position.x + left - italic * top - outlineThickness, position.y + top - outlineThickness),
                color, c2d::Vector2f(u1, v1)));
        vertices.append(c2d::Vertex(c2d::Vector2f(position.x + right - italic * top - outlineThickness,
                                                  position.y + top - outlineThickness), color, c2d::Vector2f(u2, v1)));
        vertices.append(c2d::Vertex(c2d::Vector2f(position.x + left - italic * bottom - outlineThickness,
                                                  position.y + bottom - outlineThickness), color,
                                    c2d::Vector2f(u1, v2)));
        vertices.append(c2d::Vertex(c2d::Vector2f(position.x + left - italic * bottom - outlineThickness,
                                                  position.y + bottom - outlineThickness), color,
                                    c2d::Vector2f(u1, v2)));
        vertices.append(c2d::Vertex(c2d::Vector2f(position.x + right - italic * top - outlineThickness,
                                                  position.y + top - outlineThickness), color, c2d::Vector2f(u2, v1)));
        vertices.append(c2d::Vertex(c2d::Vector2f(position.x + right - italic * bottom - outlineThickness,
                                                  position.y + bottom - outlineThickness), color,
                                    c2d::Vector2f(u2, v2)));
    }
}


namespace c2d {

////////////////////////////////////////////////////////////
    Text::Text() :
            m_string(),
            m_font(c2d_renderer->getFont()),
            m_characterSize(C2D_DEFAULT_CHAR_SIZE),
            m_style(Regular),
            m_overflow(Clamp),
            m_fillColor(255, 255, 255),
            m_outlineColor(0, 0, 0),
            m_outlineThickness(0),
            m_vertices(Triangles),
            m_outlineVertices(Triangles),
            m_bounds(),
            m_geometryNeedUpdate(false) {
        type = Type::Text;
    }


////////////////////////////////////////////////////////////
    Text::Text(const std::string &string, unsigned int characterSize, Font *font) :
            m_string(string),
            m_font(font),
            m_characterSize(characterSize),
            m_style(Regular),
            m_overflow(Clamp),
            m_fillColor(255, 255, 255),
            m_outlineColor(0, 0, 0),
            m_outlineThickness(0),
            m_vertices(Triangles),
            m_outlineVertices(Triangles),
            m_bounds(),
            m_geometryNeedUpdate(true) {
        type = Type::Text;
        if (!m_font) {
            m_font = c2d_renderer->getFont();
        }
        ensureGeometryUpdate();
    }


////////////////////////////////////////////////////////////
    void Text::setString(const std::string &string) {
        if (m_string != string) {
            m_string = string;
            m_geometryNeedUpdate = true;
            // PSP2 fix: update now as we can't delete vita2d_texture when drawing

            // ensureGeometryUpdate();
        }
    }


////////////////////////////////////////////////////////////
    void Text::setFont(const Font &font) {
        if (m_font != &font) {
            m_font = &font;
            m_geometryNeedUpdate = true;
        }
    }


////////////////////////////////////////////////////////////
    void Text::setCharacterSize(unsigned int size) {
        if (m_characterSize != size) {
            m_characterSize = size;
            m_geometryNeedUpdate = true;
        }
    }


////////////////////////////////////////////////////////////
    void Text::setStyle(uint32_t style) {
        if (m_style != style) {
            m_style = style;
            m_geometryNeedUpdate = true;
        }
    }

    void Text::setOverflow(uint32_t overflow) {
        if (m_overflow != overflow) {
            m_overflow = overflow;
            m_geometryNeedUpdate = true;
        }
    }

////////////////////////////////////////////////////////////
    void Text::setFillColor(const Color &color) {
        if (color != m_fillColor) {
            m_fillColor = color;

            // Change vertex colors directly, no need to update whole geometry
            // (if geometry is updated anyway, we can skip this step)
            if (!m_geometryNeedUpdate) {
                for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i)
                    m_vertices[i].color = m_fillColor;
                m_vertices.update();
            }
        }
    }

////////////////////////////////////////////////////////////
    void Text::setOutlineColor(const Color &color) {
        if (color != m_outlineColor) {
            m_outlineColor = color;

            // Change vertex colors directly, no need to update whole geometry
            // (if geometry is updated anyway, we can skip this step)
            if (!m_geometryNeedUpdate) {
                for (std::size_t i = 0; i < m_outlineVertices.getVertexCount(); ++i)
                    m_outlineVertices[i].color = m_outlineColor;
                m_outlineVertices.update();
            }
        }
    }


////////////////////////////////////////////////////////////
    void Text::setOutlineThickness(float thickness) {
        if (thickness != m_outlineThickness) {
            m_outlineThickness = thickness;
            m_geometryNeedUpdate = true;
        }
    }

/////////////////////////////////////////////////////////////
    void Text::setAlpha(uint8_t alpha, bool recursive) {

        if (alpha != m_fillColor.a) {
            Color color;
            color = m_fillColor;
            color.a = alpha;
            setFillColor(color);
            color = m_outlineColor;
            color.a = alpha;
            setOutlineColor(color);
        }

        if (recursive) {
            C2DObject::setAlpha(alpha, recursive);
        }
    }

    uint8_t Text::getAlpha() {
        return m_fillColor.a;
    }


////////////////////////////////////////////////////////////
    const std::string &Text::getString() const {
        return m_string;
    }


////////////////////////////////////////////////////////////
    const Font *Text::getFont() const {
        return m_font;
    }


////////////////////////////////////////////////////////////
    unsigned int Text::getCharacterSize() const {
        return m_characterSize;
    }


////////////////////////////////////////////////////////////
    uint32_t Text::getStyle() const {
        return m_style;
    }

    uint32_t Text::getOverflow() const {
        return m_overflow;
    }

////////////////////////////////////////////////////////////
    const Color &Text::getFillColor() const {
        return m_fillColor;
    }


////////////////////////////////////////////////////////////
    const Color &Text::getOutlineColor() const {
        return m_outlineColor;
    }


////////////////////////////////////////////////////////////
    float Text::getOutlineThickness() const {
        return m_outlineThickness;
    }


////////////////////////////////////////////////////////////
    Vector2f Text::findCharacterPos(std::size_t index) const {
        // Make sure that we have a valid font
        if (!m_font)
            return Vector2f();

        // Adjust the index if it's out of range
        //if (index > m_string.getSize())
        //    index = m_string.getSize();
        if (index > m_string.length())
            index = m_string.length();

        // Precompute the variables needed by the algorithm
        bool bold = (m_style & Bold) != 0;
        float hspace = static_cast<float>(m_font->getGlyph(L' ', m_characterSize, bold).advance);
        float vspace = static_cast<float>(m_font->getLineSpacing(m_characterSize)) + m_line_spacing;

        // Compute the position
        Vector2f position;
        uint32_t prevChar = 0;
        for (std::size_t i = 0; i < index; ++i) {
            uint32_t curChar = m_string[i];

            // Apply the kerning offset
            position.x += static_cast<float>(m_font->getKerning(prevChar, curChar, m_characterSize));
            prevChar = curChar;

            // Handle special characters
            switch (curChar) {
                case ' ':
                    position.x += hspace;
                    continue;
                case '\t':
                    position.x += hspace * 4;
                    continue;
                case '\n':
                    position.y += vspace;
                    position.x = 0;
                    continue;
            }

            // For regular characters, add the advance offset of the glyph
            position.x += static_cast<float>(m_font->getGlyph(curChar, m_characterSize, bold).advance);
        }

        // Transform the position to global coordinates
        position = getTransform().transformPoint(position);

        return position;
    }


////////////////////////////////////////////////////////////
    FloatRect Text::getLocalBounds() const {
        ensureGeometryUpdate();
        return m_bounds;
    }


////////////////////////////////////////////////////////////
    FloatRect Text::getGlobalBounds() const {
        Transform t = transformation * getTransform();
        return t.transformRect(getLocalBounds());
        //return getTransform().transformRect(getLocalBounds());
    }


////////////////////////////////////////////////////////////
    void Text::setOrigin(const Origin &origin) {

        m_text_origin = origin;
        float height = m_bounds.height > m_characterSize ? m_bounds.height : m_characterSize;

        switch (origin) {
            case Origin::Left:
                Transformable::setOriginVector(0, height / 2);
                break;
            case Origin::TopLeft:
                Transformable::setOriginVector(0, 0);
                break;
            case Origin::Top:
                Transformable::setOriginVector(m_bounds.width / 2, 0);
                break;
            case Origin::TopRight:
                Transformable::setOriginVector(m_bounds.width, 0);
                break;
            case Origin::Right:
                Transformable::setOriginVector(m_bounds.width, height / 2);
                break;
            case Origin::BottomRight:
                Transformable::setOriginVector(m_bounds.width, height);
                break;
            case Origin::Bottom:
                Transformable::setOriginVector(m_bounds.width / 2, height);
                break;
            case Origin::BottomLeft:
                Transformable::setOriginVector(0, height);
                break;
            case Origin::Center:
                Transformable::setOriginVector(m_bounds.width / 2, height / 2);
                break;
            default:
                break;
        }
    }

    Origin Text::getOrigin() const {
        return m_text_origin;
    }

////////////////////////////////////////////////////////////

    void Text::setSizeMax(const Vector2f &size) {
        maxSize = size;
        m_geometryNeedUpdate = true;
    }

    void Text::setSizeMax(float width, float height) {
        maxSize.x = width;
        maxSize.y = height;
        m_geometryNeedUpdate = true;
    }

    void Text::setLineSpacingModifier(int size) {
        m_line_spacing = size;
    }

    void Text::onDraw(Transform &transform, bool draw) {

        if (m_string.empty()) {
            return;
        }

        ensureGeometryUpdate();
        setOrigin(m_text_origin);

        if (draw) {
            Transform combined = transform * getTransform();
            if (getOutlineThickness() > 0) {
                c2d_renderer->draw(&m_outlineVertices, combined, &getFont()->getTexture(getCharacterSize()));
            }
            c2d_renderer->draw(&m_vertices, combined, &getFont()->getTexture(getCharacterSize()));
        }
        C2DObject::onDraw(transform, draw);
    }

////////////////////////////////////////////////////////////
    void Text::ensureGeometryUpdate() const {

        // Do nothing, if geometry has not changed
        if (!m_geometryNeedUpdate)
            return;

        // Mark geometry as updated
        m_geometryNeedUpdate = false;

        // Clear the previous geometry
        m_vertices.clear();
        m_outlineVertices.clear();
        m_bounds = FloatRect();

        // No font or text: nothing to draw
        if (!m_font || m_string.empty())
            return;

        // Compute values related to the text style
        bool bold = (m_style & Bold) != 0;
        bool underlined = (m_style & Underlined) != 0;
        bool strikeThrough = (m_style & StrikeThrough) != 0;
        float italic = (m_style & Italic) ? 0.208f : 0.f; // 12 degrees
        float underlineOffset = m_font->getUnderlinePosition(m_characterSize);
        float underlineThickness = m_font->getUnderlineThickness(m_characterSize);

        // Compute the location of the strike through dynamically
        // We use the center point of the lowercase 'x' glyph as the reference
        // We reuse the underline thickness as the thickness of the strike through as well
        FloatRect xBounds = m_font->getGlyph(L'x', m_characterSize, bold).bounds;
        float strikeThroughOffset = xBounds.top + xBounds.height / 2.f;

        // Precompute the variables needed by the algorithm
        float hspace = static_cast<float>(m_font->getGlyph(L' ', m_characterSize, bold).advance);
        float vspace = static_cast<float>(m_font->getLineSpacing(m_characterSize)) + m_line_spacing;

        float x = 0.f;
        float y = static_cast<float>(m_characterSize);

        // fix top not at 0 if needed (font->setOffset)
        // TODO: find a better fix
        float scale = (float) getCharacterSize() / (float) C2D_DEFAULT_CHAR_SIZE;
        x += m_font->getOffset().x * scale;
        y += m_font->getOffset().y * scale;

        // Create one quad for each character
        float minX = static_cast<float>(m_characterSize);
        float minY = static_cast<float>(m_characterSize);
        float maxX = 0.f;
        float maxY = 0.f;
        uint32_t prevChar = 0;
        for (std::size_t i = 0; i < m_string.length(); ++i) {

            auto curChar = (uint32_t) m_string[i];

            // handle maxSize.y
            if (maxSize.y > 0 && (y * getScale().y) > maxSize.y + 1) {
                break;
            }

            // Apply the kerning offset
            x += m_font->getKerning(prevChar, curChar, m_characterSize);
            prevChar = curChar;

            // handle maxSize.x
            if (maxSize.x > 0 && ((x + m_characterSize) * getScale().x > maxSize.x)) {
                if (m_overflow == Clamp) {
                    while (i < m_string.length()) {
                        curChar = (uint32_t) m_string[i];
                        if (curChar == '\n') {
                            break;
                        }
                        i++;
                    }
                    if (i >= m_string.length()) {
                        break;
                    }
                } else {
                    y += vspace;
                    x = 0;
                }
            }

            // If we're using the underlined style and there's a new line, draw a line
            if (underlined && (curChar == L'\n')) {
                addLine(m_vertices, x, y, m_fillColor, underlineOffset, underlineThickness);

                if (m_outlineThickness != 0)
                    addLine(m_outlineVertices, x, y, m_outlineColor, underlineOffset, underlineThickness,
                            m_outlineThickness);
            }

            // If we're using the strike through style and there's a new line, draw a line across all characters
            if (strikeThrough && (curChar == L'\n')) {
                addLine(m_vertices, x, y, m_fillColor, strikeThroughOffset, underlineThickness);

                if (m_outlineThickness != 0)
                    addLine(m_outlineVertices, x, y, m_outlineColor, strikeThroughOffset, underlineThickness,
                            m_outlineThickness);
            }

            // Handle special characters
            if ((curChar == ' ') || (curChar == '\t') || (curChar == '\n')) {
                // Update the current bounds (min coordinates)
                minX = std::min(minX, x);
                minY = std::min(minY, y);

                switch (curChar) {
                    case ' ':
                        x += hspace;
                        break;
                    case '\t':
                        x += hspace * 4;
                        break;
                    case '\n':
                        y += vspace;
                        x = 0;
                        break;
                }

                // Update the current bounds (max coordinates)
                maxX = std::max(maxX, x);
                maxY = std::max(maxY, y);

                // Next glyph, no need to create a quad for whitespace
                continue;
            }


            // Apply the outline
            if (m_outlineThickness != 0) {
                const Glyph &glyph = m_font->getGlyph(curChar, m_characterSize, bold, m_outlineThickness);

                float left = glyph.bounds.left;
                float top = glyph.bounds.top;
                float right = glyph.bounds.left + glyph.bounds.width;
                float bottom = glyph.bounds.top + glyph.bounds.height;

                // Add the outline glyph to the vertices
                addGlyphQuad(m_outlineVertices, Vector2f(x, y), m_outlineColor, glyph, italic, m_outlineThickness);

                // Update the current bounds with the outlined glyph bounds
                minX = std::min(minX, x + left - italic * bottom - m_outlineThickness);
                maxX = std::max(maxX, x + right - italic * top - m_outlineThickness);
                minY = std::min(minY, y + top - m_outlineThickness);
                maxY = std::max(maxY, y + bottom - m_outlineThickness);
            }

            // Extract the current glyph's description
            const Glyph &glyph = m_font->getGlyph(curChar, m_characterSize, bold);

            // Add the glyph to the vertices
            addGlyphQuad(m_vertices, Vector2f(x, y), m_fillColor, glyph, italic);

            // Update the current bounds with the non outlined glyph bounds
            if (m_outlineThickness == 0) {
                float left = glyph.bounds.left;
                float top = glyph.bounds.top;
                float right = glyph.bounds.left + glyph.bounds.width;
                float bottom = glyph.bounds.top + glyph.bounds.height;

                minX = std::min(minX, x + left - italic * bottom);
                maxX = std::max(maxX, x + right - italic * top);
                minY = std::min(minY, y + top);
                maxY = std::max(maxY, y + bottom);
            }

            // Advance to the next character
            x += glyph.advance;
        }

        // If we're using the underlined style, add the last line
        if (underlined && (x > 0)) {
            addLine(m_vertices, x, y, m_fillColor, underlineOffset, underlineThickness);

            if (m_outlineThickness != 0)
                addLine(m_outlineVertices, x, y, m_outlineColor, underlineOffset, underlineThickness,
                        m_outlineThickness);
        }

        // If we're using the strike through style, add the last line across all characters
        if (strikeThrough && (x > 0)) {
            addLine(m_vertices, x, y, m_fillColor, strikeThroughOffset, underlineThickness);

            if (m_outlineThickness != 0)
                addLine(m_outlineVertices, x, y, m_outlineColor, strikeThroughOffset, underlineThickness,
                        m_outlineThickness);
        }

        // Update the bounding rectangle
        m_bounds.left = minX;
        m_bounds.top = minY;
        m_bounds.width = maxX - minX;
        m_bounds.height = maxY - minY;

        m_vertices.update();
        m_outlineVertices.update();
    }

} // namespace sf
