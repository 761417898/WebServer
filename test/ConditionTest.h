#include "../base/Condition.h"
#include "../base/MutexLock.h"
#include <cassert>
#include <cstdlib>

namespace GaoServer {

    MutexLock mtxConditionTest;
    Condition condConditionTest(mtxConditionTest);

    int countConditionTest = 0;

    void* funcConditionTest(void*) {
        mtxConditionTest.lock();
        condConditionTest.wait();
        mtxConditionTest.unlock();
    }

    void testWaitSignal() {
        pthread_t tid;
        pthread_create(&tid, NULL, funcConditionTest, NULL);
        usleep(100);
        mtxConditionTest.lock();
        condConditionTest.notify();
        mtxConditionTest.unlock();
        pthread_join(tid, NULL);
    }

    void testConditionAllCases() {
        testWaitSignal();
    }

}
