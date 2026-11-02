#pragma once

#include "PoseHandler.hpp"

namespace adas
{
// 命令对象只暴露行为，不暴露数据

// 定义一个命令类 MoveCommand，完成 Move 动作（M 指令）
class MoveCommand final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        // 加速状态下执行两次移动（F 与 B 叠加时为两次后退）
        if (poseHandler.IsFast())
        {
            if (poseHandler.IsReverse())
            {
                poseHandler.Backward();
            }
            else
            {
                poseHandler.Forward();
            }
        }

        if (poseHandler.IsReverse())
        {
            poseHandler.Backward();
        }
        else
        {
            poseHandler.Forward();
        }
    }
};

// 定义一个命令类 TurnLeftCommand，完成 TurnLeft 动作（L 指令）
class TurnLeftCommand final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        // 加速状态下先移动一格（B 状态时是后退）
        if (poseHandler.IsFast())
        {
            if (poseHandler.IsReverse())
            {
                poseHandler.Backward();
            }
            else
            {
                poseHandler.Forward();
            }
        }

        // 倒车状态下左右互换
        if (poseHandler.IsReverse())
        {
            poseHandler.TurnRight();
        }
        else
        {
            poseHandler.TurnLeft();
        }
    }
};

// 定义一个命令类 TurnRightCommand，完成 TurnRight 动作（R 指令）
class TurnRightCommand final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        // 加速状态下先移动一格（B 状态时是后退）
        if (poseHandler.IsFast())
        {
            if (poseHandler.IsReverse())
            {
                poseHandler.Backward();
            }
            else
            {
                poseHandler.Forward();
            }
        }

        // 倒车状态下左右互换
        if (poseHandler.IsReverse())
        {
            poseHandler.TurnLeft();
        }
        else
        {
            poseHandler.TurnRight();
        }
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

// 定义一个命令类 ReverseCommand，完成倒车状态切换（B 指令）
class ReverseCommand final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.Reverse();  // 切换倒车状态
    }
};
}  // namespace adas
