/**
* @file LockQueue.h
* @brief 简易队列
* @details 使用list容器缓存数据，使用独占锁保证互斥
* @author PengShiqiu
* @version v1.0
* @date 2022-03-20
*/

#pragma once
#ifndef __QUEUE_LOCK_QUEUE_HPP_H__
#define __QUEUE_LOCK_QUEUE_HPP_H__

#include <list>
#include <mutex>

template<class T>
class LockQueue
{
public:
    bool Push(const T& data)
    {
        std::lock_guard<std::mutex> auto_lock(mutex_);
        queue_.push_back(data);
        return true;
    }

    bool Pop(T& data)
    {
        std::lock_guard<std::mutex> auto_lock(mutex_);
        if(queue_.empty())
        {
            return false;
        }
        data = queue_.front();
        queue_.pop_front();
        return true;
    }
    
private:
    std::list<T> queue_;
    std::mutex mutex_;
};

#endif