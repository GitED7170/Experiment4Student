#include "ExecutorImpl.hpp"

#include <new>

namespace adas
{
ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept : pose(pose)
{
}

Pose ExecutorImpl::Query(void) const noexcept
{
    return pose;
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
}

Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);  // 只在C++17下有效
}
}  // namespace adas
