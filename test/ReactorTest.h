#ifndef NET_REACTORTEST_H
#define NET_REACTORTEST_H

#include "../net/EventLoop.h"
#include "../net/Channel.h"
#include "../net/EPoller.h"
#include "../base/Thread.h"
#include <cassert>
#include <cstdio>
#include <ctime>
#include <pthread.h>

namespace GaoServer {

    static void callBackReactorTest() {
        printf("This is ReadCallBack\n");
        void *buf[100];
        read(STDIN_FILENO, buf, sizeof (buf));
    }

    void testChannel() {
        int testFd = 1;
        Channel channel;
        channel.setFd(testFd);
        channel.enableReading();
        channel.setReadCallBack(callBackReactorTest);
    }

    void testEPoller() {
        int testFd = 1;
        Channel chan;
        Channel *channel = &chan;
        channel->setFd(testFd);
        channel->enableReading();
        channel->setReadCallBack(callBackReactorTest);

        EPoller ep;
        ep.epollAdd(channel);
        printf("please input any words...\n");

        std::vector<Channel*> activeChannelList;

        ep.poll(activeChannelList);
 //       printf("active %d\n", activeChannelList.size());
        assert(activeChannelList[0]->fd() == testFd);

    }

    static void* closeMainLoop(void* arg) {
        EventLoop *mainLoop = static_cast<EventLoop*>(arg);
        printf("waitting mainLoop quited thread\n");
        sleep(6);
        printf("mainLoop quited\n");
        mainLoop->quit();
    }

    static void callBackTimerTest() {
        printf("This is callBackTimerTest, ok\n");
    }

    void testReactor() {

        int testFd = 1;
        Channel chan;
        Channel *channel = &chan;
        channel->setFd(testFd);
        channel->enableReading();
        channel->setReadCallBack(callBackReactorTest);

        EventLoop mainLoop;
        //test ChannelLoop
        mainLoop.addChannel(channel);
        //test TimerEvent
        timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        now.tv_sec += 5;
        mainLoop.addTimer(callBackTimerTest, now);
        now.tv_sec -= 2;
        mainLoop.addTimer(callBackTimerTest, now);

      /*  itimerspec new_value;
        new_value.it_value.tv_sec = 5; //第一次到期的时间
        new_value.it_value.tv_nsec = 0;
        new_value.it_interval.tv_sec = 0;
        new_value.it_interval.tv_nsec = 0;
        int timefd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
        timerfd_settime(timefd, 0, &new_value, NULL);
        Channel *timerEvent = new Channel();
        timerEvent->setFd(timefd);
        timerEvent->enableReading();
        timerEvent->setReadCallBack(callBackTimerTest);
        mainLoop.addChannel(timerEvent);

*/
        // to close loop
        pthread_t tid;
        pthread_create(&tid, NULL, closeMainLoop, (void*)&mainLoop);

        printf("please input any words\n");
        mainLoop.loop();

        pthread_join(tid, NULL);
    }

    void testReactorAllCases() {
        testChannel();
        //testEPoller();
        testReactor();
    }

}

#endif
