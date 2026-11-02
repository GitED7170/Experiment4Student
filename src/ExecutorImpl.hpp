#pragma once

#include <memory>

#include "Executor.hpp"
#include "cmder/CmderOrchestrator.hpp"
#include "core/PoseHandler.hpp"

namespace adas
{
/* Executor接口的具体实现，状态与车型编排分别由 PoseHandler / CmderOrchestrator 负责 */
class ExecutorImpl final : public Executor
{
public:
    explicit ExecutorImpl(const Pose& pose, CmderOrchestrator* orchestrator) noexcept;
    ~ExecutorImpl() noexcept = default;

    // 不允许拷贝
    ExecutorImpl(const ExecutorImpl&) = delete;
    // 不允许赋值
    ExecutorImpl& operator=(const ExecutorImpl&) = delete;

public:
    Pose Query(void) const noexcept override;
    void Execute(const std::string& commands) noexcept override;

private:
    PoseHandler poseHandler;
    std::unique_ptr<CmderOrchestrator> orchestrator;
};
}  // namespace adas
