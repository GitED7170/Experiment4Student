#pragma once

#include "cmder/CmderOrchestrator.hpp"

namespace adas
{
/* 正常车型的编排：M 前进一格，L/R 原地转向，TR 掉头 */
class NormalOrchestrator : public CmderOrchestrator
{
public:
    ActionGroup Move(const PoseHandler& poseHandler) const noexcept override
    {
        ActionGroup actionGroup;
        actionGroup += OnFast(poseHandler);                 // 加速状态先移动一步
        actionGroup.PushAction(GetStepAction(poseHandler));  // 这一步必须走
        return actionGroup;
    }

    ActionGroup TurnLeft(const PoseHandler& poseHandler) const noexcept override
    {
        ActionGroup actionGroup;
        actionGroup += OnFast(poseHandler);
        actionGroup.PushAction(poseHandler.IsReverse() ? ActionType::REVERSE_TURNLEFT_ACTION
                                                       : ActionType::TURNLEFT_ACTION);
        return actionGroup;
    }

    ActionGroup TurnRight(const PoseHandler& poseHandler) const noexcept override
    {
        ActionGroup actionGroup;
        actionGroup += OnFast(poseHandler);
        actionGroup.PushAction(poseHandler.IsReverse() ? ActionType::REVERSE_TURNRIGHT_ACTION
                                                       : ActionType::TURNRIGHT_ACTION);
        return actionGroup;
    }

    ActionGroup TurnRound(const PoseHandler& poseHandler) const noexcept override
    {
        // 倒车状态下，掉头指令被忽略
        if (poseHandler.IsReverse())
        {
            return ActionGroup();
        }
        if (poseHandler.IsFast())
        {
            return ActionGroup({
                ActionType::FORWARD_1_STEP_ACTION,
                ActionType::TURNLEFT_ACTION,
                ActionType::FORWARD_1_STEP_ACTION,
                ActionType::TURNLEFT_ACTION,
            });
        }
        return ActionGroup({
            ActionType::TURNLEFT_ACTION,
            ActionType::FORWARD_1_STEP_ACTION,
            ActionType::TURNLEFT_ACTION,
        });
    }

protected:
    ActionType GetStepAction(const PoseHandler& poseHandler) const noexcept
    {
        return poseHandler.IsReverse() ? ActionType::BACKWARD_1_STEP_ACTION : ActionType::FORWARD_1_STEP_ACTION;
    }

    ActionGroup OnFast(const PoseHandler& poseHandler) const noexcept
    {
        if (poseHandler.IsFast())
        {
            return ActionGroup({GetStepAction(poseHandler)});
        }
        return ActionGroup();
    }
};
}  // namespace adas
