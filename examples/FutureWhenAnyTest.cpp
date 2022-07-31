#include "util/future/Future.h"
#include "util/threadpool/ThreadPool.h"
#include "util/timer/Timer.h"

#define PRINT(t, arg...) printf("%s:%d " t, __FILE__, __LINE__, ##arg)

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
  std::shared_ptr<ananas::TimerManager> timer =std::make_shared<ananas::TimerManager>();
  std::shared_ptr<ThreadPool> tp = std::make_shared<ThreadPool>();
  tp->Start(2);

  std::vector<ananas::Future<int>> futures;
  for(int i = 0; i < 50; ++i){
    auto&& ft = tp->Push([](int i){return i;}, i);
    futures.emplace_back(std::move(ft));
  }
  ananas::WhenAny(std::begin(futures), std::end(futures))
  .Then([](const std::pair<size_t, ananas::Try<int>>& result){
    PRINT("ret : %lu %d\n", result.first, result.second.Value());
  });
  while(1){
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}

// TODO: 未实现内容
//  1 - 线程池JoinAll所有线程推出