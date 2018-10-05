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
#include <skeleton/sfml/Shape.hpp>

#include "skeleton/sfml/Shape.hpp"
#include "c2d.h"

namespace {
    // Compute the normal of a segment
    c2d::Vector2f computeNormal(const c2d::Vector2f &p1, const c2d::Vector2f &p2) {
        c2d::Vector2f normal(p1.y - p2.y, p2.x - p1.x);
        float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
        if (length != 0.f)
            normal /= length;
        return normal;
    }

    // Compute the dot product of two vectors
    float dotProduct(const c2d::Vector2f &p1, const c2d::Vector2f &p2) {
        return p1.x * p2.x + p1.y * p2.y;
    }
}


namespace c2d {
////////////////////////////////////////////////////////////
    Shape::~Shape() {
    }

////////////////////////////////////////////////////////////
    void Shape::setFillColor(const Color &color) {
        m_fillColor = color;
        updateFillColors();
    }


////////////////////////////////////////////////////////////
    const Color &Shape::getFillColor() const {
        return m_fillColor;
    }

    void Shape::setAlpha(uint8_t alpha) {

        if (alpha != m_fillColor.a) {
            m_fillColor.a = alpha;
            updateFillColors();
            m_outlineColor.a = alpha;
            updateOutlineColors();
        }

        C2DObject::setAlpha(alpha);
    }

    uint8_t Shape::getAlpha() {
        return m_fillColor.a;
    }

////////////////////////////////////////////////////////////
    void Shape::setOutlineColor(const Color &color) {
        m_outlineColor = color;
        updateOutlineColors();
    }


////////////////////////////////////////////////////////////
    const Color &Shape::getOutlineColor() const {
        return m_outlineColor;
    }


////////////////////////////////////////////////////////////
    void Shape::setOutlineThickness(float thickness) {
        m_outlineThickness = thickness;
        update(); // recompute everything because the whole shape must be offset
    }


////////////////////////////////////////////////////////////
    float Shape::getOutlineThickness() const {
        return m_outlineThickness;
    }


////////////////////////////////////////////////////////////
    FloatRect Shape::getLocalBounds() const {
        return m_bounds;
    }


////////////////////////////////////////////////////////////
    FloatRect Shape::getGlobalBounds() const {
        return getTransform().transformRect(getLocalBounds());
    }


////////////////////////////////////////////////////////////

    void Shape::setOrigin(float x, float y) {
        Transformable::setOrigin(x, y);
    }

    void Shape::setOrigin(const Vector2f &origin) {
        Transformable::setOrigin(origin);
    }

    void Shape::setOrigin(const Origin &origin) {

        m_origin = origin;

        switch (origin) {
            case Origin::Left:
                Transformable::setOrigin(0, m_bounds.height / 2);
                break;
            case Origin::TopLeft:
                Transformable::setOrigin(0, 0);
                break;
            case Origin::Top:
                Transformable::setOrigin(m_bounds.width / 2, 0);
                break;
            case Origin::TopRight:
                Transformable::setOrigin(m_bounds.width, 0);
                break;
            case Origin::Right:
                Transformable::setOrigin(m_bounds.width, m_bounds.height / 2);
                break;
            case Origin::BottomRight:
                Transformable::setOrigin(m_bounds.width, m_bounds.height);
                break;
            case Origin::Bottom:
                Transformable::setOrigin(m_bounds.width / 2, m_bounds.height);
                break;
            case Origin::BottomLeft:
                Transformable::setOrigin(0, m_bounds.height);
                break;
            case Origin::Center:
                Transformable::setOrigin(m_bounds.width / 2, m_bounds.height / 2);
                break;
            default:
                break;
        }
    }

////////////////////////////////////////////////////////////
    Shape::Shape() :
            m_fillColor(255, 255, 255),
            m_outlineColor(255, 255, 255),
            m_outlineThickness(0),
            m_vertices(TriangleFan),
            m_outlineVertices(TriangleStrip),
            m_insideBounds(),
            m_bounds() {
        type = Type::Shape;
    }

////////////////////////////////////////////////////////////
    void Shape::update() {

        // Get the total number of points of the shape
        std::size_t count = getPointCount();
        if (count < 3) {
            m_vertices.resize(0);
            m_outlineVertices.resize(0);
            return;
        }

        m_vertices.resize(count + 2); // + 2 for center and repeated first point

        // Position
        for (std::size_t i = 0; i < count; ++i)
            m_vertices[i + 1].position = getPoint(i);
        m_vertices[count + 1].position = m_vertices[1].position;

        // Update the bounding rectangle
        m_vertices[0] = m_vertices[1]; // so that the result of getBounds() is correct
        m_insideBounds = m_vertices.getBounds();

        // Compute the center and make it the first vertex
        m_vertices[0].position.x = m_insideBounds.left + m_insideBounds.width / 2;
        m_vertices[0].position.y = m_insideBounds.top + m_insideBounds.height / 2;

        // Color
        updateFillColors();

        // Outline
        updateOutline();

        // origin
        setOrigin(m_origin);
    }

    void Shape::draw(Transform &transform) {

        Transform combined = transform * getTransform();
        if (getFillColor().a != 0) {
            c2d_renderer->draw(&m_vertices, combined, nullptr);
        }
        if (getOutlineColor().a != 0 && m_outlineThickness > 0) {
            c2d_renderer->draw(&m_outlineVertices, combined, nullptr);
        }

        C2DObject::draw(transform);
    }

////////////////////////////////////////////////////////////
    void Shape::updateFillColors() {
        for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
            m_vertices[i].color = m_fillColor;
        }
        m_vertices.updateVbo();
    }

////////////////////////////////////////////////////////////
    void Shape::updateOutline() {
        std::size_t count = m_vertices.getVertexCount() - 2;
        m_outlineVertices.resize((count + 1) * 2);

        for (std::size_t i = 0; i < count; ++i) {
            std::size_t index = i + 1;

            // Get the two segments shared by the current point
            Vector2f p0 = (i == 0) ? m_vertices[count].position : m_vertices[index - 1].position;
            Vector2f p1 = m_vertices[index].position;
            Vector2f p2 = m_vertices[index + 1].position;

            // Compute their normal
            Vector2f n1 = computeNormal(p0, p1);
            Vector2f n2 = computeNormal(p1, p2);

            // Make sure that the normals point towards the outside of the shape
            // (this depends on the order in which the points were defined)
            if (dotProduct(n1, m_vertices[0].position - p1) > 0)
                n1 = -n1;
            if (dotProduct(n2, m_vertices[0].position - p1) > 0)
                n2 = -n2;

            // Combine them to get the extrusion direction
            float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
            Vector2f normal = (n1 + n2) / factor;

            // Update the outline points
            m_outlineVertices[i * 2 + 0].position = p1;
            m_outlineVertices[i * 2 + 1].position = p1 + normal * m_outlineThickness;
        }

        // Duplicate the first point at the end, to close the outline
        m_outlineVertices[count * 2 + 0].position = m_outlineVertices[0].position;
        m_outlineVertices[count * 2 + 1].position = m_outlineVertices[1].position;

        // Update outline colors
        updateOutlineColors();

        // Update the shape's bounds
        m_bounds = m_outlineVertices.getBounds();

    }


////////////////////////////////////////////////////////////
    void Shape::updateOutlineColors() {
        for (std::size_t i = 0; i < m_outlineVertices.getVertexCount(); ++i)
            m_outlineVertices[i].color = m_outlineColor;
        m_outlineVertices.updateVbo();
    }

} // namespace sf
