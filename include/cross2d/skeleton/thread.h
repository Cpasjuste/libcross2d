//
// Created by cpasjuste on 04/02/2021.
//

#ifndef CROSS2D_THREAD_H
#define CROSS2D_THREAD_H

namespace c2d {

    class Thread {

        typedef int (*ThreadFunc)(void *data);

    public:
        Thread(ThreadFunc fn, void *data) {};

        virtual ~Thread() = default;

        virtual int join() {
            printf("c2d::Thread:join: unimplemented\n");
            return 0;
        };
    };
}

#endif //CROSS2D_THREAD_H
