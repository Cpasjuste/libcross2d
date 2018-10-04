//
// Created by cpasjuste on 12/12/17.
//

#ifndef CROSS2D_DRAWABLE_H
#define CROSS2D_DRAWABLE_H

#include <cstdint>
#include <vector>

#include "skeleton/sfml/Transformable.hpp"
#include "tween.h"

namespace c2d {

    class Renderer;

    class C2DObject {

    public:

        enum Visibility : int {
            Visible = 0, Hidden = 1
        };

        enum DeleteMode : int {
            Auto = 0, Manual = 1
        };

        enum ObjectType : int {
            TRectangle = 1,
            TLine = 2,
            TCircle = 3,
            TTexture = 4,
            TText = 5
        };

        C2DObject();

        virtual ~C2DObject();

        virtual void draw(Transform &transform);

        // Childs
        void add(C2DObject *object);

        // remove a widget without calling it's destructor
        void remove(C2DObject *object);

        // tweeners
        void add(Tween *tweener);

        void remove(Tween *tweener);

        // visibility
        int getVisibility();

        void setVisibility(Visibility visibility, bool tweenPlay = true);

        // deletion mode
        int getDeleteMode();

        void setDeleteMode(DeleteMode mode);

        int getLayer();

        void setLayer(int layer);

        Transformable *getTransformable();

        ObjectType getType() const {
            return type;
        }

        bool available = false;

    protected:

        C2DObject *parent = nullptr;
        Transformable *transformable = nullptr;
        ObjectType type;
        std::vector<C2DObject *> childs;
        std::vector<Tween *> tweeners;
        DeleteMode deleteMode = Auto;
        int layer = 0;

    private:

        Visibility visibility_current = Visible;
        // for tweeners
        Visibility visibility_wanted = Visible;
    };
}

#endif //CROSS2D_DRAWABLE_H
