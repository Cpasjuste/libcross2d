#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <physfs.h>
#include <funchook.h>
#include <algorithm>
#include <climits>

extern "C" char _binary_romfs_zip_start[];
extern "C" char _binary_romfs_zip_end[];

struct physfsPtr {
    PHYSFS_File *file;
    int fd;
    size_t size;
};

static std::vector<physfsPtr> *physfsPtrList;
static int32_t initialised = 0;
static funchook_t *funchook;
static int fdcount = INT_MAX;

static physfsPtr *physfsGet(int fd) {

    if (!physfsPtrList) {
        return nullptr;
    }

    for (auto &ptr : *physfsPtrList) {
        if (ptr.fd == fd) {
            return &ptr;
        }
    }

    return nullptr;
}

static void *physfsDel(int fd) {

    for (size_t i = 0; i < physfsPtrList->size(); i++) {
        if (physfsPtrList->at(i).fd == fd) {
            physfsPtrList->erase(physfsPtrList->begin() + i);
        }
    }

    return nullptr;
}

static bool isRomFs(const char *pathname) {
    return pathname && strlen(pathname) > 6 && strncmp(pathname, "romfs:", 6) == 0;
}

static int (*open_func)(const char *pathname, int flags, mode_t mode);

static int open_hook(const char *pathname, int flags, mode_t mode) {
    if (isRomFs(pathname)) {
        std::string path = std::string(pathname).replace(0, 6, "/romfs");
        PHYSFS_File *file;
        if (!flags || flags & O_RDONLY) {
            file = PHYSFS_openRead(path.c_str());
        } else if (flags & O_APPEND) {
            file = PHYSFS_openAppend(path.c_str());
        } else {
            file = PHYSFS_openWrite(path.c_str());
        }
        if (file) {
            fdcount--;
            physfsPtrList->push_back({file, fdcount, PHYSFS_fileLength(file)});
            //printf("OK: open_hook:(%s, %i, %i)\n", path.c_str(), flags, mode);
            return fdcount;
        } else {
            printf("NOK: open_hook:(%s, %i, %i)\n", path.c_str(), flags, mode);
            return -1;
        }
    }

    return open_func(pathname, flags, mode);
}

static ssize_t (*read_func)(int fd, void *buf, size_t count);

static ssize_t read_hook(int fd, void *buf, size_t count) {

    physfsPtr *fsPtr = physfsGet(fd);
    if (fsPtr) {
        size_t len = PHYSFS_readBytes(fsPtr->file, buf, count);
        //printf("OK: read_hook:(%i, %p, %zu), read = %zu\n", fd, buf, count, len);
        return len;
    }

    return read_func(fd, buf, count);
}

static int (*close_func)(int fd);

static int close_hook(int fd) {

    physfsPtr *fsPtr = physfsGet(fd);
    if (fsPtr) {
        //printf("OK: close_hook:(%i)\n", fd);
        PHYSFS_close(fsPtr->file);
        physfsDel(fsPtr->fd);
    }

    return close_func(fd);
}

static int (*fclose_func)(FILE *stream);

static int fclose_hook(FILE *stream) {

    physfsPtr *fsPtr = physfsGet(stream->_fileno);
    if (fsPtr) {
        //printf("OK: fclose_hook:(%i)\n", stream->_fileno);
        PHYSFS_close(fsPtr->file);
        physfsDel(fsPtr->fd);
    }

    return fclose_func(stream);
}

static off_t (*lseek_func)(int fd, off_t offset, int whence);

static off_t lseek_hook(int fd, off_t offset, int whence) {

    physfsPtr *fsPtr = physfsGet(fd);
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
            printf("NOK: lseek_hook:(%i, %li, %i)\n", fd, offset, whence);
            return -1;
        }

        //printf("OK: lseek_hook:(%i, %li, %i)\n", fd, offset, whence);
        return cur;
    }

    return lseek_func(fd, offset, whence);
}

int (*stat_func)(const char *pathname, struct stat *buf);

int stat_hook(const char *pathname, struct stat *buf) {

    if (isRomFs(pathname)) {
        std::string path = std::string(pathname).replace(0, 6, "/romfs");
        PHYSFS_Stat st{};
        if (PHYSFS_stat(path.c_str(), &st) == 0) {
            return -1;
        }

        memset(buf, 0, sizeof(struct stat));
        buf->st_size = st.filesize;
        buf->st_atime = st.accesstime;
        buf->st_mtime = st.modtime;
        buf->st_ctime = st.createtime;
        if (st.filetype == PHYSFS_FILETYPE_REGULAR) {
            buf->st_mode = S_IFREG;
        } else if (st.filetype == PHYSFS_FILETYPE_DIRECTORY) {
            buf->st_mode = S_IFDIR;
        } else if (st.filetype == PHYSFS_FILETYPE_SYMLINK) {
            buf->st_mode = S_IFLNK;
        }
        buf->st_nlink = 1;
        buf->st_blksize = 512;
        buf->st_blocks = (buf->st_blksize + 511) / 512;
        return 0;
    }

    return stat_func(pathname, buf);
}

int romfsInit() {

    if (initialised)
        return 0;

    PHYSFS_init(nullptr);
    PHYSFS_uint64 size = _binary_romfs_zip_end - _binary_romfs_zip_start;
    PHYSFS_mountMemory(_binary_romfs_zip_start, size,
                       nullptr, "romfs.zip", "/romfs", 1);

    physfsPtrList = new std::vector<physfsPtr>();

    funchook = funchook_create();

    open_func = (int (*)(const char *, int, mode_t)) open;
    funchook_prepare(funchook, (void **) &open_func, (void *) open_hook);

    read_func = (ssize_t (*)(int, void *, size_t)) read;
    funchook_prepare(funchook, (void **) &read_func, (void *) read_hook);

    close_func = (int (*)(int)) close;
    funchook_prepare(funchook, (void **) &close_func, (void *) close_hook);

    fclose_func = (int (*)(FILE *)) fclose;
    funchook_prepare(funchook, (void **) &fclose_func, (void *) fclose_hook);

    lseek_func = (off_t (*)(int, off_t, int)) lseek;
    funchook_prepare(funchook, (void **) &lseek_func, (void *) lseek_hook);

    stat_func = (int (*)(const char *, struct stat *)) stat;
    funchook_prepare(funchook, (void **) &stat_func, (void *) stat_hook);

    funchook_install(funchook, 0);

    initialised = 1;

    return 0;
}

int romfsExit() {

    if (!initialised)
        return -1;

    funchook_destroy(funchook);

    PHYSFS_deinit();
    delete (physfsPtrList);
    physfsPtrList = nullptr;

    initialised = 0;

    return 0;
}
