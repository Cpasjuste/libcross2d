//
// Created by cpasjuste on 04/02/2021.
//

#include "cross2d/c2d.h"

using namespace c2d;

CTRThread::CTRThread(Function fn, void *data) : Thread(fn, data) {
    s32 prio = 0;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    thread = threadCreate(reinterpret_cast<ThreadFunc>(fn), data, STACKSIZE, prio - 1, -2, false);
    //thread = thd_create(0, reinterpret_cast<void *(*)(void *)>(fn), data);
}

int CTRThread::join() {
    int ret = -1;
    if (thread) {
        ret = thd_join(thread, nullptr);
        thread = nullptr;
    }

    return ret;
}

CTRThread::~CTRThread() {
    if (thread) {
        thd_destroy(thread);
    }
}
