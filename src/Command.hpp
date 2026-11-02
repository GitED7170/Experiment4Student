#pragma once

#include "ActionGroup.hpp"
#include "PoseHandler.hpp"

namespace adas
{
// 命令对象负责“编排”：根据执行器状态产生一组原子动作

// 定义一个命令类 MoveCommand，完成 Move 动作（M 指令）
class MoveCommand final
{
public:
    ActionGroup operator()(PoseHandler& poseHandler) const noexcept
    {
        ActionGroup actionGroup;

        // 倒车状态决定前进还是后退
        const auto action = poseHandler.IsReverse() ? ActionType::BACKWARD_1_STEP_ACTION
                                                    : ActionType::FORWARD_1_STEP_ACTION;

        // 加速状态下额外执行一次
        if (poseHandler.IsFast())
        {
            actionGroup.PushAction(action);
        }
        // 无论是否加速，这一步必须执行
        actionGroup.PushAction(action);

        return actionGroup;
    }
};

// 定义一个命令类 TurnLeftCommand，完成 TurnLeft 动作（L 指令）
class TurnLeftCommand final
{
public:
    ActionGroup operator()(PoseHandler& poseHandler) const noexcept
    {
        ActionGroup actionGroup;

        // 加速状态下先移动一格
        if (poseHandler.IsFast())
        {
            actionGroup.PushAction(poseHandler.IsReverse() ? ActionType::BACKWARD_1_STEP_ACTION
                                                          : ActionType::FORWARD_1_STEP_ACTION);
        }
        // 倒车状态下左右互换
        actionGroup.PushAction(poseHandler.IsReverse() ? ActionType::REVERSE_TURNLEFT_ACTION
                                                      : ActionType::TURNLEFT_ACTION);

        return actionGroup;
    }
};

// 定义一个命令类 TurnRightCommand，完成 TurnRight 动作（R 指令）
class TurnRightCommand final
{
public:
    ActionGroup operator()(PoseHandler& poseHandler) const noexcept
    {
        ActionGroup actionGroup;

        // 加速状态下先移动一格
        if (poseHandler.IsFast())
        {
            actionGroup.PushAction(poseHandler.IsReverse() ? ActionType::BACKWARD_1_STEP_ACTION
                                                          : ActionType::FORWARD_1_STEP_ACTION);
        }
        // 倒车状态下左右互换
        actionGroup.PushAction(poseHandler.IsReverse() ? ActionType::REVERSE_TURNRIGHT_ACTION
                                                      : ActionType::TURNRIGHT_ACTION);

        return actionGroup;
    }
};

// 定义一个命令类 FastCommand，完成加速状态切换（F 指令）
class FastCommand final
{
public:
    ActionGroup operator()(PoseHandler& poseHandler) const noexcept
    {
        (void)poseHandler;
        return ActionGroup({ActionType::BE_FAST_ACTION});
    }
};

// 定义一个命令类 ReverseCommand，完成倒车状态切换（B 指令）
class ReverseCommand final
{
public:
    ActionGroup operator()(PoseHandler& poseHandler) const noexcept
    {
        (void)poseHandler;
        return ActionGroup({ActionType::BE_REVERSE_ACTION});
    }
};
}  // namespace adas
