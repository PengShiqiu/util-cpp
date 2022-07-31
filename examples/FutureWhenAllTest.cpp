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

  auto future1 = tp->Push([]{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 1;
  });  
  auto future2 = tp->Push([]{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return std::string("hello");
  });  
  ananas::WhenAll(future1, future2)
  .Then([](const std::tuple<ananas::Try<int>, ananas::Try<std::string>>& results){
    PRINT("result 0 : %d\n", std::get<0>(results).Value());
    PRINT("result 1 : %s\n", std::get<1>(results).Value().c_str());
  });

  std::vector<ananas::Future<int>> futures;
  for(int i = 0; i < 50; ++i){
    auto&& ft = tp->Push([](int i){return i;}, i);
    futures.emplace_back(std::move(ft));
  }
  ananas::WhenAll(std::begin(futures), std::end(futures))
  .Then([](const std::vector<ananas::Try<int>>& results){
    for (auto& t : results){
      PRINT("ret : %d \n", t.Value());
    }
  });
  while(1){
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}

// TODO: 未实现内容
//  1 - 线程池JoinAll所有线程推出