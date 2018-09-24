//
// Created by cpasjuste on 05/01/18.
//

#ifndef CROSS2D_LISTBOX_H
#define CROSS2D_LISTBOX_H

#include "c2d.h"

namespace c2d {

    class ListBox : public Rectangle {

    public:

        class ListBoxLine : public Rectangle {

        public:

            ListBoxLine(const FloatRect &rect, const std::string &str, const Font &font,
                        unsigned int fontSize, Texture *icon = nullptr, bool use_icons = false);

            ~ListBoxLine();

            void setString(const std::string &string);

            void setColor(const Color &color);

            void setIcon(Texture *icon);

        private:

            Rectangle *iconRect = nullptr;
            Texture *icon = nullptr;
            Text *text = nullptr;
            bool use_icons = false;
        };

        ListBox(const Font &font, int fontSize, const FloatRect &rect,
                const std::vector<Io::File *> &fileList, bool use_icons = false);

        ~ListBox();

        void setFiles(const std::vector<Io::File *> &fileList);

        void setSelection(int index);

        void setHighLight(bool enable);

        Rectangle *getHighLight();

        int getMaxLines();

        std::vector<ListBoxLine *> getLines();

        std::vector<c2d::Io::File *> getFiles();

        Io::File *getSelection();

        int getIndex();

    private:

        std::vector<Io::File *> files;
        std::vector<ListBoxLine *> lines;
        Rectangle *highlight;
        float line_height;
        int max_lines;
        int index = 0;
        bool use_icons = false;
    };
}

#endif //CROSS2D_LISTBOX_H
