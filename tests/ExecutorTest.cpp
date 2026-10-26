#include <gtest/gtest.h>

#include <memory>

#include "Executor.hpp"

namespace adas
{
// ASSERT_EQ内部调用了重载的Pose的==，工程模板未提供，按需求在测试文件中补充
bool operator==(const Pose& lhs, const Pose& rhs) noexcept
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.heading == rhs.heading;
}
}  // namespace adas

using adas::Executor;
using adas::Pose;

TEST(ExecutorTest, should_return_init_pose_when_without_command)
{
    // given 给定一个executor，并给定初始姿势
    std::unique_ptr<Executor> executor(Executor::NewExecutor({1, 2, 'E'}));

    // when 不执行任何指令

    // then 返回初始化指定的位置
    const Pose target({1, 2, 'E'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_default_pose_when_without_init_and_command)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor());  // 没有初始姿势

    // when

    // then
    const Pose target({0, 0, 'N'});
    ASSERT_EQ(target, executor->Query());
}
