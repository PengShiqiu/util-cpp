/**
 * @file ThreadPool.h
 * @brief 线程池
 * @details 可传参数及获取返回值
 * @author PengShiqiu
 * @version v1.0
 * @date 2022-03-15
 */
#pragma once
#ifndef __CXX11_THREAD_POOL_H__
#define __CXX11_THREAD_POOL_H__

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>
#include "util/future/Future.h"

/**
 * @brief 线程池
 * 基于C++11特性的线程池类 1)、支持可变参数模块
 * 2)、支持std::future<RetType>返回值类型
 */
class ThreadPool final {
 public:
  ThreadPool();
  ~ThreadPool();

  /**
   * @brief Push执行函数
   * @details 可使用std::bind、lambda
   *
   * @param f 执行仿函数模块
   * @param args 可变参数模板
   * @return ananas::Future<T> 执行函数返回Future数据，使用Future模型
   */
    template <typename F, typename... Args,
              typename = typename std::enable_if<!std::is_void<typename std::result_of<F (Args...)>::type>::value, void>::type,
              typename Dummy = void>
    auto Push(F&& f, Args&&... args) -> ananas::Future<typename std::result_of<F (Args...)>::type>;

    template <typename F, typename... Args,
              typename = typename std::enable_if<std::is_void<typename std::result_of<F (Args...)>::type>::value, void>::type>
    auto Push(F&& f, Args&&... args) -> ananas::Future<void>;

  /**
   * @brief 启动线程池
   * @param max_thread_num 最大线程数量
   * @return void
   */
  void Start(int max_thread_num);

  /**
   * @brief 停止线程池
   * @return void
   */
  void Stop();

 private:
  using Task = std::function<void()>;

  std::vector<std::thread> threads_; /**< 线程池 */
  std::queue<Task> task_que_;        /**< 任务函数队列 */
  std::condition_variable task_cv_;  /**< 任务条件变量 */
  std::mutex task_mutex_;            /**< 任务函数队列锁 */
  std::atomic<bool> run_;            /**< 线程池运行标志 */
};


// if F return something
template <typename F, typename... Args, typename, typename >
auto ThreadPool::Push(F&& f, Args&&... args) -> ananas::Future<typename std::result_of<F (Args...)>::type> {
    using resultType = typename std::result_of<F (Args...)>::type;

    std::unique_lock<std::mutex> guard(task_mutex_);
    if (!run_)
        throw std::runtime_error("thread pool stoped.");

    ananas::Promise<resultType> promise;
    auto future = promise.GetFuture();

    auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    auto task = [t = std::move(func), pm = std::move(promise)]() mutable {
        try {
            pm.SetValue(ananas::Try<resultType>(t()));
        } catch(...) {
            pm.SetException(std::current_exception());
        }
    };

    task_que_.emplace(std::move(task));
    task_cv_.notify_one();

    return future;
}

// F return void
template <typename F, typename... Args, typename >
auto ThreadPool::Push(F&& f, Args&&... args) -> ananas::Future<void> {
    using resultType = typename std::result_of<F (Args...)>::type;
    static_assert(std::is_void<resultType>::value, "must be void");

    std::unique_lock<std::mutex> guard(task_mutex_);
    if (!run_)
        throw std::runtime_error("thread pool stoped.");

    ananas::Promise<resultType> promise;
    auto future = promise.GetFuture();

    auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    auto task = [t = std::move(func), pm = std::move(promise)]() mutable {
        try {
            t();
            pm.SetValue();
        } catch(...) {
            pm.SetException(std::current_exception());
        }
    };

    task_que_.emplace(std::move(task));
    task_cv_.notify_one();
    return future;
}

#endif