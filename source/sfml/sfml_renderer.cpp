//
// Created by cpasjuste on 21/11/16.
//

#include <GL/gl.h>
#include "c2d.h"

using namespace c2d;

SFMLRenderer::SFMLRenderer(const Vector2f &size, const std::string &shaderPath) : Renderer(size) {

    sf::ContextSettings settings(16, 0, 0);
    mode = sf::VideoMode::getDesktopMode();
    sf::Uint32 style = sf::Style::Fullscreen;

    // windowed
    if (getSize().x != 0 && getSize().y != 0) {
        mode.width = (unsigned int) getSize().x;
        mode.height = (unsigned int) getSize().y;
        style = sf::Style::Default;
    }

    window.create(mode, "SFMLRenderer", style, settings);
    window.setVerticalSyncEnabled(true);

    printf("SFMLRenderer: %i x %i @ %i bpp\n", mode.width, mode.height, mode.bitsPerPixel);
    const unsigned char *glversion = glGetString(GL_VERSION);
    printf("SFMLRenderer: glversion: %s\n", glversion);
    const unsigned char *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("SFMLRenderer: glslversion: %s\n", glslversion);

    if (shaders) {
        delete (shaders);
    }
    shaders = (Shaders * )
    new SFMLShaders(shaderPath);
}

void SFMLRenderer::setShader(int index) {

    if (index == shaders->current || index >= shaders->Count()) {
        return;
    }
    shaders->current = index;
}

void SFMLRenderer::draw(const VertexArray &vertices,
                        const Transform &transform,
                        const Texture *texture) {

    sf::RenderStates states;
    states.transform = ((sf::Transform &) transform);
    if (texture) {
        states.texture = &((SFMLTexture *) texture)->texture;

        sf::Shader *shader = (sf::Shader *) shaders->Get()->data;
        if (shader) {
            shader->setUniform("Texture", ((SFMLTexture *) texture)->texture);
            shader->setUniform("MVPMatrix", sf::Glsl::Mat4(
                    window.getView().getTransform().getMatrix()));
            shader->setUniform("TextureSize", sf::Glsl::Vec2(
                    texture->getTextureRect().width,
                    texture->getTextureRect().height));
            shader->setUniform("InputSize", sf::Glsl::Vec2(
                    texture->getTextureRect().width,
                    texture->getTextureRect().height));
            shader->setUniform("OutputSize", sf::Glsl::Vec2(
                    texture->getSize().x,
                    texture->getSize().y));
            states.shader = shader;
        }

    }

    window.draw((sf::Vertex *) vertices.getVertices().data(), vertices.getVertexCount(),
                (sf::PrimitiveType) vertices.getPrimitiveType(), states);

}

void SFMLRenderer::flip() {

    // clear screen
    window.clear((const sf::Color &) getFillColor());

    // call base class (draw childs)
    Renderer::flip();

    // flip
    window.display();
}

void SFMLRenderer::delay(unsigned int ms) {

    sf::sleep(sf::milliseconds(ms));
}

SFMLRenderer::~SFMLRenderer() {

    delete (shaders);
    window.close();
}
