#ifndef Test_MutexLockTest_H
#define Test_MutexLockTest_H

#include "../base/MutexLock.h"
#include <cassert>
#include <unistd.h>
#include <pthread.h>

namespace GaoServer {
    static MutexLock mtx;
    static int count = 0;
    static void* func(void*) {
        mtx.lock();
        count++;
        mtx.unlock();
    }

    static void testLockUnlock() {
        pthread_t tid;
        assert(pthread_create(&tid, NULL, func, NULL) != -1);
        sleep(1);
        mtx.lock();
        assert(count == 1);
        mtx.unlock();
        pthread_join(tid, NULL);
    }

    void testMutexLockAllCases() {
        testLockUnlock();
    }
}

#endif
