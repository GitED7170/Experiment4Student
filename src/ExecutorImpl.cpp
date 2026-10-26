#include "ExecutorImpl.hpp"

#include <new>

namespace adas
{
namespace
{
/* 沿当前朝向前进一格：E=X+1，W=X-1，N=Y+1，S=Y-1 */
void Move(Pose& pose) noexcept
{
    switch (pose.heading)
    {
        case 'E':
            ++pose.x;
            break;
        case 'W':
            --pose.x;
            break;
        case 'N':
            ++pose.y;
            break;
        case 'S':
            --pose.y;
            break;
        default:
            break;
    }
}
}  // namespace

ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept : pose(pose)
{
}

Pose ExecutorImpl::Query(void) const noexcept
{
    return pose;
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    for (const auto cmd : commands)
    {
        switch (cmd)
        {
            case 'M':
                Move(pose);
                break;
            default:
                break;
        }
    }
}

Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);  // 只在C++17下有效
}
}  // namespace adas
