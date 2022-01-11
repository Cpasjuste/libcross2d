#include <string>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <physfs.h>
#include <climits>
#include <funchook.h>

#ifdef NDEBUG
#pragma GCC diagnostic ignored "-Wunused-value"
#define printf(fmt, ...) (0)
#endif

extern "C" const char c2d_romfs[];
extern "C" const int c2d_romfs_length;

struct PhysPtr {
    std::string path;
    PHYSFS_File *file = nullptr;
    int fd = 0;
    size_t size = 0;
};

static std::vector<PhysPtr> physPtrList;
static bool initialised = false;
static funchook_t *funchook;
static int fdcount = INT_MAX;

static PhysPtr *physGet(int fd) {

    if (physPtrList.empty()) {
        return nullptr;
    }

    for (auto &ptr: physPtrList) {
        if (ptr.fd == fd) {
            return &ptr;
        }
    }

    return nullptr;
}

static int physDel(int fd) {

    if (physPtrList.empty()) {
        return -1;
    }

    for (size_t i = 0; i < physPtrList.size(); i++) {
        if (physPtrList.at(i).fd == fd) {
            physPtrList.erase(physPtrList.begin() + i);
        }
    }

    return 0;
}

static bool isRomFs(const char *pathname) {
    return pathname && strlen(pathname) > 6 && strncmp(pathname, "romfs:", 6) == 0;
}

static int (*open_func)(const char *pathname, int flags, mode_t mode);

static int open_hook(const char *pathname, int flags, mode_t mode) {

    if (isRomFs(pathname)) {
        std::string path = std::string(pathname).replace(0, 6, "/romfs");
        PHYSFS_File *file;
        if (flags & O_WRONLY) {
            file = PHYSFS_openWrite(path.c_str());
        } else if (flags & O_APPEND) {
            file = PHYSFS_openAppend(path.c_str());
        } else {
            file = PHYSFS_openRead(path.c_str());
        }

        if (file) {
            fdcount--;
            int size = PHYSFS_fileLength(file);
            physPtrList.push_back({pathname, file, fdcount, size});
            printf("OK: open_hook(%i, %s, %i, %i)\n", fdcount, path.c_str(), flags, mode);
            return fdcount;
        } else {
            printf("NOK: open_hook(%s, %i, %i)\n", path.c_str(), flags, mode);
            return -1;
        }
    }

    return open_func(pathname, flags, mode);
}

static ssize_t (*read_func)(int fd, void *buf, size_t count);

static ssize_t read_hook(int fd, void *buf, size_t count) {

    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        ssize_t len = buf ? PHYSFS_readBytes(fsPtr->file, buf, count) : 0;
        printf("OK: read_hook(%i, %p, %zu), read = %zu\n", fd, buf, count, len);
        return len;
    }

    return read_func(fd, buf, count);
}

static off_t (*lseek_func)(int fd, off_t offset, int whence);

static off_t lseek_hook(int fd, off_t offset, int whence) {

    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        off_t pos = PHYSFS_tell(fsPtr->file);
        if (whence == SEEK_SET) {
            pos = offset;
        } else if (whence == SEEK_CUR) {
            pos += offset;
        } else if (whence == SEEK_END) {
            pos = (off_t) fsPtr->size + offset;
        }

        if (PHYSFS_seek(fsPtr->file, pos) == 0) {
            printf("NOK: lseek_hook(%i, %li, %i)\n", fd, offset, whence);
            return -1;
        }

        printf("OK: lseek_hook(%i, %li, %i), ret = %li\n", fd, offset, whence, pos);
        return pos;
    }

    return lseek_func(fd, offset, whence);
}

static off64_t (*lseek64_func)(int fd, off64_t offset, int whence);

static off64_t lseek64_hook(int fd, off64_t offset, int whence) {

    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: lseek64_hook(%i, %li, %i)\n", fd, offset, whence);
        return lseek_hook(fd, offset, whence);
    }

    return lseek64_func(fd, offset, whence);
}

static int (*close_func)(int fd);

static int close_hook(int fd) {

    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: close_hook(%i)\n", fd);
        PHYSFS_close(fsPtr->file);
        physDel(fsPtr->fd);
        return 0;
    }

    return close_func(fd);
}

int (*stat_func)(const char *pathname, struct stat *buf);

int stat_hook(const char *pathname, struct stat *buf) {

    if (isRomFs(pathname)) {
        std::string path = std::string(pathname).replace(0, 6, "/romfs");
        PHYSFS_Stat st{};
        if (PHYSFS_stat(path.c_str(), &st) == 0) {
            printf("NOK: stat_hook(%s)\n", path.c_str());
            return -1;
        }

        printf("OK: stat_hook(%s)\n", path.c_str());
        memset(buf, 0, sizeof(struct stat));
        buf->st_size = st.filesize;
        buf->st_atime = st.accesstime;
        buf->st_mtime = st.modtime;
        buf->st_ctime = st.createtime;
        if (st.filetype == PHYSFS_FILETYPE_DIRECTORY) {
            buf->st_mode = S_IFDIR;
        } else if (st.filetype == PHYSFS_FILETYPE_SYMLINK) {
#ifndef __WINDOWS__
            buf->st_mode = S_IFLNK;
#endif
        } else {
            buf->st_mode = S_IFREG;
        }
        buf->st_nlink = 1;
#ifdef __WINDOWS__
        buf->st_size = 512;
#else
        buf->st_blksize = 512;
        buf->st_blocks = (buf->st_blksize + 511) / 512;
#endif
        return 0;
    }

    return stat_func(pathname, buf);
}

int (*stat64_func)(const char *pathname, struct stat64 *buf);

int stat64_hook(const char *pathname, struct stat64 *buf) {
    if (isRomFs(pathname)) {
        printf("OK: stat64_hook(%s)\n", pathname);
        return stat_hook(pathname, (struct stat *) buf);
    }
    return stat64_func(pathname, buf);
}

#ifdef __LINUX__

int (*lstat_func)(const char *pathname, struct stat *buf);

int lstat_hook(const char *pathname, struct stat *buf) {
    if (isRomFs(pathname)) {
        printf("OK: lstat_hook(%s)\n", pathname);
        return stat_hook(pathname, buf);
    }
    return lstat_func(pathname, buf);
}

int (*xstat_func)(int ver, const char *pathname, struct stat *buf);

int xstat_hook(int ver, const char *pathname, struct stat *buf) {

    if (isRomFs(pathname)) {
        printf("OK: xstat_hook(%s)\n", pathname);
        return stat_hook(pathname, buf);
    }

    return xstat_func(ver, pathname, buf);
}

int (*lxstat_func)(int ver, const char *pathname, struct stat *buf);

int lxstat_hook(int ver, const char *pathname, struct stat *buf) {

    if (isRomFs(pathname)) {
        printf("OK: lxstat_hook(%s)\n", pathname);
        return stat_hook(pathname, buf);
    }

    return lxstat_func(ver, pathname, buf);
}

#endif

static FILE *(*fopen_func)(const char *filename, const char *mode);

static FILE *fopen_hook(const char *filename, const char *mode) {

    if (isRomFs(filename)) {
        printf("OK: fopen_hook(%s, %s)\n", filename, mode);
        int fd = open_hook(filename, O_RDONLY, 0);
        if (fd > 0) {
            FILE *file = (FILE *) malloc(sizeof(FILE));
            memset(file, 0, sizeof(FILE));
#ifdef __WINDOWS__
            file->_file = fd;
#else
            file->_fileno = fd;
            file->_flags = 0x0004; // ok to read
#endif
            return file;
        } else {
            return nullptr;
        }
    }

    return fopen_func(filename, mode);
}

static size_t (*fread_func)(void *buffer, size_t blocSize, size_t blocCount, FILE *stream);

static size_t fread_hook(void *buffer, size_t blocSize, size_t blocCount, FILE *stream) {
#ifdef __WINDOWS__
    int fd = stream->_file;
#else
    int fd = stream->_fileno;
#endif
    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: fread_hook(%i, %p, %lu, %lu)\n", fd, buffer, blocSize, blocCount);
        return read_hook(fd, buffer, blocSize * blocCount);
    }

    return fread_func(buffer, blocSize, blocCount, stream);
}

static int (*fseek_func)(FILE *stream, long offset, int whence);

static int fseek_hook(FILE *stream, long offset, int whence) {
#ifdef __WINDOWS__
    int fd = stream->_file;
#else
    int fd = stream->_fileno;
#endif
    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: fseek_hook(%i, %lu, %i)\n", fd, offset, whence);
        return (int) lseek_hook(fd, offset, whence);
    }

    return fseek_func(stream, offset, whence);
}

static int (*fseeko_func)(FILE *stream, off_t offset, int whence);

static int fseeko_hook(FILE *stream, off_t offset, int whence) {
#ifdef __WINDOWS__
    int fd = stream->_file;
#else
    int fd = stream->_fileno;
#endif
    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: fseeko_hook(%i, %lu, %i)\n", fd, offset, whence);
        return (int) lseek_hook(fd, offset, whence);
    }

    return fseeko_func(stream, offset, whence);
}

static long (*ftell_func)(FILE *stream);

static long int ftell_hook(FILE *stream) {
#ifdef __WINDOWS__
    int fd = stream->_file;
#else
    int fd = stream->_fileno;
#endif
    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: ftell_hook(%i)\n", fd);
        return (long int) PHYSFS_tell(fsPtr->file);
    }

    return ftell_func(stream);
}

static off_t (*ftello_func)(FILE *stream);

static off_t ftello_hook(FILE *stream) {
#ifdef __WINDOWS__
    int fd = stream->_file;
#else
    int fd = stream->_fileno;
#endif
    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: ftell_hook(%i)\n", fd);
        return (off_t) PHYSFS_tell(fsPtr->file);
    }

    return ftello_func(stream);
}

static int (*fclose_func)(FILE *stream);

static int fclose_hook(FILE *stream) {
#ifdef __WINDOWS__
    int fd = stream->_file;
#else
    int fd = stream->_fileno;
#endif
    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: fclose_hook(%i)\n", fd);
        close_hook(fd);
        free(stream);
        return 0;
    }

    return fclose_func(stream);
}

static int (*fileno_func)(FILE *stream);

static int fileno_hook(FILE *stream) {
#ifdef __WINDOWS__
    int fd = stream->_file;
#else
    int fd = stream->_fileno;
#endif
    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: fileno_hook(%i)\n", fd);
        return fd;
    }

    return fileno_func(stream);
}

static int (*fgetc_func)(FILE *stream);

int fgetc_hook(FILE *stream) {
#ifdef __WINDOWS__
    int fd = stream->_file;
#else
    int fd = stream->_fileno;
#endif
    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        char buf;
        ssize_t len = PHYSFS_readBytes(fsPtr->file, &buf, 1);
        if (len == 1) {
            printf("OK: fgetc_hook(%i)\n", fd);
            return buf;
        } else {
            printf("NOK: fgetc_hook(%i)\n", fd);
            return -1;
        }
    }

    return fgetc_func(stream);
}

int (*fstat_func)(int fd, struct stat *buf);

int fstat_hook(int fd, struct stat *buf) {

    printf("fstat_hook(%i)\n", fd);
    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: fstat_hook(%i)\n", fd);
        return stat_hook(fsPtr->path.c_str(), buf);
    }

    return fstat_func(fd, buf);
}

int (*fstat64_func)(int fd, struct stat64 *buf);

int fstat64_hook(int fd, struct stat64 *buf) {

    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: fstat64_hook(%i)\n", fd);
        return stat_hook(fsPtr->path.c_str(), (struct stat *) buf);
    }

    return fstat64_func(fd, buf);
}

#ifdef __LINUX__

int (*fxstat_func)(int rev, int fd, struct stat *buf);

int fxstat_hook(int rev, int fd, struct stat *buf) {

    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: fxstat_hook(%i)\n", fd);
        return fstat_hook(fd, buf);
    }

    return fxstat_func(rev, fd, buf);
}

#endif

int romfsExit() {

    if (!initialised)
        return -1;

    funchook_uninstall(funchook, 0);
    funchook_destroy(funchook);
    PHYSFS_deinit();
    physPtrList.clear();
    fdcount = INT_MAX;
    initialised = false;

    return 0;
}

int romfsInit() {

    int ret;

    if (initialised) {
        return -1;
    }

    printf("romfsInit... ");
    PHYSFS_init(nullptr);
    PHYSFS_uint64 size = c2d_romfs_length;
    PHYSFS_mountMemory(c2d_romfs, size,
                       nullptr, "romfs.zip", "/romfs", 1);

    funchook = funchook_create();

    open_func = (int (*)(const char *, int, mode_t)) open;
    ret = funchook_prepare(funchook, (void **) &open_func, (void *) open_hook);
    if (ret != 0) {
        printf("romfsInit: open_hook failed\n");
    }

    read_func = (ssize_t (*)(int, void *, size_t)) read;
    ret = funchook_prepare(funchook, (void **) &read_func, (void *) read_hook);
    if (ret != 0) {
        printf("romfsInit: read_hook failed\n");
    }

    lseek_func = (off_t (*)(int, off_t, int)) lseek;
    ret = funchook_prepare(funchook, (void **) &lseek_func, (void *) lseek_hook);
    if (ret != 0) {
        printf("romfsInit: lseek_hook failed\n");
    }

    lseek64_func = (off64_t (*)(int, off64_t, int)) lseek64;
    ret = funchook_prepare(funchook, (void **) &lseek64_func, (void *) lseek64_hook);
    if (ret != 0) {
        printf("romfsInit: lseek64_hook failed\n");
    }

    stat_func = (int (*)(const char *, struct stat *)) stat;
    ret = funchook_prepare(funchook, (void **) &stat_func, (void *) stat_hook);
    if (ret != 0) {
        printf("romfsInit: stat_hook failed\n");
    }

    stat64_func = (int (*)(const char *, struct stat64 *)) stat64;
    ret = funchook_prepare(funchook, (void **) &stat64_func, (void *) stat64_hook);
    if (ret != 0) {
        printf("romfsInit: stat64_hook failed\n");
    }

#ifdef __LINUX__
    lstat_func = (int (*)(const char *, struct stat *)) lstat;
    ret = funchook_prepare(funchook, (void **) &lstat_func, (void *) lstat_hook);
    if (ret != 0) {
        printf("romfsInit: lstat_hook failed\n");
    }
#if __GLIBC__ == 2 && __GLIBC_MINOR__ < 33
    xstat_func = (int (*)(int, const char *, struct stat *)) __xstat;
    ret = funchook_prepare(funchook, (void **) &xstat_func, (void *) xstat_hook);
    if (ret != 0) {
        printf("romfsInit: xstat_hook failed\n");
    }

    lxstat_func = (int (*)(int, const char *, struct stat *)) __lxstat;
    ret = funchook_prepare(funchook, (void **) &lxstat_func, (void *) lxstat_hook);
    if (ret != 0) {
        printf("romfsInit: lxstat_hook failed\n");
    }
#endif
#endif

    close_func = (int (*)(int)) close;
    ret = funchook_prepare(funchook, (void **) &close_func, (void *) close_hook);
    if (ret != 0) {
        printf("romfsInit: close_hook failed\n");
    }

    fopen_func = (FILE *(*)(const char *, const char *)) fopen;
    ret = funchook_prepare(funchook, (void **) &fopen_func, (void *) fopen_hook);
    if (ret != 0) {
        printf("romfsInit: fopen_hook failed\n");
    }

    fread_func = (size_t (*)(void *, size_t, size_t, FILE *)) fread;
    ret = funchook_prepare(funchook, (void **) &fread_func, (void *) fread_hook);
    if (ret != 0) {
        printf("romfsInit: fread_hook failed\n");
    }

    fseek_func = (int (*)(FILE *, long, int)) fseek;
    ret = funchook_prepare(funchook, (void **) &fseek_func, (void *) fseek_hook);
    if (ret != 0) {
        printf("romfsInit: fseek_hook failed\n");
    }

    fseeko_func = (int (*)(FILE *, off_t, int)) fseeko;
    ret = funchook_prepare(funchook, (void **) &fseeko_func, (void *) fseeko_hook);
    if (ret != 0) {
        printf("romfsInit: fseeko_hook failed\n");
    }

    ftell_func = (long int (*)(FILE *)) ftell;
    ret = funchook_prepare(funchook, (void **) &ftell_func, (void *) ftell_hook);
    if (ret != 0) {
        printf("romfsInit: ftell_hook failed\n");
    }

    ftello_func = (off_t (*)(FILE *)) ftello;
    ret = funchook_prepare(funchook, (void **) &ftello_func, (void *) ftello_hook);
    if (ret != 0) {
        printf("romfsInit: ftello_hook failed\n");
    }

    fstat_func = (int (*)(int, struct stat *)) fstat;
    ret = funchook_prepare(funchook, (void **) &fstat_func, (void *) fstat_hook);
    if (ret != 0) {
        printf("romfsInit: fstat_hook failed\n");
    }

    fstat64_func = (int (*)(int, struct stat64 *)) fstat64;
    ret = funchook_prepare(funchook, (void **) &fstat64_func, (void *) fstat64_hook);
    if (ret != 0) {
        printf("romfsInit: fstat64_hook failed\n");
    }

#if __LINUX__ && __GLIBC__ == 2 && __GLIBC_MINOR__ < 33
    fxstat_func = (int (*)(int, int, struct stat *)) __fxstat;
    ret = funchook_prepare(funchook, (void **) &fxstat_func, (void *) fxstat_hook);
    if (ret != 0) {
        printf("romfsInit: fxstat_hook failed\n");
    }
#endif

    fclose_func = (int (*)(FILE *)) fclose;
    ret = funchook_prepare(funchook, (void **) &fclose_func, (void *) fclose_hook);
    if (ret != 0) {
        printf("romfsInit: fclose_hook failed\n");
    }

    fileno_func = (int (*)(FILE *)) fileno;
    ret = funchook_prepare(funchook, (void **) &fileno_func, (void *) fileno_hook);
    if (ret != 0) {
        printf("romfsInit: fileno_hook failed\n");
    }

    fgetc_func = (int (*)(FILE *)) fgetc;
    ret = funchook_prepare(funchook, (void **) &fgetc_func, (void *) fgetc_hook);
    if (ret != 0) {
        printf("romfsInit: fgetc_hook failed\n");
    }

    ret = funchook_install(funchook, 0);
    if (ret != 0) {
        printf("romfsInit: funchook_install failed\n");
    }

    initialised = true;
    printf("romfsInit: done\n");

    return 0;
}
