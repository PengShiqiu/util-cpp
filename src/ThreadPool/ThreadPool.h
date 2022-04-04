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

#include <vector>
#include <queue>
#include <atomic>
#include <future>
#include <condition_variable>
#include <thread>
#include <functional>

/**
 * @brief 线程池
 * 基于C++11特性的线程池类 1)、支持可变参数模块 2)、支持std::future<RetType>返回值类型
 */
class ThreadPool final
{
public:
    ThreadPool();
    ~ThreadPool();
    
    /**
     * @brief Push执行函数
     * @details 可使用std::bind、lambda
     *
     * @param f 执行仿函数模块
     * @param args 可变参数模板
     * @return std::future<T> 执行函数返回数据
    */
    template <class F, class... Args>
    auto Push(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        if (!run_)
        {
            throw std::runtime_error("thread pool stoped.");
        }

        //推导函数返回数据类型
        using RetType = decltype(f(std::forward<Args>(args)...));

        //创建packaged_task对象，使用bind将函数转换为task()
        auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        //获取函数返回值std::future<RetType>
        auto ret = task->get_future();
        {
            std::lock_guard<std::mutex> auto_lock(task_mutex_);
            // 执行函数加入队列
            task_que_.emplace([task]
            { 
                (*task)(); 
            });
        }

        // 通知单个线程
        task_cv_.notify_one();

        return ret;
    }

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

    std::vector<std::thread>    threads_;       /**< 线程池 */
    std::queue<Task>            task_que_;      /**< 任务函数队列 */
    std::condition_variable     task_cv_;       /**< 任务条件变量 */
    std::mutex                  task_mutex_;    /**< 任务函数队列锁 */
    std::atomic<bool>           run_;           /**< 线程池运行标志 */
};

#endif