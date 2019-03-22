#ifndef TEST_THREADTEST_H
#define TEST_THREADTEST_H

#include "../base/Thread.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>

namespace GaoServer {

    bool funcStartThreadTest = false;

    static void funcThreadTest() {
        funcStartThreadTest = true;
        //printf("%s\n%d\n", CurrentThread::name(), CurrentThread::tid());
    }

    void testThreadAllCases() {
        Thread thread(funcThreadTest);
        thread.start();
        thread.join();
        assert(true == funcStartThreadTest);
        
    }

}

#endif
