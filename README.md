#   C++基础组件
基础组件
- 线程池 [util/threadpool] 
- 无锁环形队列 [util/queue/RingQueue]
- Promise/Future并发模型 [util/future]
- 定时器 - 未实现
  - 红黑树:适用于单线程环境，锁的粒度较大。
  - 最小堆：插入效率高于红黑树
  - 跳表：适合多线程环境
  - 时间轮：时间及空间复杂度都是最优、
- 内存池 - 未实现
- 步请求池 - 未实现
- 协程框架 - 未实现
- 用户态协议栈 - 未实现
- 死锁检测 - 未实现
- 内存泄漏 - 未实现

数据结构
-  跳表 [util/skiplist]
                    
## 1.无锁队列-环形队列实现

------------
```
src/Queue/RingQueue.hpp
```
编译测试代码：`cd tests && make`

执行结果：`./test_main `
```
ring queue运行时间：134ms
lock queue运行时间：1585ms
```
------------
## 2.线程池
```
src/ThreadPool/ThreadPool.hpp
src/ThreadPool/ThreadPool.cpp
```
使用c++11开发的通用线程池
- 支持可变参数传入
- 支持函数返回值

## 3. Promise/Future并发模型
ref: https://github.com/PengShiqiu/ananas