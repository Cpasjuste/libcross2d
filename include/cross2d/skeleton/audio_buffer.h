#ifndef __AUDIO_BUFFER_H
#define __AUDIO_BUFFER_H

#include <cstring>

#if __cplusplus >= 201103L

#include <cstdint>

#else

#include <stdint.h>

#endif

// TODO: fix mono ?

namespace c2d {
    class SampleBuffer {
    public:
        SampleBuffer() {
            buffer_size = 0;
            buffer = nullptr;
        }

        explicit SampleBuffer(int num_samples) {
            buffer_size = num_samples;
            buffer = new int16_t[buffer_size];
            clear();
        }

        ~SampleBuffer() {
            delete[] buffer;
            buffer = nullptr;
        }

        inline void clear() {
            if (buffer == nullptr) {
                return;
            }
            start = 0;
            size = 0;
            memset(buffer, 0, buffer_size * 2);
        }

        inline bool pull(int16_t *dst, int num_samples) {
            if (space_filled() < num_samples)
                return false;

            memcpy(dst, buffer + start, min(num_samples, buffer_size - start) * 2);

            if (num_samples > (buffer_size - start))
                memcpy(dst + (buffer_size - start), buffer, (num_samples - (buffer_size - start)) * 2);

            start = (start + num_samples) % buffer_size;
            size -= num_samples;

            return true;
        }

        inline bool push(int16_t *src, int num_samples) {
            if (space_empty() < num_samples)
                return false;

            int end = start + size;
            if (end > buffer_size)
                end -= buffer_size;
            int first_write_size = min(num_samples, buffer_size - end);

            memcpy(buffer + end, src, first_write_size * 2);

            if (num_samples > first_write_size)
                memcpy(buffer, src + first_write_size, (num_samples - first_write_size) * 2);

            size += num_samples;

            return true;
        }

        inline int space() const {
            return buffer_size;
        }

        inline int space_empty() const {
            return buffer_size - size;
        }

        inline int space_filled() const {
            return size;
        }

        void resize(int num_samples) {
            if (buffer) {
                delete[] buffer;
            }
            buffer_size = num_samples;
            buffer = new int16_t[buffer_size];
            clear();
        }

    private:
        int size = 0;
        int start = 0;
        int16_t *buffer;
        int buffer_size;

        static inline int min(int a, int b) {
            return ((a) < (b) ? (a) : (b));
        }
    };
}

#endif /* __AUDIO_BUFFER_H */
