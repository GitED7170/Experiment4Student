#include <gtest/gtest.h>

#include <memory>

#include "Executor.hpp"
#include "PoseEq.hpp"

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

TEST(ExecutorTest, should_return_x_plus_1_given_command_is_M_and_facing_is_E)
{
    // given 给定一个executor
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));  // 给了初始姿势, 当前朝向是E, 起点(0,0)

    // when 调用executor的Execute方法去执行M指令
    executor->Execute("M");

    // then
    const Pose target({1, 0, 'E'});  // 如果执行M指令正确，新的姿势应该是target: {1, 0, 'E'}
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_x_minus_1_given_command_is_M_and_facing_is_W)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));

    // when
    executor->Execute("M");

    // then
    const Pose target({-1, 0, 'W'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_y_plus_1_given_command_is_M_and_facing_is_N)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));

    // when
    executor->Execute("M");

    // then
    const Pose target({0, 1, 'N'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_y_minus_1_given_command_is_M_and_facing_is_S)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));

    // when
    executor->Execute("M");

    // then
    const Pose target({0, -1, 'S'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_N_given_command_is_L_and_facing_is_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("L");

    // then
    const Pose target({0, 0, 'N'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_S_given_command_is_L_and_facing_is_W)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));

    // when
    executor->Execute("L");

    // then
    const Pose target({0, 0, 'S'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_W_given_command_is_L_and_facing_is_N)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));

    // when
    executor->Execute("L");

    // then
    const Pose target({0, 0, 'W'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_E_given_command_is_L_and_facing_is_S)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));

    // when
    executor->Execute("L");

    // then
    const Pose target({0, 0, 'E'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_S_given_command_is_R_and_facing_is_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("R");

    // then
    const Pose target({0, 0, 'S'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_N_given_command_is_R_and_facing_is_W)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));

    // when
    executor->Execute("R");

    // then
    const Pose target({0, 0, 'N'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_E_given_command_is_R_and_facing_is_N)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));

    // when
    executor->Execute("R");

    // then
    const Pose target({0, 0, 'E'});
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_W_given_command_is_R_and_facing_is_S)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));

    // when
    executor->Execute("R");

    // then
    const Pose target({0, 0, 'W'});
    ASSERT_EQ(target, executor->Query());
}
