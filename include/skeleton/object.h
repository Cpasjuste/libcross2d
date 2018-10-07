//
// Created by cpasjuste on 12/12/17.
//

#ifndef CROSS2D_OBJECT_H
#define CROSS2D_OBJECT_H

#include <cstdint>
#include <vector>
#include "skeleton/sfml/Transform.hpp"
#include "tween.h"

namespace c2d {

    enum class Origin : int {
        Left, TopLeft, Top, TopRight,
        Right, BottomRight, Bottom, BottomLeft, Center
    };

    enum class Visibility : int {
        Visible, Hidden
    };

    enum class DeleteMode : int {
        Auto, Manual
    };

    enum class Type : int {
        Shape, Texture, Text
    };

    class C2DObject {

    public:

        virtual ~C2DObject();

        // childs
        virtual void add(C2DObject *object);

        // remove a widget without calling it's destructor
        virtual void remove(C2DObject *object);

        // tweeners
        virtual void add(Tween *tween);

        // remove a tweener without calling it's destructor
        virtual void remove(Tween *tween);

        // visibility
        virtual bool isVisible();

        virtual Visibility getVisibility();

        virtual void setVisibility(Visibility visibility, bool tweenPlay = true);

        // deletion mode
        virtual DeleteMode getDeleteMode();

        virtual void setDeleteMode(DeleteMode mode);

        virtual int getLayer();

        virtual void setLayer(int layer);

        Type getType() const {
            return type;
        }

        bool available = false;

    protected:

        virtual void draw(Transform &transform);

        virtual void setAlpha(uint8_t alpha);

        Type type;
        Transform transformation;

    private:

        int layer = 0;
        C2DObject *parent = nullptr;
        std::vector<Tween *> tweens;
        std::vector<C2DObject *> childs;
        DeleteMode deleteMode = DeleteMode::Auto;
        Visibility visibility_current = Visibility::Visible;
        Visibility visibility_wanted = Visibility::Visible;
    };
}

#endif //CROSS2D_OBJECT_H
