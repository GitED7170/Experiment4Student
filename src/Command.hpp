#pragma once

#include "PoseHandler.hpp"

namespace adas
{
// 定义所有具体 Command 对象的抽象基类 ICommand
class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void DoOperate(PoseHandler& poseHandler) const noexcept = 0;
};

// 定义一个嵌套类 MoveCommand，完成 Move 动作（M 指令）
class MoveCommand final : public ICommand
{
public:
    void DoOperate(PoseHandler& poseHandler) const noexcept override
    {
        if (poseHandler.IsFast())
        {
            poseHandler.Move();  // 如果是F状态，多执行一次MOVE
        }
        poseHandler.Move();      // 通过状态管理器完成 Move 动作
    }
};

// 定义一个嵌套类 TurnLeftCommand，完成 TurnLeft 动作（L 指令）
class TurnLeftCommand final : public ICommand
{
public:
    void DoOperate(PoseHandler& poseHandler) const noexcept override
    {
        // 加速状态下先前进一格，再左转
        if (poseHandler.IsFast())
        {
            poseHandler.Move();
        }
        poseHandler.TurnLeft();
    }
};

// 定义一个嵌套类 TurnRightCommand，完成 TurnRight 动作（R 指令）
class TurnRightCommand final : public ICommand
{
public:
    void DoOperate(PoseHandler& poseHandler) const noexcept override
    {
        // 加速状态下先前进一格，再右转
        if (poseHandler.IsFast())
        {
            poseHandler.Move();
        }
        poseHandler.TurnRight();
    }
};

// 定义一个嵌套类 FastCommand，完成加速状态切换（F 指令）
class FastCommand final : public ICommand
{
public:
    void DoOperate(PoseHandler& poseHandler) const noexcept override
    {
        poseHandler.Fast();
    }
};
}  // namespace adas
