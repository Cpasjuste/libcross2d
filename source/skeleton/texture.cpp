//
// Created by cpasjuste on 29/11/17.
//

#include "c2d.h"

using namespace c2d;

Texture::Texture(const char *p) : Transformable()/*: RectangleShape(Vector2f(0, 0))*/ {

    printf("Texture(%p): %s\n", this, p);

    format = C2D_TEXTURE_FMT_RGBA8;
    bpp = 4;

    m_vertices.setPrimitiveType(TriangleStrip);
    m_vertices.resize(4);
    setColor(Color::White);
    setFiltering(C2D_TEXTURE_FILTER_LINEAR);

    strncpy(path, p, 511);

    thisTransform = this;
}

Texture::Texture(const unsigned char *buffer, int bufferSize) : Transformable()/*: RectangleShape(Vector2f(0, 0))*/ {

    printf("Texture(%p)\n", this);

    format = C2D_TEXTURE_FMT_RGBA8;
    bpp = 4;

    m_vertices.setPrimitiveType(TriangleStrip);
    m_vertices.resize(4);
    setColor(Color::White);
    setFiltering(C2D_TEXTURE_FILTER_LINEAR);

    thisTransform = this;
}

Texture::Texture(const Vector2f &size, int fmt) : Transformable()/*: RectangleShape(size)*/ {

    printf("Texture(%p)\n", this);

    format = fmt;
    bpp = (fmt == C2D_TEXTURE_FMT_RGBA8 || fmt == C2D_TEXTURE_FMT_ARGB8) ? 4 : 2;
    pitch = (int) (size.x * bpp);

    m_vertices.setPrimitiveType(TriangleStrip);
    m_vertices.resize(4);
    setColor(Color::White);
    setFiltering(C2D_TEXTURE_FILTER_LINEAR);

    thisTransform = this;
}

void Texture::setTextureRect(const IntRect &rectangle) {
    if (rectangle != m_textureRect) {
        m_textureRect = rectangle;
        updatePositions();
        updateTexCoords();
    }
}


void Texture::setColor(const Color &color) {
    m_vertices[0].color = color;
    m_vertices[1].color = color;
    m_vertices[2].color = color;
    m_vertices[3].color = color;
}

const IntRect &Texture::getTextureRect() const {
    return m_textureRect;
}


const Color &Texture::getColor() const {
    return m_vertices[0].color;
}

FloatRect Texture::getLocalBounds() const {
    float width = static_cast<float>(std::abs(m_textureRect.width));
    float height = static_cast<float>(std::abs(m_textureRect.height));

    return FloatRect(0.f, 0.f, width, height);
}

FloatRect Texture::getGlobalBounds() const {
    return getTransform().transformRect(getLocalBounds());
}

VertexArray Texture::getVertices() const {
    return m_vertices;
}

void Texture::updatePositions() {
    FloatRect bounds = getLocalBounds();

    m_vertices[0].position = Vector2f(0, 0);
    m_vertices[1].position = Vector2f(0, bounds.height);
    m_vertices[2].position = Vector2f(bounds.width, 0);
    m_vertices[3].position = Vector2f(bounds.width, bounds.height);
}


////////////////////////////////////////////////////////////
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

    // draw texture from renderer
    c2d_renderer->drawTexture(*this, transform);

    // call base class (draw childs)
    C2DObject::draw(transform);
}

Texture::~Texture() {
    printf("~Texture(%p)\n", this);
}
