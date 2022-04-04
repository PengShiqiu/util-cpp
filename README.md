#   C++基础组件

                    
## 1.无锁队列-环形队列实现

------------

- src/Queue/RingQueue.hpp

编译测试代码：`cd tests && make`

执行结果：`./test_main `
```
ring queue运行时间：134ms
lock queue运行时间：1585ms
```

## 2.线程池
使用c++11开发的通用线程池
- 支持可变参数传入
- 支持函数返回值

#### 使用示例　
        ThreadPool threadpool;
        threadpool.Start(2);

        threadpool.Push([]
        {
            std::cout << std::this_thread::get_id() << " hello word 1" << std::endl;
        });
        threadpool.Push([]
        {
            std::cout << std::this_thread::get_id() << " hello word 2" << std::endl;
        });
        threadpool.Push([]
        {
            std::cout << std::this_thread::get_id() << " hello word 3" << std::endl;
        });

        auto ret = threadpool.Push(Sum, 1, 2);
        std::cout << "sum:" << ret.get() << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));
        threadpool.Stop();
