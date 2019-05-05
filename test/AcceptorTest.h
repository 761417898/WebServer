#ifndef TEST_ACCEPTORTEST_H
#define TEST_ACCEPTORTEST_H

#include <stdint.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <cassert>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include "../net/EventLoop.h"
#include "../net/Acceptor.h"

namespace GaoServer {

void testSocket() {
    Socket sockfd;
}

static void testAcceptorNewConnCallBack(int sockfd, const InetAddress& addr) {
    printf("This is testAcceptorNewConnCallBack, accept a new Connection\n");
}

static void* closeMainLoopTestAcceptor(void* arg) {
    EventLoop *mainLoop = static_cast<EventLoop*>(arg);
    ::sleep(60);
    printf("mainLoop quited\n");
    mainLoop->quit();
}

void testAcceptor() {
    printf("\nAcceptor test begin, and will be closed after 120s:\n");
    uint16_t testPort = 5678;
    InetAddress listenAddr(testPort);
    EventLoop loop;
    Acceptor accept(&loop, listenAddr);
    accept.setNewConnectionCallBack(testAcceptorNewConnCallBack);
    accept.listen();

    pthread_t tid;
    pthread_create(&tid, NULL, closeMainLoopTestAcceptor, (void*)&loop);
    loop.loop();
    pthread_join(tid, NULL);
}

void testAcceptorAllCases() {
    testSocket();
    testAcceptor();
}

}

#endif
