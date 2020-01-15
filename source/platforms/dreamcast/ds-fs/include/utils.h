//
// Created by cpasjuste on 15/01/2020.
//

#ifndef RETRODREAM_UTILS_H
#define RETRODREAM_UTILS_H

void *memcpy_sh4(void *dest, const void *src, size_t count);
void *memmove_sh4(void *dest, const void *src, size_t count);
void *memset_sh4(void *dest, uint32 val, size_t count);

#define memcpy  memcpy_sh4
#define memmove memmove_sh4
#define memset  memset_sh4

#endif //RETRODREAM_UTILS_H
