#include "PoseEq.hpp"

namespace adas
{
bool operator==(const Pose& lhs, const Pose& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.heading == rhs.heading;
}
}  // namespace adas
