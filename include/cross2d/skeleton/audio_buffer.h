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

        AudioBuffer(int num_samples) {
            this->buffer_size = num_samples;
            buffer = new int16_t[this->buffer_size];
            clear();
        }

        ~AudioBuffer() {
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

        inline int space_empty() const {
            return buffer_size - size;
        }

        inline int space_filled() const {
            return size;
        }

        void resize(int num_samples) {
            if (buffer != nullptr) {
                delete[] buffer;
            }
            buffer_size = num_samples;
            buffer = new int16_t[buffer_size];
            clear();
        }
    };
}

#endif /* __AUDIO_BUFFER_H */
