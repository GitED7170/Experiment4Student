#include "PoseHandler.hpp"

namespace adas
{
PoseHandler::PoseHandler(const Pose& pose) noexcept
    : point(pose.x, pose.y), facing(&Direction::GetDirection(pose.heading))
{
}

void PoseHandler::Forward(void) noexcept
{
    point += facing->Move();
}

void PoseHandler::Backward(void) noexcept
{
    point -= facing->Move();
}

void PoseHandler::TurnLeft(void) noexcept
{
    facing = &(facing->LeftOne());
}

void PoseHandler::TurnRight(void) noexcept
{
    facing = &(facing->RightOne());
}

/* 切换加速状态：每收到一次 F 取反 */
void PoseHandler::Fast(void) noexcept
{
    fast = !fast;
}

bool PoseHandler::IsFast(void) const noexcept
{
    return fast;
}

/* 切换倒车状态：每收到一次 B 取反 */
void PoseHandler::Reverse(void) noexcept
{
    reverse = !reverse;
}

bool PoseHandler::IsReverse(void) const noexcept
{
    return reverse;
}

Pose PoseHandler::Query(void) const noexcept
{
    return {point.GetX(), point.GetY(), facing->GetHeading()};
}
}  // namespace adas
