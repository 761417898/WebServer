#ifndef NET_REACTORTEST_H
#define NET_REACTORTEST_H

#include "../net/EventLoop.h"
#include "../net/Channel.h"
#include "../net/EPoller.h"
#include <cassert>
#include <cstdio>

namespace GaoServer {
    
    void testEventLoopConstruct() {
        EventLoop mainLoop;
        assert(mainLoop.getEventLoopOfCurrentThread() == &mainLoop);
    }

    void callBackReactorTest() {
        printf("This is ReadCallBack\n");
    }

    void testReactor(EventLoop &mainLoop) {
        Channel channel(&mainLoop, 1);
        channel.enableReading();
        channel.setReadCallBack(callBackReactorTest);

//        mainLoop.loop();
//
//        mainLoop.quit();

//        channel.disableReading();
    }

    void testReactorAllCases() {
        EventLoop mainLoop;
//        testEventLoopConstruct();
        testReactor(mainLoop);
printf("end of testReactor\n");
    }

}

#endif
