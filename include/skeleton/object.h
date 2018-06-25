//
// Created by cpasjuste on 12/12/17.
//

#ifndef CROSS2D_DRAWABLE_H
#define CROSS2D_DRAWABLE_H

#include <cstdint>
#include <vector>

#include "skeleton/sfml/RectangleShape.hpp"

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

        C2DObject();

        virtual ~C2DObject();

        virtual void draw(Transform &transform);

        // Childs
        void add(C2DObject *object);

        // remove a widget without calling
        // it's destructor
        void removeChild(C2DObject *object);

        // visibility
        int getVisibility();

        void setVisibility(Visibility visibility);

        // deletion mode
        int getDeleteMode();

        void setDeleteMode(DeleteMode mode);

        int getLayer();

        void setLayer(int layer);

        bool available = false;

    protected:

        C2DObject *parent = NULL;
        Transformable *thisTransform = NULL;
        std::vector<C2DObject *> childs;
        Visibility visibility = Visible;
        DeleteMode deleteMode = Auto;
        int layer = 0;
    };
}

#endif //CROSS2D_DRAWABLE_H
