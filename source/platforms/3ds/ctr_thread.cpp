//
// Created by cpasjuste on 04/02/2021.
//

#include "cross2d/c2d.h"

#warning "svcExitThread must be called manually when exiting thread function"

c2d::CTRThread::CTRThread(Function fn, void *data) : c2d::Thread(fn, data) {
    Result rc;
    s32 priority = 0;
    svcGetThreadPriority(&priority, CUR_THREAD_HANDLE);
    stack = linearMemAlign(CTR_THREAD_STACK_SIZE, 8);
    if (!stack) {
        return;
    }
    rc = svcCreateThread(&handle, (ThreadFunc) fn, (u32) data,
                         (uint32_t *) ((uint32_t) stack + CTR_THREAD_STACK_SIZE),
                         priority - 1, -2);
    if (R_FAILED(rc)) {
        free(stack);
        stack = nullptr;
    }
}

int c2d::CTRThread::join() {
    if (!stack) {
        return -1;
    }
    Result rc = svcWaitSynchronization(handle, INT64_MAX);
    svcCloseHandle(handle);
    free(stack);
    stack = nullptr;
    return R_SUCCEEDED(rc);
}

c2d::CTRThread::~CTRThread() {
    if (stack) {
        c2d::CTRThread::join();
    }
}
