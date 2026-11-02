#include "ExecutorImpl.hpp"

#include <algorithm>
#include <functional>
#include <new>

#include "CmderFactory.hpp"
#include "Singleton.hpp"

namespace adas
{
ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept : poseHandler(pose)
{
}

Pose ExecutorImpl::Query(void) const noexcept
{
    return poseHandler.Query();
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    // 使用指令工厂获取指令字符串对应的命令列表
    // cmders 类型是 std::list<Cmder>
    const auto cmders = Singleton<CmderFactory>::Instance().GetCmders(commands);

    // 遍历命令列表里的每个命令 cmder
    std::for_each(
        cmders.begin(), cmders.end(),
        [this](const std::function<void(PoseHandler& poseHandler)>& cmder) noexcept { cmder(poseHandler); });
}

Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);  // 只在C++17下有效
}
}  // namespace adas
