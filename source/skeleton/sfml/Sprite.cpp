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
        setTexture(texture, true);
    }


////////////////////////////////////////////////////////////
    Sprite::Sprite(Texture *texture, const IntRect &rectangle) :
            m_vertices(TriangleStrip, 4),
            m_texture(nullptr) {
        type = Type::Sprite;
        m_textureRect = rectangle;
        setTexture(texture);
    }


////////////////////////////////////////////////////////////
    void Sprite::setTexture(Texture *texture, bool resetRect) {

        m_texture = texture;
        if (texture == nullptr) {
            m_size = {0, 0};
            setTextureRect({0, 0, 0, 0});
        } else {
            // Recompute the texture area if requested, or if there was no valid texture & rect before
            if (resetRect || m_textureRect == IntRect()) {
                m_size = {(int) texture->getTextureRect().width, (int) texture->getTextureRect().height};
                setTextureRect(IntRect(0, 0,
                                       (int) texture->getTextureRect().width, (int) texture->getTextureRect().height));
            } else if (m_textureRect != IntRect()) {
                m_size = {(float) m_textureRect.width, (float) m_textureRect.height};
                updatePositions();
                updateTexCoords();
            }
        }
    }


////////////////////////////////////////////////////////////
    void Sprite::setTextureRect(const IntRect &rectangle) {
        if (rectangle != m_textureRect) {
            m_size = {rectangle.width, rectangle.height};
            m_textureRect = rectangle;
            if (m_texture != nullptr) {
                m_texture->setTextureRect(rectangle);
            }
            updatePositions();
            updateTexCoords();
        }
    }


////////////////////////////////////////////////////////////
    void Sprite::setFillColor(const Color &color) {
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
    const Color &Sprite::getFillColor() const {
        return m_vertices[0].color;
    }


////////////////////////////////////////////////////////////
    FloatRect Sprite::getLocalBounds() const {
        return {0.f, 0.f, m_size.x, m_size.y};
    }


////////////////////////////////////////////////////////////

    const c2d::Vector2f &Sprite::getSize() const {
        return m_size;
    }

    void Sprite::setSize(const c2d::Vector2f &size) {
        m_size = size;
        updatePositions();
    }

    void Sprite::setSize(float width, float height) {
        m_size.x = width;
        m_size.y = height;
        updatePositions();
    }

    FloatRect Sprite::getGlobalBounds() const {
        Transform t = transformation * getTransform();
        return t.transformRect(getLocalBounds());
    }


    VertexArray *Sprite::getVertexArray() {
        return &m_vertices;
    }

    void Sprite::setOrigin(const Origin &origin) {
#ifdef __BOX2D__ // TODO: fixme
        if (m_body) {
            return;
        }
#endif
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

    ////////////////////////////////////////////////////////////
    void Sprite::onDraw(Transform &transform, bool draw) {

#ifdef __BOX2D__
        if (m_body) {
            b2Vec2 pos = m_body->GetPosition();
            setPosition(pos.x * c2d_renderer->getPixelsPerMeter(), pos.y * c2d_renderer->getPixelsPerMeter());
            float32 angle = m_body->GetAngle();
            setRotation(angle * 180 / M_PI);
        }
#endif
        if (draw) {
            Transform combined = transform * getTransform();
            c2d_renderer->draw(&m_vertices, combined, nullptr, this);
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
        m_vertices.update();
    }


////////////////////////////////////////////////////////////
    void Sprite::updateTexCoords() {

        if (m_texture == nullptr) {
            return;
        }

        setOrigin(m_sprite_origin);

        auto left = (float) m_textureRect.left;
        float right = left + m_textureRect.width;
        auto top = (float) m_textureRect.top;
        float bottom = top + m_textureRect.height;
        Vector2f texSize = m_texture->getSize();

        m_vertices[0].texCoords = Vector2f(left / texSize.x, top / texSize.y);
        m_vertices[1].texCoords = Vector2f(left / texSize.x, bottom / texSize.y);
        m_vertices[2].texCoords = Vector2f(right / texSize.x, top / texSize.y);
        m_vertices[3].texCoords = Vector2f(right / texSize.x, bottom / texSize.y);
        m_vertices.update();
    }

#ifdef __BOX2D__

    b2Body *Sprite::addPhysicsBody(b2BodyType type, float density, float friction) {
        if (!m_body) {

            m_bodyDef.type = type;
            m_bodyDef.position.Set(getPosition().x / c2d_renderer->getPixelsPerMeter(),
                                   getPosition().y / c2d_renderer->getPixelsPerMeter());
            m_body = c2d_renderer->getPhysicsWorld()->CreateBody(&m_bodyDef);

            Vector2f boxSize = {(getSize().x / c2d_renderer->getPixelsPerMeter()) / 2,
                                (getSize().y / c2d_renderer->getPixelsPerMeter()) / 2};

            m_polyShape.m_type = b2Shape::Type::e_polygon;
            m_polyShape.m_centroid = {boxSize.x, boxSize.y};

            auto vs = (b2Vec2 *) malloc(sizeof(b2Vec2) * 4);
            vs[0].Set(0, 0);
            vs[1].Set(m_size.x / c2d_renderer->getPixelsPerMeter(), 0);
            vs[2].Set(m_size.x / c2d_renderer->getPixelsPerMeter(), m_size.y / c2d_renderer->getPixelsPerMeter());
            vs[3].Set(0, m_size.y / c2d_renderer->getPixelsPerMeter());
            m_polyShape.Set(vs, 4);
            free(vs);

            m_fixtureDef.shape = &m_polyShape;
            m_fixtureDef.density = density;
            m_fixtureDef.friction = friction;
            m_fixture = m_body->CreateFixture(&m_fixtureDef);
        }

        return m_body;
    }

    b2Body *Sprite::getPhysicsBody() {
        return m_body;
    }

#endif

} // namespace c2d
