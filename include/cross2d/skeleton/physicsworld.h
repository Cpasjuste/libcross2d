//
// Created by cpasjuste on 25/05/2021.
//

#ifndef SILLYTOWER_PHYSICSWORLD_H
#define SILLYTOWER_PHYSICSWORLD_H

#ifdef __BOX2D__

#ifdef __PSP2__
#include <box2d/box2d.h>
#else

#include <Box2D/Box2D.h>

#endif

#include "cross2d/skeleton/sfml/Rectangle.hpp"

class PhysicsWorld : public c2d::Rectangle {

public:
    explicit PhysicsWorld(const c2d::Vector2f &gravity = {0, -10},
                          const c2d::Vector2i &iterations = {6, 2}, float m_pixelsPerMeter = 32);

    ~PhysicsWorld() override;

    b2World *getPhysics() {
        return m_world;
    }

    void setPaused(bool paused) {
        physicsPaused = paused;
    }

    void setPixelsPerMeter(float ppm) {
        m_pixelsPerMeter = ppm;
    }

    float getPixelsPerMeter() {
        return m_pixelsPerMeter;
    }

protected:
    void onUpdate() override;

private:
    b2World *m_world = nullptr;
    c2d::Vector2i m_iterations = {6, 2};
    float m_pixelsPerMeter = 32;
    bool physicsPaused = false;
};

#endif //__BOX2D__

#endif //SILLYTOWER_PHYSICSWORLD_H
