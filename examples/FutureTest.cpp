#include "util/future/Future.h"
#include "util/threadpool/ThreadPool.h"
#include "util/timer/Timer.h"

class FutureSchedule : public ananas::Scheduler {
 public:
  FutureSchedule(std::shared_ptr<ThreadPool> threadpool,
                 std::shared_ptr<ananas::TimerManager> timer)
      : threadpool_(threadpool), timer_(timer) {}
  virtual ~FutureSchedule() {}
  virtual void ScheduleLater(std::chrono::milliseconds duration,
                             std::function<void()> f) override {
    timer_->ScheduleAfter(duration, f);
  }
  virtual void Schedule(std::function<void()> f) override {
    threadpool_->Push(f);
  }

 private:
  std::shared_ptr<ThreadPool> threadpool_;
  std::shared_ptr<ananas::TimerManager> timer_;
};

int main(void) {
  std::shared_ptr<ananas::TimerManager> timer =
      std::make_shared<ananas::TimerManager>();
  std::shared_ptr<ThreadPool> tp = std::make_shared<ThreadPool>();
  tp->Start(2);

  std::shared_ptr<FutureSchedule> sche =
      std::make_shared<FutureSchedule>(tp, timer);

  auto future = tp->Push([] {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    printf("executor 1.\n");
  });

  future.Then([] { printf("executor 2.\n"); })
      .Then(sche.get(), [] { printf("executor 3.\n"); })
      .Then([] { printf("executor 4.\n"); })
      .Then([] { printf("executor 5.\n"); })
      .OnTimeout(std::chrono::seconds(1), [] {
            printf("OnTimeout.\n");
          }, sche.get());

  while (true) {
    timer->Update();
  }

  return 0;
}

// TODO: 未实现内容
//  1 - 线程池JoinAll所有线程推出