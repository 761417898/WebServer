#ifndef TEST_TCPCONNECTIONNEW_H
#define TEST_TCPCONNECTIONNEW_H

#include <cassert>
#include <cstdio>
#include <unistd.h>
#include "../net/Buffer.h"
#include "../net/TcpServer.h"

namespace GaoServer {

void onConnectionTestNewConn(const TcpConnectionPtr& conn) {
    //printf("This is onConnectionTestNewConn\n");
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buffer) {
    //printf("on message, received: %ld bytes, %s\n", buffer->readableBytes(), (buffer->readAsString(12)).c_str());

    //std::string message = "hello world too\n";
    //conn->send(message);
}

void *closeMainLoopTcpConnection(void *arg) {
    EventLoop *mainLoop = static_cast<EventLoop*>(arg);
    ::sleep(120);
    printf("mainLoop quited\n");
    mainLoop->quit();
}

void testTcpConnectionNew() {
    //printf("main(): pid = %d\nNEW TcpConnection Test\n", getpid());
    uint16_t testPort = 8888;
    InetAddress localAddr(testPort);
    std::string testName = "NEW TcpConnection Test";
    EventLoop loop;

    TcpServer server(&loop, localAddr, testName);
    server.setConnectionCallBack(onConnectionTestNewConn);
    server.setMessageCallBack(onMessage);

    server.start();

    pthread_t tid;
    pthread_create(&tid, NULL, closeMainLoopTcpConnection, (void*)&loop);
    loop.loop();
    pthread_join(tid, NULL);
}

}

#endif
