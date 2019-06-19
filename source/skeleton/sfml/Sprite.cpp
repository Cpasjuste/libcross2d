////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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
#include <cstdlib>

#include "cross2d/c2d.h"

namespace c2d {

////////////////////////////////////////////////////////////
    Sprite::Sprite() :
            m_vertices(TriangleStrip, 4),
            m_texture(nullptr),
            m_textureRect() {
        type = Type::Sprite;
    }


////////////////////////////////////////////////////////////
    Sprite::Sprite(Texture *texture) :
            m_vertices(TriangleStrip, 4),
            m_texture(nullptr),
            m_textureRect() {
        type = Type::Sprite;
        setTexture(texture);
    }


////////////////////////////////////////////////////////////
    Sprite::Sprite(Texture *texture, const IntRect &rectangle) :
            m_vertices(TriangleStrip, 4),
            m_texture(nullptr),
            m_textureRect() {
        type = Type::Sprite;
        setTexture(texture);
        setTextureRect(rectangle);
    }


////////////////////////////////////////////////////////////
    void Sprite::setTexture(Texture *texture, bool resetRect) {

        m_texture = texture;

        if (!texture) {
            setTextureRect({0, 0, 0, 0});
        } else {
            // Recompute the texture area if requested, or if there was no valid texture & rect before
            if (resetRect || (!m_texture && (m_textureRect == IntRect()))) {
                setTextureRect(IntRect(0, 0, (int) texture->getSize().x, (int) texture->getSize().y));
            }
        }
    }


////////////////////////////////////////////////////////////
    void Sprite::setTextureRect(const IntRect &rectangle) {
        if (rectangle != m_textureRect) {
            m_textureRect = rectangle;
            //if (m_texture) {
            //    m_texture->setTextureRect(rectangle);
            //}
            updatePositions();
            updateTexCoords();
            m_vertices.update();
        }
    }


////////////////////////////////////////////////////////////
    void Sprite::setColor(const Color &color) {
        // Update the vertices' color
        m_vertices[0].color = color;
        m_vertices[1].color = color;
        m_vertices[2].color = color;
        m_vertices[3].color = color;
        m_vertices.update();
    }


////////////////////////////////////////////////////////////
    Texture *Sprite::getTexture() {
        return m_texture;
    }


////////////////////////////////////////////////////////////
    const IntRect &Sprite::getTextureRect() const {
        return m_textureRect;
    }


////////////////////////////////////////////////////////////
    const Color &Sprite::getColor() const {
        return m_vertices[0].color;
    }


////////////////////////////////////////////////////////////
    FloatRect Sprite::getLocalBounds() const {
        auto width = (float) std::abs(m_textureRect.width);
        auto height = (float) std::abs(m_textureRect.height);
        return {0.f, 0.f, width, height};
    }


////////////////////////////////////////////////////////////
    FloatRect Sprite::getGlobalBounds() const {
        Transform t = transformation * getTransform();
        return t.transformRect(getLocalBounds());
    }


    VertexArray *Sprite::getVertexArray() {
        return &m_vertices;
    }

    void Sprite::setOrigin(const Origin &origin) {

        FloatRect m_bounds = getLocalBounds();
        m_sprite_origin = origin;

        switch (origin) {
            case Origin::Left:
                Transformable::setOriginVector(0, m_bounds.height / 2);
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
                Transformable::setOriginVector(m_bounds.width, m_bounds.height / 2);
                break;
            case Origin::BottomRight:
                Transformable::setOriginVector(m_bounds.width, m_bounds.height);
                break;
            case Origin::Bottom:
                Transformable::setOriginVector(m_bounds.width / 2, m_bounds.height);
                break;
            case Origin::BottomLeft:
                Transformable::setOriginVector(0, m_bounds.height);
                break;
            case Origin::Center:
                Transformable::setOriginVector(m_bounds.width / 2, m_bounds.height / 2);
                break;
            default:
                break;
        }
    }

    Origin Sprite::getOrigin() const {
        return m_sprite_origin;
    }

    void Sprite::setAlpha(uint8_t alpha, bool recursive) {

        if (alpha != m_vertices[0].color.a) {
            m_vertices[0].color.a = alpha;
            m_vertices[1].color.a = alpha;
            m_vertices[2].color.a = alpha;
            m_vertices[3].color.a = alpha;
            m_vertices.update();
        }

        if (recursive) {
            C2DObject::setAlpha(alpha, recursive);
        }
    }

    uint8_t Sprite::getAlpha() {
        return m_vertices[0].color.a;
    }

    void Sprite::setScale(float factorX, float factorY) {
        if (m_texture) {
            m_texture->setScale(factorX, factorY);
        }
        Transformable::setScale(factorX, factorY);
    }

    ////////////////////////////////////////////////////////////
    void Sprite::onDraw(Transform &transform, bool draw) {
        if (draw) {
            Transform combined = transform * getTransform();
            c2d_renderer->draw(&m_vertices, combined, m_texture);
        }
        C2DObject::onDraw(transform, draw);
    }

////////////////////////////////////////////////////////////
    void Sprite::updatePositions() {

        setOrigin(m_sprite_origin);

        FloatRect bounds = getLocalBounds();

        m_vertices[0].position = Vector2f(0, 0);
        m_vertices[1].position = Vector2f(0, bounds.height);
        m_vertices[2].position = Vector2f(bounds.width, 0);
        m_vertices[3].position = Vector2f(bounds.width, bounds.height);
    }


////////////////////////////////////////////////////////////
    void Sprite::updateTexCoords() {

        setOrigin(m_sprite_origin);

        auto left = (float) m_textureRect.left;
        float right = left + m_textureRect.width;
        auto top = (float) m_textureRect.top;
        float bottom = top + m_textureRect.height;

        m_vertices[0].texCoords = Vector2f(left, top);
        m_vertices[1].texCoords = Vector2f(left, bottom);
        m_vertices[2].texCoords = Vector2f(right, top);
        m_vertices[3].texCoords = Vector2f(right, bottom);
    }

} // namespace c2d
