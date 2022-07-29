#include "util/future/Future.h"
#include "util/threadpool/ThreadPool.h"

class FutureSchedule : public ananas::Scheduler {
 public:
  FutureSchedule(std::shared_ptr<ThreadPool> threadpool)
      : threadpool_(threadpool) {}
  virtual ~FutureSchedule() {}
  virtual void ScheduleLater(std::chrono::milliseconds duration,
                             std::function<void()> f) override {
    // TODO:定时器功能未实现
    // threadpool_->Push(f);
  }
  virtual void Schedule(std::function<void()> f) override {
    threadpool_->Push(f);
  }

 private:
  std::shared_ptr<ThreadPool> threadpool_;
};

int main(void) {
  std::shared_ptr<ThreadPool> tp = std::make_shared<ThreadPool>();
  tp->Start(2);

  std::shared_ptr<FutureSchedule> sche = std::make_shared<FutureSchedule>(tp);

  auto future = tp->Push([] { printf("executor 1.\n"); });

  future.Then([] { printf("executor 2.\n"); })
      .Then(sche.get(), [] { printf("executor 3.\n"); })
      .Then([] { printf("executor 4.\n"); })
      .Then([] { printf("executor 5.\n"); })
      .OnTimeout(
          std::chrono::seconds(1), [] {}, sche.get());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  tp->Stop();

  return 0;
}

// TODO: 未实现内容
//  1 - 线程池JoinAll所有线程推出
//  2 - 定时器 ... future OnTimeout需要依赖定时器