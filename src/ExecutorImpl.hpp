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
    void Move(void) noexcept;
    void TurnLeft(void) noexcept;

private:
    Pose pose;
};
}  // namespace adas
