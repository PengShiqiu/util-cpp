#pragma once
#ifndef __QUEUE_RING_QUEUE_HPP_H__
#define __QUEUE_RING_QUEUE_HPP_H__

#include <vector>

template<class T, size_t Size>
class RingQueue
{
public:
    RingQueue():queue_(Size)
    {
        read_ = 0;
        write_ = 0;
    }
    ~RingQueue()
    {

    }

    bool Push(const T& data)
    {
        if(!queue_[write_].read_flag)
        {
            queue_[write_].data = data;
            queue_[write_].read_flag = true;
            write_ = (++write_) % Size;
            return true;
        }
        return false;
    }

    bool Pop(T& data)
    {
        if(queue_[read_].read_flag)
        {
            data = queue_[read_].data;
            queue_[read_].read_flag = false;
            read_ = (++read_) % Size;
            return true;
        }
        return false;
    }
    
private:
    struct Node
    {
        T data;
        bool read_flag;
    };
    std::vector<Node> queue_;
    size_t read_;
    size_t write_;
};

#endif