#include "ExecutorImpl.hpp"

#include <memory>
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

/* 左转90度（逆时针），位置不变：E→N，W→S，N→W，S→E */
void ExecutorImpl::TurnLeft(void) noexcept
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
void ExecutorImpl::TurnRight(void) noexcept
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

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    for (const auto cmd : commands)
    {
        if (cmd == 'M')
        {
            // 智能指针指向 MoveCommand 实例，不用担心 delete
            std::unique_ptr<MoveCommand> cmder = std::make_unique<MoveCommand>();
            // *this 就是 ExecutorImpl 实例对象，作为实参传递给 DoOperate
            cmder->DoOperate(*this);  // 执行 MoveCommand 的 DoOperate，即 Move
        }
        else if (cmd == 'L')
        {
            TurnLeft();
        }
        else if (cmd == 'R')
        {
            TurnRight();
        }
    }
}

Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);  // 只在C++17下有效
}
}  // namespace adas
