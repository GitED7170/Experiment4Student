#pragma once

#include "Executor.hpp"

namespace adas
{
/* Executor接口的具体实现，维护汽车的当前姿态 */
class ExecutorImpl final : public Executor
{
public:
    explicit ExecutorImpl(const Pose& pose) noexcept;
    ~ExecutorImpl() noexcept = default;

    // 不允许拷贝
    ExecutorImpl(const ExecutorImpl&) = delete;
    // 不允许赋值
    ExecutorImpl& operator=(const ExecutorImpl&) = delete;

public:
    Pose Query(void) const noexcept override;
    void Execute(const std::string& commands) noexcept override;

private:
    // 定义一个嵌套类 MoveCommand，完成 Move 动作（M 指令）
    class MoveCommand final
    {
    public:
        // 执行 Move 动作，需要委托 ExecutorImpl& 执行器来完成动作
        void DoOperate(ExecutorImpl& executor) const noexcept
        {
            executor.Move();
        }
    };

    // 定义一个嵌套类 TurnLeftCommand，完成 TurnLeft 动作（L 指令）
    class TurnLeftCommand final
    {
    public:
        void DoOperate(ExecutorImpl& executor) const noexcept
        {
            executor.TurnLeft();
        }
    };

    // 定义一个嵌套类 TurnRightCommand，完成 TurnRight 动作（R 指令）
    class TurnRightCommand final
    {
    public:
        void DoOperate(ExecutorImpl& executor) const noexcept
        {
            executor.TurnRight();
        }
    };

private:
    void Move(void) noexcept;
    void TurnLeft(void) noexcept;
    void TurnRight(void) noexcept;

private:
    Pose pose;
};
}  // namespace adas
