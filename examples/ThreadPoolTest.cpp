#include <iostream>

#include "util/threadpool/ThreadPool.h"

int Sum(int a, int b) { return a + b; }

int main() {
  ThreadPool threadpool;
  threadpool.Start(2);

  threadpool.Push([] {
    std::cout << std::this_thread::get_id() << " hello word 1" << std::endl;
  });
  threadpool.Push([] {
    std::cout << std::this_thread::get_id() << " hello word 2" << std::endl;
  });
  threadpool.Push([] {
    std::cout << std::this_thread::get_id() << " hello word 3" << std::endl;
  });

  auto ret = threadpool.Push(Sum, 1, 2);
  std::cout << "sum:" << ret.get() << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(2));
  threadpool.Stop();
  return 0;
}