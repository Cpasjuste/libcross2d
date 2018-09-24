//
// Created by cpasjuste on 12/12/17.
//

#ifndef CROSS2D_DRAWABLE_H
#define CROSS2D_DRAWABLE_H

#include <cstdint>
#include <vector>

#include "skeleton/sfml/Transformable.hpp"
#include "tweener.h"

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
        void add(Tweener *tweener);

        void remove(Tweener *tweener);

        // visibility
        int getVisibility();

        void setVisibility(Visibility visibility);

        // deletion mode
        int getDeleteMode();

        void setDeleteMode(DeleteMode mode);

        int getLayer();

        void setLayer(int layer);

        const ObjectType getType() const {
            return type;
        }

        bool available = false;

    protected:

        C2DObject *parent = nullptr;
        ObjectType type;
        //void *thisTransform = nullptr;

        std::vector<C2DObject *> childs;
        std::vector<Tweener *> tweeners;
        Visibility visibility = Visible;
        DeleteMode deleteMode = Auto;
        int layer = 0;
    };
}

#endif //CROSS2D_DRAWABLE_H
