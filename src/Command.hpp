#pragma once

#include "ExecutorImpl.hpp"

namespace adas
{
// 定义所有具体 Command 对象的抽象基类 ICommand
class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void DoOperate(ExecutorImpl& executor) const noexcept = 0;
};

// 定义一个嵌套类 MoveCommand，完成 Move 动作（M 指令）
class MoveCommand final : public ICommand
{
public:
    void DoOperate(ExecutorImpl& executor) const noexcept override
    {
        executor.Move();
        // 加速状态下多前进一格（仍然是一格一格移动）
        if (executor.IsFast())
        {
            executor.Move();
        }
    }
};

// 定义一个嵌套类 TurnLeftCommand，完成 TurnLeft 动作（L 指令）
class TurnLeftCommand final : public ICommand
{
public:
    void DoOperate(ExecutorImpl& executor) const noexcept override
    {
        // 加速状态下先前进一格，再左转
        if (executor.IsFast())
        {
            executor.Move();
        }
        executor.TurnLeft();
    }
};

// 定义一个嵌套类 TurnRightCommand，完成 TurnRight 动作（R 指令）
class TurnRightCommand final : public ICommand
{
public:
    void DoOperate(ExecutorImpl& executor) const noexcept override
    {
        // 加速状态下先前进一格，再右转
        if (executor.IsFast())
        {
            executor.Move();
        }
        executor.TurnRight();
    }
};

// 定义一个嵌套类 FastCommand，完成加速状态切换（F 指令）
class FastCommand final : public ICommand
{
public:
    void DoOperate(ExecutorImpl& executor) const noexcept override
    {
        executor.Fast();
    }
};
}  // namespace adas
