#pragma once

#include "cmder/NormalOrchestrator.hpp"

namespace adas
{
/* 公交：占用两格，坐标取车头；L/R 先前进一格再转向 */
class BusOrchestrator final : public NormalOrchestrator
{
public:
    ActionGroup TurnLeft(const PoseHandler& poseHandler) const noexcept override
    {
        ActionGroup actionGroup;
        actionGroup += OnFast(poseHandler);
        actionGroup.PushAction(GetStepAction(poseHandler));  // 先前进一格
        actionGroup.PushAction(poseHandler.IsReverse() ? ActionType::REVERSE_TURNLEFT_ACTION
                                                       : ActionType::TURNLEFT_ACTION);
        return actionGroup;
    }

    ActionGroup TurnRight(const PoseHandler& poseHandler) const noexcept override
    {
        ActionGroup actionGroup;
        actionGroup += OnFast(poseHandler);
        actionGroup.PushAction(GetStepAction(poseHandler));  // 先前进一格
        actionGroup.PushAction(poseHandler.IsReverse() ? ActionType::REVERSE_TURNRIGHT_ACTION
                                                       : ActionType::TURNRIGHT_ACTION);
        return actionGroup;
    }
};
}  // namespace adas
