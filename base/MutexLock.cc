#include "MutexLock.h"

namespace GaoServer {

MutexLock::MutexLock() {
    pthread_mutex_init(&mutex_, NULL);
}
MutexLock::~MutexLock() {
    pthread_mutex_lock(&mutex_);
    pthread_mutex_destroy(&mutex_);
}
void MutexLock::lock() {
    pthread_mutex_lock(&mutex_);
}
void MutexLock::unlock() {
    pthread_mutex_unlock(&mutex_);
}
pthread_mutex_t* MutexLock::get() {
    return &mutex_;
}

}
