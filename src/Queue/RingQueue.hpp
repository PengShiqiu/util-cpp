
/**
 * @file RingQueue.h
 * @brief 无锁环形队列
 * @details /
 * @author PengShiqiu
 * @version v1.0
 * @date 2022-03-20
 */

#pragma once
#ifndef __QUEUE_RING_QUEUE_HPP_H__
#define __QUEUE_RING_QUEUE_HPP_H__

#include <vector>

/**
 * @brief 环形队列
 * @param T 队列数据类型
 * @param Size 队列总大小
 */
template <class T, size_t Size>
class RingQueue {
 public:
  RingQueue() : queue_(Size) {
    read_ = 0;
    write_ = 0;
  }
  ~RingQueue() {}
  /**
   * @brief 数据入队列
   * @param data 数据
   * @return 是否成功
   */
  bool Push(const T& data) {
    if (!queue_[write_].read_flag) {
      queue_[write_].data = data;
      queue_[write_].read_flag = true;
      write_ = (++write_) % Size;
      return true;
    }
    return false;
  }

  /**
   * @brief 数据出队列
   * @param data 数据
   * @return 是否成功
   */
  bool Pop(T& data) {
    if (queue_[read_].read_flag) {
      data = queue_[read_].data;
      queue_[read_].read_flag = false;
      read_ = (++read_) % Size;
      return true;
    }
    return false;
  }

 private:
  struct Node {
    Node() : read_flag(false) {}
    T data;         /**< 数据 */
    bool read_flag; /**< 可读标志 */
  };
  std::vector<Node> queue_; /**< 队列 */
  size_t read_;             /**< 读队列索引 */
  size_t write_;            /**< 写队列索引 */
};

#endif