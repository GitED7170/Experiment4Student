#pragma once

#include "cmder/ActionGroup.hpp"

namespace adas
{
/* 命令编排器抽象基类：不同车型把同一命令编排成不同的原子动作 */
class CmderOrchestrator
{
public:
    virtual ~CmderOrchestrator() = default;

public:
    virtual ActionGroup Move(const PoseHandler& poseHandler) const noexcept = 0;
    virtual ActionGroup TurnLeft(const PoseHandler& poseHandler) const noexcept = 0;
    virtual ActionGroup TurnRight(const PoseHandler& poseHandler) const noexcept = 0;
    virtual ActionGroup TurnRound(const PoseHandler& poseHandler) const noexcept = 0;
};
}  // namespace adas
