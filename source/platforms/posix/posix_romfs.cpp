//
// Created by cpasjuste on 23/02/2022.
//

#ifdef __FUZE_FS__

#include "cross2d/c2d.h"
#include "cross2d/platforms/posix/posix_romfs.h"

#define FUSE_USE_VERSION 27

#include <fuse.h>
#include <fuse_opt.h>
#include <fuse_lowlevel.h>
#include "../../skeleton/romfs/fuse-zip/lib/fuse-zip.h"
#include "../../skeleton/romfs/fuse-zip/lib/fuseZipData.h"

using namespace c2d;

extern "C" unsigned char c2d_romfs_zip[];
extern "C" int c2d_romfs_zip_len;
static struct fuse *fuse_session = nullptr;

static int fuseThread(void *ptr) {
    auto romFs = (POSIXRomFs *) ptr;
    char *fuse_mnt = nullptr;
    FuseZipData *data;

    struct fuse_args args = FUSE_ARGS_INIT(0, nullptr);
    fuse_opt_add_arg(&args, "c2d_romfs");
    fuse_opt_add_arg(&args, romFs->mountPoint.c_str()); // mount point
    fuse_opt_add_arg(&args, "-r");                      // read only
    fuse_opt_add_arg(&args, "-s");                      // libzip is not multi-threaded
    fuse_opt_add_arg(&args, "-f");                      // do not run in background

    if (fuse_opt_parse(&args, nullptr, nullptr, nullptr)) {
        printf("POSIXRomFs::POSIXRomFs: fuse_opt_parse failed\n");
        fuse_opt_free_args(&args);
        return EXIT_FAILURE;
    }

    if ((data = initFuseZip("fuseThread", c2d_romfs_zip, c2d_romfs_zip_len,
                            true, false)) == nullptr) {
        printf("POSIXRomFs::POSIXRomFs: initFuseZip failed\n");
        fuse_opt_free_args(&args);
        return EXIT_FAILURE;
    }

    struct fuse_operations op{};
    op.init = fusezip_init;
    op.destroy = fusezip_destroy;
    op.readdir = fusezip_readdir;
    op.getattr = fusezip_getattr;
    op.statfs = fusezip_statfs;
    op.open = fusezip_open;
    op.read = fusezip_read;
    op.write = fusezip_write;
    op.release = fusezip_release;
    op.unlink = fusezip_unlink;
    op.rmdir = fusezip_rmdir;
    op.mkdir = fusezip_mkdir;
    op.rename = fusezip_rename;
    op.create = fusezip_create;
    op.mknod = fusezip_mknod;
    op.chmod = fusezip_chmod;
    op.chown = fusezip_chown;
    op.flush = fusezip_flush;
    op.fsync = fusezip_fsync;
    op.fsyncdir = fusezip_fsyncdir;
    op.opendir = fusezip_opendir;
    op.releasedir = fusezip_releasedir;
    op.access = fusezip_access;
    op.utimens = fusezip_utimens;
    op.ftruncate = fusezip_ftruncate;
    op.truncate = fusezip_truncate;
    op.setxattr = fusezip_setxattr;
    op.getxattr = fusezip_getxattr;
    op.listxattr = fusezip_listxattr;
    op.removexattr = fusezip_removexattr;
    op.readlink = fusezip_readlink;
    op.symlink = fusezip_symlink;
#if FUSE_VERSION >= 28
    // don't allow NULL path
    op.flag_nullpath_ok = 0;
#if FUSE_VERSION == 29
    op.flag_utime_omit_ok = 1;
#endif
#endif

    int res;
    int threaded;
    fuse_session = fuse_setup(args.argc, args.argv, &op, sizeof(op),
                              &fuse_mnt, &threaded, data);
    fuse_opt_free_args(&args);
    if (fuse_session == nullptr) {
        printf("POSIXRomFs::POSIXRomFs: fuse_setup failed\n");
        delete (data);
        return EXIT_FAILURE;
    }

    printf("POSIXRomFs::POSIXRomFs: fuse mount point: %s\n", fuse_mnt);
    res = fuse_loop(fuse_session);

    delete (data);

    printf("POSIXRomFs::POSIXRomFs: exited fuse thread with status %i\n", res);
    return (res == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

POSIXRomFs::POSIXRomFs(const std::string &mnt) {
    mountPoint = mnt;
    thread = new C2DThread(fuseThread, this);
}

POSIXRomFs::~POSIXRomFs() {
    if (thread) {
        if (fuse_session) {
            fuse_exit(fuse_session);
            struct fuse_chan *ch = fuse_session_next_chan(fuse_get_session(fuse_session), nullptr);
            if (ch) {
                fuse_unmount(mountPoint.c_str(), ch);
            }
        }
        thread->join();
        delete (thread);
    }
}

#endif // __FUZE_FS__