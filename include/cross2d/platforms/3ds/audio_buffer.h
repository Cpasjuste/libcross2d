#ifndef __AUDIO_BUFFER_H
#define __AUDIO_BUFFER_H

#include <cstring>

#if __cplusplus >= 201103L

#include <cstdint>

#else

#include <stdint.h>

#endif

namespace c2d {
    class AudioBuffer {
    public:
        int size;
        int buffer_size;
        int start;
        int16_t *buffer;

        static inline int min(int a, int b) {
            return ((a) < (b) ? (a) : (b));
        }

        AudioBuffer() {
            this->buffer_size = 0;
            buffer = nullptr;
        }

        ~AudioBuffer() {
            delete[] buffer;
            buffer = nullptr;
        }

        inline void clear() {
            if (buffer == nullptr)
                return;

            start = 0;
            size = 0;
            memset(buffer, 0, buffer_size * 2);
        }

        inline bool pull(int16_t *dst, int bufSize) {
            if (space_filled() < bufSize)
                return false;

            memcpy(dst, buffer + start, min(bufSize, buffer_size - start));

            if (bufSize > (buffer_size - start))
                memcpy(dst + (buffer_size - start), buffer, bufSize - (buffer_size - start));

            start = (start + bufSize) % buffer_size;
            size -= bufSize;

            return true;
        }

        inline bool push(int16_t *src, int bufSize) {
            if (space_empty() < bufSize)
                return false;

            int end = start + size;
            if (end > buffer_size)
                end -= buffer_size;
            int first_write_size = min(bufSize, buffer_size - end);

            memcpy(buffer + end, src, first_write_size);

            if (bufSize > first_write_size)
                memcpy(buffer, src + first_write_size, (bufSize - first_write_size));

            size += bufSize;

            return true;
        }

        inline int space_empty() const {
            return buffer_size - size;
        }

        inline int space_filled() const {
            return size;
        }

        inline int avail() {
            return size;
        }

        void resize(int bufSize) {
            if (buffer != nullptr)
                delete[] buffer;
            buffer_size = bufSize;
            buffer = new int16_t[buffer_size];
            clear();
        }
    };
}

#endif /* __AUDIO_BUFFER_H */
