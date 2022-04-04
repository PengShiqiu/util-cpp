/**
* @file ThreadPool.cpp
* @brief 线程池
* @details 可传参数及获取返回值
* @author PengShiqiu
* @version v1.0
* @date 2022-03-15
*/

#include "ThreadPool.h"

ThreadPool::ThreadPool()
{

}

ThreadPool::~ThreadPool()
{
    Stop();
}

void ThreadPool::Start(int max_thread_num)
{
    for (int i = 0; i < max_thread_num; i++)
    {
        // 创建线程并存放在threads_中
        threads_.emplace_back(std::thread([this]
        {
            while (run_)
            {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(task_mutex_);

                    // 等待执行任务
                    task_cv_.wait(lock, [this]
                    {
                        return !run_ || !task_que_.empty();
                    });

                    // 线程池stop时退出线程
                    if(!run_ && task_que_.empty())
                    {
                        return;
                    }

                    // 获取任务执行函数 使用move减少拷贝
                    task = std::move(task_que_.front());

                    // 执行函数出队列
                    task_que_.pop();
                }

                // 执行任务函数
                task();
            } 
        }));
    }

    run_ = true;
}

void ThreadPool::Stop()
{
    if(!run_)
    {
        return;
    }
    run_ = false;
    task_cv_.notify_all();
    for(auto& th : threads_)
    {
        if(th.joinable())
        {
            th.join();
        }
    }
}