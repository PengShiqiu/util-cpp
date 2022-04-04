#include <iostream>


extern void TestRingQueue();
extern void TestLockQueue();
extern void ThreadPoolTest();

int main()
{
    TestRingQueue();
    TestLockQueue();
    ThreadPoolTest();
    return 0;
}