//
// Created by cpasjuste on 05/01/18.
//

#ifndef CROSS2D_LISTBOX_H
#define CROSS2D_LISTBOX_H

#include "cross2d/skeleton/sfml/RectangleShape.hpp"
#include "cross2d/skeleton/sfml/Text.hpp"
#include "cross2d/skeleton/io.h"

namespace c2d {

    class ListBoxLine : public RectangleShape {

    public:

        ListBoxLine(const FloatRect &rect, const std::string &str, Font *font,
                    unsigned int fontSize, Texture *icon = nullptr, bool use_icons = false);

        ~ListBoxLine();

        void setSize(const Vector2f &size) override;

        void setSize(float width, float height) override;

        void setString(const std::string &string);

        void setColor(const Color &color);

        void setIcon(Texture *icon);

        Text *getText();

    private:

        RectangleShape *iconRect = nullptr;
        Texture *icon = nullptr;
        Text *text = nullptr;
        bool use_icons = false;
    };

    class ListBox : public RectangleShape {

    public:

        ListBox(Font *font, int fontSize, const FloatRect &rect,
                const std::vector<Io::File> &fileList, bool useIcons = false);

        ListBox(Font *font, int fontSize, const FloatRect &rect,
                const std::vector<Io::File *> &fileList, bool useIcons = false);

        ~ListBox();

        void setFiles(const std::vector<Io::File> &fileList);

        void setFiles(const std::vector<Io::File *> fileList);

        void setSelection(int index);

        void setSize(const Vector2f &size);

        void setSize(float width, float height);

        void setTextOutlineColor(const Color &color);

        void setTextOutlineThickness(float thickness);

        void setHighlightEnabled(bool enable);

        void setHighlightUseFileColor(bool enable);

        RectangleShape *getHighlight();

        int getMaxLines();

        std::vector<ListBoxLine *> getLines();

        std::vector<c2d::Io::File *> getFiles();

        Io::File *getSelection();

        int getIndex();

    private:

        void init(Font *font, int fontSize, bool useIcons);

        std::vector<Io::File *> files;
        std::vector<ListBoxLine *> lines;
        RectangleShape *highlight;
        float line_height;
        int max_lines;
        int index = 0;
        bool use_icons = false;
        bool use_highlight = true;
        bool highlight_use_files_color = false;
        bool files_are_mine = false;
    };
}

#endif //CROSS2D_LISTBOX_H
