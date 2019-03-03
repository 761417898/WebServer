#ifndef BASE_CURRENTTHREAD_H
#define BASE_CURRENTTHREAD_H

//#include <stdint.h>

namespace GaoServer {

class CurrentThread {
    extern __thread int t_cachedTid;
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    extern __thread const char *t_threadName;
    void cachedTid();

    inline int tid() {
        if (__builtin_expect(t_cachedTid == 0, 0)) {
        //告诉编译器判断为真的概率大，方便指令预取
            cachedTid();
        }
        return t_cachedTid;
    }

    inline const char* tidString() {
        return t_tidString;
    }//for logging

    inline int tidStringLength() {
        return t_tidStringLength;
    }

    inline const char* name() {
        return t_threadName;
    }


};

}

#endif // BASE_CURRENTTHREAD_H
