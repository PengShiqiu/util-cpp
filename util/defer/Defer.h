//copy from ananas
#pragma once
#include <functional>

namespace inner{
    
class ExecuteOnScopeExit {
public:
    ExecuteOnScopeExit() = default;

    // movable
    ExecuteOnScopeExit(ExecuteOnScopeExit&& ) = default;
    ExecuteOnScopeExit& operator=(ExecuteOnScopeExit&& ) = default;

    // non copyable
    ExecuteOnScopeExit(const ExecuteOnScopeExit& e) = delete;
    void operator=(const ExecuteOnScopeExit& f) = delete;

    template <typename F, typename... Args>
    ExecuteOnScopeExit(F&& f, Args&&... args) {
        func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    }

    ~ExecuteOnScopeExit() noexcept {
        if (func_) func_();
    }

private:
    std::function<void ()> func_;
};
}

#define _CONCAT(a, b) a##b
#define _MAKE_DEFER_(line) inner::ExecuteOnScopeExit _CONCAT(defer, line) = [&]()
#undef ANANAS_DEFER
#define ANANAS_DEFER _MAKE_DEFER_(__LINE__)
