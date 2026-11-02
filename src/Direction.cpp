#include "Direction.hpp"

namespace adas
{
namespace
{
// 4种方向
const Direction directions[4] = {
    {0, 'E'},
    {1, 'S'},
    {2, 'W'},
    {3, 'N'},
};

// 4种前进坐标
const Point points[4] = {
    {1, 0},   // E
    {0, -1},  // S
    {-1, 0},  // W
    {0, 1},   // N
};
}  // namespace

const Direction& Direction::GetDirection(const char heading) noexcept
{
    for (const auto& direction : directions)
    {
        if (direction.heading == heading)
        {
            return direction;
        }
    }
    return directions[3];  // 没有匹配的方向返回默认方向 N
}

Direction::Direction(const unsigned index, const char heading) noexcept : index(index), heading(heading)
{
}

const Point& Direction::Move(void) const noexcept
{
    return points[index];
}

const Direction& Direction::LeftOne(void) const noexcept
{
    return directions[(index + 3) % 4];
}

const Direction& Direction::RightOne(void) const noexcept
{
    return directions[(index + 1) % 4];
}

const char Direction::GetHeading(void) const noexcept
{
    return heading;
}
}  // namespace adas
