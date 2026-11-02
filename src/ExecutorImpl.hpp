#pragma once

#include "Executor.hpp"
#include "core/PoseHandler.hpp"

namespace adas
{
/* Executor接口的具体实现，只保留对外接口，状态交由 PoseHandler 管理 */
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
    PoseHandler poseHandler;  // 状态管理类
};
}  // namespace adas
