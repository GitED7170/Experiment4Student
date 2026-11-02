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
    // 所有具体命令对象的抽象基类
    class ICommand
    {
    public:
        virtual ~ICommand() = default;
        virtual void DoOperate(ExecutorImpl& executor) const noexcept = 0;
    };

    // 定义一个嵌套类 MoveCommand，完成 Move 动作（M 指令）
    class MoveCommand final : public ICommand
    {
    public:
        // 执行 Move 动作，需要委托 ExecutorImpl& 执行器来完成动作
        void DoOperate(ExecutorImpl& executor) const noexcept override
        {
            executor.Move();
            // 加速状态下多前进一格（仍然是一格一格移动）
            if (executor.IsFast())
            {
                executor.Move();
            }
        }
    };

    // 定义一个嵌套类 TurnLeftCommand，完成 TurnLeft 动作（L 指令）
    class TurnLeftCommand final : public ICommand
    {
    public:
        void DoOperate(ExecutorImpl& executor) const noexcept override
        {
            // 加速状态下先前进一格，再左转
            if (executor.IsFast())
            {
                executor.Move();
            }
            executor.TurnLeft();
        }
    };

    // 定义一个嵌套类 TurnRightCommand，完成 TurnRight 动作（R 指令）
    class TurnRightCommand final : public ICommand
    {
    public:
        void DoOperate(ExecutorImpl& executor) const noexcept override
        {
            // 加速状态下先前进一格，再右转
            if (executor.IsFast())
            {
                executor.Move();
            }
            executor.TurnRight();
        }
    };

    // 定义一个嵌套类 FastCommand，完成加速状态切换（F 指令）
    class FastCommand final : public ICommand
    {
    public:
        void DoOperate(ExecutorImpl& executor) const noexcept override
        {
            executor.Fast();
        }
    };

private:
    void Move(void) noexcept;
    void TurnLeft(void) noexcept;
    void TurnRight(void) noexcept;
    void Fast(void) noexcept;          // 切换加速状态
    bool IsFast(void) const noexcept;  // 查询当前是否处于加速状态

private:
    Pose pose;
    bool fast{false};  // 记录是否处于加速状态
};
}  // namespace adas
