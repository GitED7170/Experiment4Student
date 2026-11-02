#pragma once

#include "cmder/ActionGroup.hpp"
#include "cmder/CmderOrchestrator.hpp"
#include "core/PoseHandler.hpp"

namespace adas
{
// 命令对象负责把“指令”交给当前车型的编排器，得到一组原子动作

class MoveCommand final
{
public:
    ActionGroup operator()(const PoseHandler& poseHandler, const CmderOrchestrator& orchestrator) const noexcept
    {
        return orchestrator.Move(poseHandler);
    }
};

class TurnLeftCommand final
{
public:
    ActionGroup operator()(const PoseHandler& poseHandler, const CmderOrchestrator& orchestrator) const noexcept
    {
        return orchestrator.TurnLeft(poseHandler);
    }
};

class TurnRightCommand final
{
public:
    ActionGroup operator()(const PoseHandler& poseHandler, const CmderOrchestrator& orchestrator) const noexcept
    {
        return orchestrator.TurnRight(poseHandler);
    }
};

class TurnRoundCommand final
{
public:
    ActionGroup operator()(const PoseHandler& poseHandler, const CmderOrchestrator& orchestrator) const noexcept
    {
        return orchestrator.TurnRound(poseHandler);
    }
};

// 加速 / 倒车与车型无关，直接返回状态切换动作
class FastCommand final
{
public:
    ActionGroup operator()(const PoseHandler&, const CmderOrchestrator&) const noexcept
    {
        return ActionGroup({ActionType::BE_FAST_ACTION});
    }
};

class ReverseCommand final
{
public:
    ActionGroup operator()(const PoseHandler&, const CmderOrchestrator&) const noexcept
    {
        return ActionGroup({ActionType::BE_REVERSE_ACTION});
    }
};
}  // namespace adas
