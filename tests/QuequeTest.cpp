#include <time.h>

#include <iostream>
#include <thread>

#include "../src/Queue/LockQueue.hpp"
#include "../src/Queue/RingQueue.hpp"

#define TEST_NUM 1000000
clock_t start, end;

void TestRingQueue() {
  start = clock();
  auto queue_ptr = std::make_shared<RingQueue<int, 10>>();
  auto write_th = std::thread([queue_ptr] {
    for (int i = 0; i < TEST_NUM; i++) {
      while (!queue_ptr->Push(i))
        ;
    }
  });

  auto read_th = std::thread([queue_ptr] {
    while (1) {
      int i = 0;
      if (!queue_ptr->Pop(i)) {
        // std::cout << "pop fail:" << i << std::endl;
      } else {
        // std::cout << "pop ok:" << i << std::endl;
      }
      if (i == TEST_NUM - 1) {
        break;
      }
    }
  });

  write_th.join();
  read_th.join();
  end = clock();

  std::cout << "ring queue运行时间：" << (end - start) / 1000 << "ms"
            << std::endl;
}

void TestLockQueue() {
  start = clock();
  auto queue_ptr = std::make_shared<LockQueue<int>>();
  auto write_th = std::thread([queue_ptr] {
    for (int i = 0; i < TEST_NUM; i++) {
      while (!queue_ptr->Push(i))
        ;
    }
  });

  auto read_th = std::thread([queue_ptr] {
    while (1) {
      int i = 0;
      if (!queue_ptr->Pop(i)) {
        // std::cout << "pop fail:" << i << std::endl;
      } else {
        // std::cout << "pop ok:" << i << std::endl;
      }
      if (i == TEST_NUM - 1) {
        break;
      }
    }
  });

  write_th.join();
  read_th.join();
  end = clock();

  std::cout << "lock queue运行时间：" << (end - start) / 1000 << "ms"
            << std::endl;
}
