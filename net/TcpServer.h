#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H

#include "EventLoop.h"
#include "Acceptor.h"
#include "InetAddress.h"
#include "TcpConnection.h"
#include "Socket.h"
#include <functional>

namespace GaoServer {

class TcpServer {
private:
    typedef std::function<void (const TcpConnectionPtr& conn)> ConnectionCallBack;
    typedef std::function<void (const TcpConnectionPtr& conn,
                                const char* data, ssize_t len)> MessageCallBack;
    ConnectionCallBack connectionCallBack_;
    MessageCallBack messageCallBack_;

    //used for acceptor, callback
    void newConnection(int sockfd, const InetAddress& peerAddr);
    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

    EventLoop* loop_;
    const std::string name_;
    std::shared_ptr<Acceptor> acceptor_;
    bool started_;
    int nextConnId_;
    ConnectionMap connections_;
public:
    TcpServer(EventLoop *loop, const InetAddress& listenAddr, std::string& name);
    ~TcpServer();
    void start();
    void setConnectionCallBack(const ConnectionCallBack &cb) {
        connectionCallBack_ = cb;
    }
    void setMessageCallBack(const MessageCallBack& cb) {
        messageCallBack_ = cb;
    }
};

}

#endif
