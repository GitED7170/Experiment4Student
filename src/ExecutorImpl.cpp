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

/* 切换加速状态：每收到一次 F 取反 */
void ExecutorImpl::Fast(void) noexcept
{
    fast = !fast;
}

bool ExecutorImpl::IsFast(void) const noexcept
{
    return fast;
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    for (const auto cmd : commands)
    {
        // 声明一个 ICommand 类型的智能指针
        std::unique_ptr<ICommand> cmder;

        if (cmd == 'M')
        {
            // 智能指针指向子类 MoveCommand 实例
            cmder = std::make_unique<MoveCommand>();
        }
        else if (cmd == 'L')
        {
            cmder = std::make_unique<TurnLeftCommand>();
        }
        else if (cmd == 'R')
        {
            cmder = std::make_unique<TurnRightCommand>();
        }
        else if (cmd == 'F')
        {
            cmder = std::make_unique<FastCommand>();
        }

        if (cmder)
        {
            // 多态：cmder 指向不同子类实例，调用不同的命令
            cmder->DoOperate(*this);
        }
    }
}

Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);  // 只在C++17下有效
}
}  // namespace adas
