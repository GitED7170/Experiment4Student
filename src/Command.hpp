#pragma once

#include "PoseHandler.hpp"

namespace adas
{
// 用 operator() 重载替代 std::function 成员：
// 命令对象只暴露行为，不暴露数据，恢复封装性

// 定义一个命令类 MoveCommand，完成 Move 动作（M 指令）
class MoveCommand final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        if (poseHandler.IsFast())
        {
            poseHandler.Move();  // 如果是F状态，多执行一次MOVE
        }
        poseHandler.Move();      // 通过状态管理器完成 Move 动作
    }
};

// 定义一个命令类 TurnLeftCommand，完成 TurnLeft 动作（L 指令）
class TurnLeftCommand final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        // 加速状态下先前进一格，再左转
        if (poseHandler.IsFast())
        {
            poseHandler.Move();
        }
        poseHandler.TurnLeft();
    }
};

// 定义一个命令类 TurnRightCommand，完成 TurnRight 动作（R 指令）
class TurnRightCommand final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        // 加速状态下先前进一格，再右转
        if (poseHandler.IsFast())
        {
            poseHandler.Move();
        }
        poseHandler.TurnRight();
    }
};

// 定义一个命令类 FastCommand，完成加速状态切换（F 指令）
class FastCommand final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.Fast();
    }
};
}  // namespace adas
