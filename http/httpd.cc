//like TinyHttpd
#include "../net/GaoServerHeaders.h"

using namespace GaoServer;

void onConnectionServer(const TcpConnectionPtr& conn) {
}

void onMessageServer(const TcpConnectionPtr& conn, const char* data, ssize_t len) {
    write(conn->socket_->fd(), "hello world too\n", 16);
}

void *closeMainLoopServer(void *arg) {
    EventLoop *mainLoop = static_cast<EventLoop*>(arg);
    ::sleep(12000);
    printf("mainLoop quited\n");
    mainLoop->quit();
}

int main()
{
    uint16_t testPort = 8888;
    InetAddress localAddr(testPort);
    std::string testName = "NEW TcpConnection Test";
    EventLoop loop;

    TcpServer server(&loop, localAddr, testName);
    server.setConnectionCallBack(onConnectionServer);
    server.setMessageCallBack(onMessageServer);

    server.start();

    pthread_t tid;
    pthread_create(&tid, NULL, closeMainLoopServer, (void*)&loop);
    loop.loop();
    pthread_join(tid, NULL);
}
