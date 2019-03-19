#include "MutexLockTest.h"
#include "ConditionTest.h"
#include <iostream>

int main() 
{
    using namespace GaoServer;
    std::cout << " **********   UNITTEST   *************\n";
    testMutexLockAllCases();
    testConditionAllCases();
}
