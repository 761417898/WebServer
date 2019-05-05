#include "Condition.h"
#include "MutexLock.h"

namespace GaoServer {

bool Condition::waitForSeconds(int seconds) {
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += static_cast<time_t>(seconds);
    //设置了阻塞时间，超时返回ETIMEDOUT
    return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.get(), &abstime);
}

}
