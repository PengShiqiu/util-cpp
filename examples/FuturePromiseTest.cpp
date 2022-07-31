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

  std::shared_ptr<FutureSchedule> sche = std::make_shared<FutureSchedule>(tp, timer);
  
  std::shared_ptr<ananas::Promise<int>> promise(new ananas::Promise<int>());
  std::shared_ptr<ananas::Promise<int>> promise_excep(new ananas::Promise<int>());

  timer->ScheduleAfter(std::chrono::milliseconds(2000), [promise_excep, promise] {
    if (!promise->IsReady()) {
      PRINT("executor 1.\n");
      promise->SetValue(1);
    }
    if (!promise_excep->IsReady()) {
      try {
        throw std::runtime_error("ThreadFunc exception");
      } catch(...) {
        promise_excep->SetException(std::current_exception());
      }
    }
  });
  promise->GetFuture().Then(sche.get(), [](ananas::Try<int>&& ret){
    try {
      int value = ret;
      PRINT("executor ret:%d.\n", value);
    } catch (std::exception& err) {
      PRINT("Then got exception:%s\n",err. what());
    }
  })
  .OnTimeout(std::chrono::milliseconds(1000), []{
    PRINT("timeout\n");
  }, sche.get());

  promise_excep->GetFuture().Then(sche.get(),[](ananas::Try<int>&& ret){
    try {
      int value = ret;
      PRINT("executor ret:%d.\n", value);
    } catch (std::exception& err) {
      PRINT("Then got exception:%s\n",err. what());
    }
  })
  .OnTimeout(std::chrono::milliseconds(1000), []{
    PRINT("timeout\n");
  }, sche.get());

  while (true) {
    timer->Update();
  }

  return 0;
}

// TODO: 未实现内容
//  1 - 线程池JoinAll所有线程推出