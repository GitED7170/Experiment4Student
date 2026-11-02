#include "ExecutorImpl.hpp"

#include <new>

namespace adas
{
namespace
{
/* 左转90度（逆时针），位置不变：E→N，W→S，N→W，S→E */
void TurnLeft(Pose& pose) noexcept
{
    switch (pose.heading)
    {
        case 'E':
            pose.heading = 'N';
            break;
        case 'W':
            pose.heading = 'S';
            break;
        case 'N':
            pose.heading = 'W';
            break;
        case 'S':
            pose.heading = 'E';
            break;
        default:
            break;
    }
}

/* 右转90度（顺时针），位置不变：E→S，W→N，N→E，S→W */
void TurnRight(Pose& pose) noexcept
{
    switch (pose.heading)
    {
        case 'E':
            pose.heading = 'S';
            break;
        case 'W':
            pose.heading = 'N';
            break;
        case 'N':
            pose.heading = 'E';
            break;
        case 'S':
            pose.heading = 'W';
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

/* 沿当前朝向前进一格：E=X+1，W=X-1，N=Y+1，S=Y-1 */
void ExecutorImpl::Move(void) noexcept
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

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    for (const auto cmd : commands)
    {
        switch (cmd)
        {
            case 'M':
                Move();
                break;
            case 'L':
                TurnLeft(pose);
                break;
            case 'R':
                TurnRight(pose);
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
