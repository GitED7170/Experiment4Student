#include "ExecutorImpl.hpp"

#include <new>

namespace adas
{
// 注意：本提交故意未在初始化列表中初始化pose，用于演示“未初始化导致用例失败”
ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept
{
}

Pose ExecutorImpl::Query(void) const noexcept
{
    return Pose{};
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
}

Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);  // 只在C++17下有效
}
}  // namespace adas
