#include "ActionGroup.hpp"

#include <algorithm>
#include <functional>
#include <vector>

namespace adas
{
namespace
{
/* 8 种原子动作，顺序必须与 ActionType 枚举一一对应 */

class ForwardAction final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.Forward();
    }
};

class BackwardAction final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.Backward();
    }
};

class TurnLeftAction final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.TurnLeft();
    }
};

// 反向左转，即右转 90 度
class ReverseTurnLeftAction final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.TurnRight();
    }
};

class TurnRightAction final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.TurnRight();
    }
};

// 反向右转，即左转 90 度
class ReverseTurnRightAction final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.TurnLeft();
    }
};

class BeFaseAction final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.Fast();
    }
};

class BeReverseAction final
{
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.Reverse();
    }
};
}  // namespace

ActionGroup::ActionGroup(const std::list<ActionType>& actions) noexcept : actions(actions)
{
}

void ActionGroup::PushAction(const ActionType ActionType) noexcept
{
    actions.push_back(ActionType);
}

void ActionGroup::DoOperate(PoseHandler& poseHandler) const noexcept
{
    static std::vector<std::function<void(PoseHandler&)>> actionVec = {
        ForwardAction(),
        BackwardAction(),
        TurnLeftAction(),
        ReverseTurnLeftAction(),
        TurnRightAction(),
        ReverseTurnRightAction(),
        BeFaseAction(),
        BeReverseAction(),
    };

    std::for_each(actions.begin(), actions.end(),
                  [&poseHandler](const ActionType actionType) mutable noexcept {
                      actionVec[static_cast<uint16_t>(actionType)](poseHandler);
                  });
}
}  // namespace adas
