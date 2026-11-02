#include "PoseHandler.hpp"

namespace adas
{
PoseHandler::PoseHandler(const Pose& pose) noexcept : pose(pose)
{
}

/* 沿当前朝向前进一格：E=X+1，W=X-1，N=Y+1，S=Y-1 */
void PoseHandler::Move(void) noexcept
{
    switch (pose.heading)
    {
        case 'E':
            ++pose.x;
            break;
        case 'W':
            --pose.x;
            break;
        case 'N':
            ++pose.y;
            break;
        case 'S':
            --pose.y;
            break;
        default:
            break;
    }
}

/* 左转90度（逆时针），位置不变：E→N，W→S，N→W，S→E */
void PoseHandler::TurnLeft(void) noexcept
{
    switch (pose.heading)
    {
        case 'E':
            pose.heading = 'N';
            break;
        case 'W':
            pose.heading = 'S';
            break;
        case 'N':
            pose.heading = 'W';
            break;
        case 'S':
            pose.heading = 'E';
            break;
        default:
            break;
    }
}

/* 右转90度（顺时针），位置不变：E→S，W→N，N→E，S→W */
void PoseHandler::TurnRight(void) noexcept
{
    switch (pose.heading)
    {
        case 'E':
            pose.heading = 'S';
            break;
        case 'W':
            pose.heading = 'N';
            break;
        case 'N':
            pose.heading = 'E';
            break;
        case 'S':
            pose.heading = 'W';
            break;
        default:
            break;
    }
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

Pose PoseHandler::Query(void) const noexcept
{
    return pose;
}
}  // namespace adas
