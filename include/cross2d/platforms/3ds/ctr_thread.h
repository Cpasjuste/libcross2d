//
// Created by cpasjuste on 04/02/2021.
//

#ifndef CROSS2D_CTR_THREAD_H
#define CROSS2D_CTR_THREAD_H

#define STACKSIZE (4 * 1024)

namespace c2d {

    class CTRThread : public c2d::Thread {

    public:
        CTRThread(Function fn, void *data);

        ~CTRThread();

        int join() override;

    private:
        Thread thread;
    };
}

#endif //CROSS2D_CTR_THREAD_H
