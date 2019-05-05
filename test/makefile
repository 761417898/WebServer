unittest: main.o AsyncLogging.o FileUtil.o LogFile.o Logging.o LogStream.o Condition.o MutexLock.o EventLoopThread.o EventLoopThreadPool.o CountDownLatch.o Thread.o Channel.o EPoller.o EventLoop.o TimerQueue.o Socket.o InetAddress.o Acceptor.o TcpConnection.o TcpServer.o Buffer.o
	g++ -g -W -o unittest main.o AsyncLogging.o FileUtil.o LogFile.o Logging.o LogStream.o Condition.o MutexLock.o EventLoopThread.o EventLoopThreadPool.o CountDownLatch.o Thread.o Channel.o EPoller.o EventLoop.o TimerQueue.o Socket.o InetAddress.o Acceptor.o TcpConnection.o TcpServer.o Buffer.o -lpthread

main.o: main.cc
	g++ -g -W -c -o main.o main.cc -lpthread

AsyncLogging.o:
	g++ -g -W -c -o AsyncLogging.o ../base/log/AsyncLogging.cc -lpthread

FileUtil.o:
	g++ -g -W -c -o FileUtil.o ../base/log/FileUtil.cc -lpthread

LogFile.o:
	g++ -g -W -c -o LogFile.o ../base/log/LogFile.cc -lpthread

Logging.o:
	g++ -g -W -c -o Logging.o ../base/log/Logging.cc -lpthread

LogStream.o:
	g++ -g -W -c -o LogStream.o ../base/log/LogStream.cc -lpthread

Condition.o:
	g++ -g -W -c -o Condition.o ../base/Condition.cc -lpthread

MutexLock.o:
	g++ -g -W -c -o MutexLock.o ../base/MutexLock.cc -lpthread

CountDownLatch.o:
	g++ -g -W -c -o CountDownLatch.o -lpthread ../base/CountDownLatch.cc

Thread.o:
	g++ -g -W -c -o Thread.o ../base/Thread.cc -lpthread

Channel.o:
	g++ -g -W -c -o Channel.o ../net/Channel.cc -lpthread

EPoller.o:
	g++ -g -W -c -o EPoller.o ../net/EPoller.cc -lpthread

EventLoop.o:
	g++ -g -W -c -o EventLoop.o ../net/EventLoop.cc -lpthread

TimerQueue.o:
	g++ -g -W -c -o TimerQueue.o ../net/TimerQueue.cc -lpthread

Socket.o:
	g++ -g -W -c -o Socket.o ../net/Socket.cc

InetAddress.o:
	g++ -g -W -c -o InetAddress.o ../net/InetAddress.cc

Acceptor.o:
	g++ -g -W -c -o Acceptor.o ../net/Acceptor.cc

TcpConnection.o:
	g++ -g -W -c -o TcpConnection.o ../net/TcpConnection.cc

TcpServer.o:
	g++ -g -W -c -o TcpServer.o ../net/TcpServer.cc

Buffer.o:
	g++ -g -W -c -o Buffer.o ../net/Buffer.cc

EventLoopThread.o:
	g++ -g -W -c -o EventLoopThread.o ../net/EventLoopThread.cc

EventLoopThreadPool.o:
	g++ -g -W -c -o EventLoopThreadPool.o ../net/EventLoopThreadPool.cc

clean:
	rm *.o
