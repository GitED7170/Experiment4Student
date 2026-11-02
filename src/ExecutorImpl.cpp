#include "ExecutorImpl.hpp"

#include <algorithm>
#include <new>

#include "cmder/BusOrchestrator.hpp"
#include "cmder/CmderFactory.hpp"
#include "cmder/NormalOrchestrator.hpp"
#include "cmder/SportsCarOrchestrator.hpp"
#include "core/Singleton.hpp"

namespace adas
{
ExecutorImpl::ExecutorImpl(const Pose& pose, CmderOrchestrator* orchestrator) noexcept
    : poseHandler(pose), orchestrator(orchestrator)
{
}

Pose ExecutorImpl::Query(void) const noexcept
{
    return poseHandler.Query();
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    // 使用指令工厂获取指令字符串对应的命令列表
    const auto cmders = Singleton<CmderFactory>::Instance().GetCmders(commands);

    // 每个命令根据当前车型编排出一组原子动作，再批量执行
    std::for_each(cmders.begin(), cmders.end(), [this](const Cmder& cmder) noexcept {
        cmder(poseHandler, *orchestrator).DoOperate(poseHandler);
    });
}

Executor* Executor::NewExecutor(const Pose& pose, const ExecutorType executorType) noexcept
{
    CmderOrchestrator* orchestrator = nullptr;
    switch (executorType)
    {
        case ExecutorType::NORMAL:
            orchestrator = new (std::nothrow) NormalOrchestrator();
            break;
        case ExecutorType::SPORTS_CAR:
            orchestrator = new (std::nothrow) SportsCarOrchestrator();
            break;
        case ExecutorType::BUS:
            orchestrator = new (std::nothrow) BusOrchestrator();
            break;
    }
    return new (std::nothrow) ExecutorImpl(pose, orchestrator);
}
}  // namespace adas
