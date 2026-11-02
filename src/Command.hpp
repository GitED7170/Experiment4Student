#pragma once

#include <functional>

#include "PoseHandler.hpp"

namespace adas
{
// 删除继承多态：命令对象改为持有一个可调用对象（std::function + Lambda）

// 定义一个命令类 MoveCommand，完成 Move 动作（M 指令）
class MoveCommand final
{
public:
    // 定义函数对象 operate，接收参数 PoseHandler，返回 void
    const std::function<void(PoseHandler&)> operate = [](PoseHandler& poseHandler) noexcept {
        if (poseHandler.IsFast())
        {
            poseHandler.Move();  // 如果是F状态，多执行一次MOVE
        }
        poseHandler.Move();      // 通过状态管理器完成 Move 动作
    };
};

// 定义一个命令类 TurnLeftCommand，完成 TurnLeft 动作（L 指令）
class TurnLeftCommand final
{
public:
    const std::function<void(PoseHandler&)> operate = [](PoseHandler& poseHandler) noexcept {
        // 加速状态下先前进一格，再左转
        if (poseHandler.IsFast())
        {
            poseHandler.Move();
        }
        poseHandler.TurnLeft();
    };
};

// 定义一个命令类 TurnRightCommand，完成 TurnRight 动作（R 指令）
class TurnRightCommand final
{
public:
    const std::function<void(PoseHandler&)> operate = [](PoseHandler& poseHandler) noexcept {
        // 加速状态下先前进一格，再右转
        if (poseHandler.IsFast())
        {
            poseHandler.Move();
        }
        poseHandler.TurnRight();
    };
};

// 定义一个命令类 FastCommand，完成加速状态切换（F 指令）
class FastCommand final
{
public:
    const std::function<void(PoseHandler&)> operate = [](PoseHandler& poseHandler) noexcept {
        poseHandler.Fast();
    };
};
}  // namespace adas
