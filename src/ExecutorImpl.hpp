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

public:
    // 这些状态操作暂时公开，供 Command.hpp 中的命令类委托调用
    void Move(void) noexcept;          // 执行M指令的逻辑现在封装在函数Move里
    void TurnLeft(void) noexcept;      // 执行L指令的逻辑现在封装在函数TurnLeft里
    void TurnRight(void) noexcept;     // 执行R指令的逻辑现在封装在函数TurnRight里
    void Fast(void) noexcept;          // 切换加速状态
    bool IsFast(void) const noexcept;  // 查询当前是否处于加速状态

private:
    Pose pose;
    bool fast{false};  // 记录是否处于加速状态
};
}  // namespace adas
