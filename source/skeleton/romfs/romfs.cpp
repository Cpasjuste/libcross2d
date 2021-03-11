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

extern "C" char _binary_romfs_zip_start[];
extern "C" char _binary_romfs_zip_end[];

struct PhysPtr {
    std::string path;
    PHYSFS_File *file = nullptr;
    int fd = 0;
    size_t size = 0;
};

static std::vector<PhysPtr> physPtrList;
static int32_t initialised = 0;
static funchook_t *funchook;
static int fdcount = INT_MAX;

static PhysPtr *physGet(int fd) {

    if (physPtrList.empty()) {
        return nullptr;
    }

    for (auto &ptr : physPtrList) {
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
        size_t len = buf ? PHYSFS_readBytes(fsPtr->file, buf, count) : 0;
        printf("OK: read_hook(%i, %p, %zu), read = %zu\n", fd, buf, count, len);
        return len;
    }

    return read_func(fd, buf, count);
}

static off_t (*lseek_func)(int fd, off_t offset, int whence);

static off_t lseek_hook(int fd, off_t offset, int whence) {

    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        off_t cur = PHYSFS_tell(fsPtr->file);
        if (whence == SEEK_SET) {
            cur = offset;
        } else if (whence == SEEK_CUR) {
            cur += offset;
        } else if (whence == SEEK_END) {
            cur = fsPtr->size + offset;
        }

        if (PHYSFS_seek(fsPtr->file, cur) == 0) {
            printf("NOK: lseek_hook(%i, %li, %i)\n", fd, offset, whence);
            return -1;
        }

        printf("OK: lseek_hook(%i, %li, %i)\n", fd, offset, whence);
        return cur;
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

int (*stat_func)(int, const char *pathname, struct stat *buf);

int stat_hook(int ver, const char *pathname, struct stat *buf) {

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

    return stat_func(ver, pathname, buf);
}

int (*lstat_func)(int ver, const char *pathname, struct stat *buf);

int lstat_hook(int ver, const char *pathname, struct stat *buf) {
    return stat_hook(_STAT_VER, pathname, buf);
}

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
        return lseek_hook(fd, offset, whence);
    }

    return fseek_func(stream, offset, whence);
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

int (*fstat_func)(int ver, int fd, struct stat *buf);

int fstat_hook(int ver, int fd, struct stat *buf) {

    PhysPtr *fsPtr = physGet(fd);
    if (fsPtr) {
        printf("OK: fstat_hook(%i)\n", fd);
        return stat_hook(ver, fsPtr->path.c_str(), buf);
    }

    return fstat_func(ver, fd, buf);
}

int romfsInit() {

    int ret;

    if (initialised)
        return 0;

    PHYSFS_init(nullptr);
    PHYSFS_uint64 size = _binary_romfs_zip_end - _binary_romfs_zip_start;
    PHYSFS_mountMemory(_binary_romfs_zip_start, size,
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

    stat_func = (int (*)(int, const char *, struct stat *)) __xstat;
    ret = funchook_prepare(funchook, (void **) &stat_func, (void *) stat_hook);
    if (ret != 0) {
        printf("romfsInit: stat_hook failed\n");
    }

    lstat_func = (int (*)(int, const char *, struct stat *)) __lxstat;
    ret = funchook_prepare(funchook, (void **) &lstat_func, (void *) lstat_hook);
    if (ret != 0) {
        printf("romfsInit: lstat_hook failed\n");
    }

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

    ftell_func = (long int (*)(FILE *)) ftell;
    ret = funchook_prepare(funchook, (void **) &ftell_func, (void *) ftell_hook);
    if (ret != 0) {
        printf("romfsInit: ftell_hook failed\n");
    }

    fstat_func = (int (*)(int, int, struct stat *)) __fxstat;
    ret = funchook_prepare(funchook, (void **) &fstat_func, (void *) fstat_hook);
    if (ret != 0) {
        printf("romfsInit: fstat_hook failed\n");
    }

    fclose_func = (int (*)(FILE *)) fclose;
    ret = funchook_prepare(funchook, (void **) &fclose_func, (void *) fclose_hook);
    if (ret != 0) {
        printf("romfsInit: fclose_hook failed\n");
    }

    ret = funchook_install(funchook, 0);
    if (ret != 0) {
        printf("romfsInit: funchook_install failed\n");
    }

    initialised = 1;

    return 0;
}

int romfsExit() {

    if (!initialised)
        return -1;

    funchook_uninstall(funchook, 0);
    funchook_destroy(funchook);
    PHYSFS_deinit();
    initialised = 0;

    return 0;
}
