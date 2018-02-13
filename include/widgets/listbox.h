//
// Created by cpasjuste on 05/01/18.
//

#ifndef CROSS2D_LISTBOX_H
#define CROSS2D_LISTBOX_H

#include "c2d.h"

namespace c2d {

    class ListBox : public Rectangle {

    public:

        ListBox(const Font &font, int fontSize,
                const FloatRect &rect, const std::vector<Io::File *> &fileList);

        ~ListBox();

        void setSelection(int index);

        void setHighLight(bool enable);

        int getMaxLines();

        std::vector<c2d::Text *> getLines();

        std::vector<c2d::Io::File *> getFiles();

        Io::File *getSelection();

        int getIndex();

    private:

        std::vector<Io::File *> files;
        std::vector<c2d::Text *> lines;
        Rectangle *highlight;
        int line_height;
        int max_lines;
        int index = 0;
    };
}

#endif //CROSS2D_LISTBOX_H
