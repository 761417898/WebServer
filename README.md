# WebServer
A C++ non-blocking network library like muduo

整体思路就是每个线程运行一个epoll，主线程的epoll用于监听主套接字，accept成功返回后的套接字
分发给工作线程监听并处理。

线程之间的任务分发通过任务队列来实现，互斥的把要执行的任务加到要调度线程的任务队列中。如何
唤醒处于epoll_loop中的线程呢，预先让该线程监听一个wakeup_fd，要调度该线程时直接写该wakeup_fd即可。

对于计时器事件的统一，不同于libevent中设置epoll超时时间的方式，计时器事件的处理通过linux中的
timerfd来实现，timerfd的时间应设置为最近要发生的计时器事件。计时器事件通过set来管理（方便对中间
元素进行操作）

为数据读写专门封装了缓冲区，并在缓冲区头部预留了很小的空间，为了能更快的处理小几率的首部插入

具体各个类的封装实现在《设计文档》中，本项目的设计思路来自于muduo网络库。