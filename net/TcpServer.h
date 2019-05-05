#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H

#include "EventLoop.h"
#include "Acceptor.h"
#include "InetAddress.h"
#include "TcpConnection.h"
#include "Socket.h"
#include "Buffer.h"

namespace GaoServer {

class EventLoopThreadPool;

class TcpServer {
private:
    typedef std::function<void (const TcpConnectionPtr& conn)> ConnectionCallBack;
    typedef std::function<void (const TcpConnectionPtr& conn,
                                Buffer* buffer)> MessageCallBack;
    typedef std::function<void (const TcpConnectionPtr&)> CloseCallBack;
    ConnectionCallBack connectionCallBack_;
    MessageCallBack messageCallBack_;
    CloseCallBack closeCallBack_;

    //used for acceptor, callback
    void newConnection(int sockfd, const InetAddress& peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr&conn);

    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

    EventLoop* loop_;
    const std::string name_;
    std::shared_ptr<Acceptor> acceptor_;

    std::shared_ptr<EventLoopThreadPool> threadPool_;

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
    void setCloseCallBack(const CloseCallBack& cb) {
        closeCallBack_ = cb;
    }
};

}

#endif
