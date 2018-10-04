//
// Created by cpasjuste on 29/11/17.
//

#include "c2d.h"

using namespace c2d;

Texture::Texture(const char *p) : Transformable() {

    printf("Texture(%p): %s\n", this, p);

    format = Format::RGBA8;
    bpp = 4;

    m_vertices.setPrimitiveType(TriangleStrip);
    m_vertices.resize(4);
    setColor(Color::White);

    strncpy(path, p, 511);

    type = Type::Texture;
}

Texture::Texture(const unsigned char *buffer, int bufferSize) : Transformable() {

    printf("Texture(%p)\n", this);

    format = Format::RGBA8;
    bpp = 4;

    m_vertices.setPrimitiveType(TriangleStrip);
    m_vertices.resize(4);
    setColor(Color::White);

    type = Type::Texture;
}

Texture::Texture(const Vector2f &size, Format fmt) : Transformable() {

    printf("Texture(%p)\n", this);

    format = fmt;
    bpp = fmt == Format::RGB565 ? 2 : 4;
    pitch = (int) (size.x * bpp);

    m_vertices.setPrimitiveType(TriangleStrip);
    m_vertices.resize(4);
    setColor(Color::White);

    type = Type::Texture;
}

Texture::~Texture() {
    printf("~Texture(%p)\n", this);
}

void Texture::setTextureRect(const IntRect &rectangle) {
    if (rectangle != m_textureRect) {
        m_textureRect = rectangle;
        updatePositions();
        updateTexCoords();
        m_vertices.updateVbo();
    }
}

void Texture::setOrigin(float x, float y) {
    Transformable::setOrigin(x, y);
}

void Texture::setOrigin(const Vector2f &origin) {
    Transformable::setOrigin(origin);
}

void Texture::setOrigin(const Origin &origin) {

    m_origin = origin;
    FloatRect m_bounds = getLocalBounds();

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

void Texture::setColor(const Color &color) {
    m_vertices[0].color = color;
    m_vertices[1].color = color;
    m_vertices[2].color = color;
    m_vertices[3].color = color;
    m_vertices.updateVbo();
}

const IntRect &Texture::getTextureRect() const {
    return m_textureRect;
}


const Color &Texture::getColor() const {
    return m_vertices[0].color;
}

FloatRect Texture::getLocalBounds() const {
    float width = (float) std::abs(m_textureRect.width);
    float height = (float) std::abs(m_textureRect.height);

    return {0.f, 0.f, width, height};
}

FloatRect Texture::getGlobalBounds() const {
    return getTransform().transformRect(getLocalBounds());
}

VertexArray *Texture::getVertices() {
    return &m_vertices;
}

////////////////////////////////////////////////////////////
//// PRIVATE
////////////////////////////////////////////////////////////

void Texture::updatePositions() {
    FloatRect bounds = getLocalBounds();

    m_vertices[0].position = Vector2f(0, 0);
    m_vertices[1].position = Vector2f(0, bounds.height);
    m_vertices[2].position = Vector2f(bounds.width, 0);
    m_vertices[3].position = Vector2f(bounds.width, bounds.height);

    // origin
    setOrigin(m_origin);
}

void Texture::updateTexCoords() {
    float left = static_cast<float>(m_textureRect.left);
    float right = left + m_textureRect.width;
    float top = static_cast<float>(m_textureRect.top);
    float bottom = top + m_textureRect.height;

    m_vertices[0].texCoords = Vector2f(left, top);
    m_vertices[1].texCoords = Vector2f(left, bottom);
    m_vertices[2].texCoords = Vector2f(right, top);
    m_vertices[3].texCoords = Vector2f(right, bottom);
}

void Texture::draw(Transform &transform) {

    Transform combined = transform * getTransform();

    // TODO
    //if (texture.getOutlineColor().a != 0 && texture.getOutlineThickness() > 0) {
    //    draw(texture.getOutlineVertices(), combined, nullptr);
    //}
    if (available && getColor().a != 0) {
        c2d_renderer->draw(getVertices(), combined, this);
    }

    // call base class (draw childs)
    C2DObject::draw(transform);
}
