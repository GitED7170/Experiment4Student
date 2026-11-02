#include "ExecutorImpl.hpp"

#include <functional>
#include <new>
#include <unordered_map>

#include "Command.hpp"

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
    // 表驱动：建立“指令字符 -> 可调用对象”的映射
    std::unordered_map<char, std::function<void(PoseHandler&)>> cmderMap;
    cmderMap.emplace('M', MoveCommand());       // 前进
    cmderMap.emplace('L', TurnLeftCommand());   // 左转
    cmderMap.emplace('R', TurnRightCommand());  // 右转
    cmderMap.emplace('F', FastCommand());       // 快速

    for (const auto cmd : commands)
    {
        // 根据操作查找表驱动
        const auto it = cmderMap.find(cmd);
        // 如果找到表驱动，执行操作对应的指令
        if (it != cmderMap.end())
        {
            it->second(poseHandler);  // 可以直接将其作为函数使用
        }
    }
}

Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);  // 只在C++17下有效
}
}  // namespace adas
