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