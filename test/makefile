unittest: main.o CountDownLatch.o Thread.o Channel.o EPoller.o EventLoop.o TimerQueue.o
	g++ -g -W -o unittest main.o CountDownLatch.o Thread.o Channel.o EPoller.o EventLoop.o TimerQueue.o -lpthread

main.o: main.cc
	g++ -g -W -c -o main.o main.cc -lpthread

CountDownLatch.o: ../base/CountDownLatch.cc
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

clean:
	rm *.o
