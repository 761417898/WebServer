#ifndef TEST_LOGTEST_H
#define TEST_LOGTEST_H

#include "../base/log/Logging.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>

namespace GaoServer {

void logTest() {
    const char* str = "This is log test\n";
    //printf("%s %d\n", str, strlen(str));
    LOG << str;
}

    void testLogAllCases() {
        logTest();
    }

}

#endif
