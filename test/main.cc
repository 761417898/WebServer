#include "MutexLockTest.h"
#include "ConditionTest.h"
#include "CountDownLatchTest.h"
#include "ThreadTest.h"
#include "ReactorTest.h"
#include "AcceptorTest.h"
#include "TcpConnectionNewTest.h"
#include <pthread.h>
#include <iostream>

int main() 
{
    using namespace GaoServer;
    std::cout << " **********   UNITTEST   *************\n";
    //testMutexLockAllCases();
    //testConditionAllCases();
    //testCountDownLatchAllCases();
    //testThreadAllCases();
    //testReactorAllCases();
    //testAcceptorAllCases();
    testTcpConnectionNew();
}
