#include <gtest/gtest.h>

#include <memory>

#include "Executor.hpp"
#include "PoseEq.hpp"

using adas::Executor;
using adas::Pose;

TEST(ExecutorFastTest, should_return_x_plus_2_given_status_is_fast_command_is_M_and_facing_is_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("FM");  // FM: F 状态下 Move

    // then
    const Pose target{2, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorFastTest, should_return_N_and_x_plus_1_given_status_is_fast_command_is_L_and_facing_is_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("FL");  // FL: F 状态下先前进一格再左转

    // then
    const Pose target{1, 0, 'N'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorFastTest, should_return_S_and_x_plus_1_given_status_is_fast_given_command_is_R_and_facing_is_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("FR");  // FR: F 状态下先前进一格再右转

    // then
    const Pose target{1, 0, 'S'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorFastTest, should_return_y_plus_1_given_command_is_FFM_and_facing_is_N)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor());  // 默认起始状态是 {0,0,'N'}

    // when
    executor->Execute("FFM");  // FFM 等价于 M

    // then
    const Pose target{0, 1, 'N'};
    ASSERT_EQ(target, executor->Query());
}
