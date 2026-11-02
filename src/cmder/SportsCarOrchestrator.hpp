#pragma once

#include "cmder/NormalOrchestrator.hpp"

namespace adas
{
/* 跑车：M 前进两格；L/R 转向后再前进一格 */
class SportsCarOrchestrator final : public NormalOrchestrator
{
public:
    ActionGroup Move(const PoseHandler& poseHandler) const noexcept override
    {
        ActionGroup actionGroup;
        const auto step = GetStepAction(poseHandler);

        // 加速状态下额外执行两次
        if (poseHandler.IsFast())
        {
            actionGroup.PushAction(step);
            actionGroup.PushAction(step);
        }
        // 正常两步
        actionGroup.PushAction(step);
        actionGroup.PushAction(step);
        return actionGroup;
    }

    ActionGroup TurnLeft(const PoseHandler& poseHandler) const noexcept override
    {
        ActionGroup actionGroup;
        actionGroup += OnFast(poseHandler);
        actionGroup.PushAction(poseHandler.IsReverse() ? ActionType::REVERSE_TURNLEFT_ACTION
                                                       : ActionType::TURNLEFT_ACTION);
        actionGroup.PushAction(GetStepAction(poseHandler));  // 转向后再前进一格
        return actionGroup;
    }

    ActionGroup TurnRight(const PoseHandler& poseHandler) const noexcept override
    {
        ActionGroup actionGroup;
        actionGroup += OnFast(poseHandler);
        actionGroup.PushAction(poseHandler.IsReverse() ? ActionType::REVERSE_TURNRIGHT_ACTION
                                                       : ActionType::TURNRIGHT_ACTION);
        actionGroup.PushAction(GetStepAction(poseHandler));  // 转向后再前进一格
        return actionGroup;
    }
};
}  // namespace adas
