//
// Created by cpasjuste on 05/01/18.
//

#ifndef CROSS2D_LISTBOX_H
#define CROSS2D_LISTBOX_H

#include "cross2d/c2d.h"

namespace c2d {

    class ListBoxLine : public RectangleShape {

    public:

        ListBoxLine(const FloatRect &rect, const std::string &str, Font *font,
                    unsigned int fontSize, Texture *icon = nullptr, bool use_icons = false);

        ~ListBoxLine();

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

        ~ListBox();

        void setFiles(const std::vector<Io::File> &fileList);

        void setSelection(int index);

        void setTextOutlineColor(const Color &color);

        void setTextOutlineThickness(float thickness);

        void setHighlight(bool enable);

        void setHighlightColor(const Color &color);

        void setHighlightOutlineColor(const Color &color);

        void setHighlightAlpha(uint8_t alpha);

        void setHighlightTween(Tween *tween);

        void setHighlightThickness(float thickness);

        void setHighlightUseFileColor(bool enable);

        int getMaxLines();

        std::vector<ListBoxLine *> getLines();

        std::vector<c2d::Io::File> getFiles();

        Io::File getSelection();

        int getIndex();

    private:

        std::vector<Io::File> files;
        std::vector<ListBoxLine *> lines;
        RectangleShape *highlight;
        float line_height;
        int max_lines;
        int index = 0;
        bool use_icons = false;
        bool highlight_use_files_color = true;
    };
}

#endif //CROSS2D_LISTBOX_H
