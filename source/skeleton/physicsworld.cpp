//
// Created by cpasjuste on 25/05/2021.
//

#include "cross2d/c2d.h"
#include "cross2d/skeleton/physicsworld.h"

using namespace c2d;

PhysicsWorld::PhysicsWorld(const c2d::Vector2f &gravity, const c2d::Vector2i &iterations, float pixelsPerMeter)
        : Rectangle({1000, 1000}) {

    Rectangle::setOrigin(Origin::BottomLeft);
    Rectangle::setScale(1.0f, -1.0f);

    m_world = new b2World({gravity.x, gravity.y});
    m_iterations = iterations;
    m_pixelsPerMeter = pixelsPerMeter;
}

void PhysicsWorld::onUpdate() {

    if (m_world && !physicsPaused) {
        m_world->Step(c2d_renderer->getDeltaTime().asSeconds(), m_iterations.x, m_iterations.y);
    }

    Rectangle::onUpdate();
}

PhysicsWorld::~PhysicsWorld() {
    delete (m_world);
}
