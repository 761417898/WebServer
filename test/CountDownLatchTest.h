#ifndef TEST_COUNTDOWNLATCHTEST_H
#define TEST_COUNTDOWNLATCHTEST_H

#include "../base/CountDownLatch.h"
#include <pthread.h>

namespace GaoServer {

    CountDownLatch cdlCountDownLatchTest(1);

    void* funcCountDownLatch(void*) {
        cdlCountDownLatchTest.wait();
    }

    void testCountDownLatchWaitNotify() {
        pthread_t tid;
        pthread_create(&tid, NULL, funcCountDownLatch, NULL);
        usleep(100);
        cdlCountDownLatchTest.countDown();
        pthread_join(tid, NULL);
    }

    void testCountDownLatchAllCases() {
        testCountDownLatchWaitNotify();
    }

}

#endif
