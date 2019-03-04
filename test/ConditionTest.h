#include "../base/Condition.h"
#include <cassert>
#include <cstdlib>

namespace GaoServer {

    static Condition cond;
    static MutexLock mtx;

    static int count = 0;

    static void* func(void*) {
        mtx.lock();
        cond.wait();
        count++;
        mtx.unlock();
    }

    static testWaitSignal() {
        mtx.lock();
        cond.signal();
        mtx.unlock();
        sleep(1);
        assert(count == 1);
    }

    void testConditionAllCases() {
        testWaitSignal();
    }

}
