/*
 * 接accept，控制流又回到了tcpserevr的newconnection中，这里会新建一个connection，connection的构造函数会对
 * connectfd创建channel并注册。这里刚建立连接时会调用早已注册的connCallBack，收到消息会调用messageCallBack
 */

#ifndef NET_TCPCONNECTION_H
#define NET_TCPCONNECTION_H

#include <memory>
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"

namespace GaoServer {

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection {
private:
    enum StateE {kConnecting, kConnected};
    StateE state_;
    void setState(StateE s) {
        state_ = s;
    }
    void handleRead();
    EventLoop* loop_;
    std::string name_;
    std::shared_ptr<Socket> socket_;
    std::shared_ptr<Channel> channel_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    const TcpConnectionPtr sharedThis;

    typedef std::function<void (const TcpConnectionPtr& conn)> ConnectionCallBack;
    typedef std::function<void (const TcpConnectionPtr& conn,
                                const char* data, ssize_t len)> MessageCallBack;
    ConnectionCallBack connectionCallBack_;
    MessageCallBack messageCallBack_;
public:
    TcpConnection(EventLoop* loop,
                  const std::string& name,
                  int sockfd,
                  const InetAddress& localAddr,
                  const InetAddress& peerAddr);
    ~TcpConnection() {

    }
    void setConnectionCallBack(ConnectionCallBack& cb) {
        connectionCallBack_ = cb;
    }
    void setMessageCallBack(MessageCallBack& cb) {
        messageCallBack_ = cb;
    }
    void connectEstablish();
};

}

#endif
