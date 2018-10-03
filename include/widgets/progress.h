//
// Created by cpasjuste on 03/10/18.
//

#ifndef C2D_PROGRESS_H
#define C2D_PROGRESS_H

namespace c2d {

    class Progress : public c2d::Rectangle {

    public:

        Progress(const c2d::FloatRect &rect);

        void setProgress(float progress);

        void setBgColor(const c2d::Color &color);

        void setFgColor(const c2d::Color &color);

    private:

        c2d::Rectangle *progress_fg = nullptr;

    };
}

#endif //C2D_PROGRESS_H
