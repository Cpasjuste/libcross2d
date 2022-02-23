//
// Created by cpasjuste on 23/02/2022.
//

#ifndef C2D_POSIX_ROMFS_H
#define C2D_POSIX_ROMFS_H

#ifdef __FUZE_FS__

namespace c2d {
    class POSIXRomFs {
    public:
        explicit POSIXRomFs(const std::string &mountPoint);

        ~POSIXRomFs();

        std::string mountPoint;

        bool inited = false;

    private:
        C2DThread *thread = nullptr;
    };
}

#endif // __FUZE_FS__
#endif //C2D_POSIX_ROMFS_H
