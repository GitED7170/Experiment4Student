#include "ExecutorImpl.hpp"

#include <memory>
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
    // 表驱动：建立“指令字符 -> 命令对象”的映射
    std::unordered_map<char, std::unique_ptr<ICommand>> cmderMap;
    // 建立操作 M 和前进指令的映射关系
    cmderMap.emplace('M', std::make_unique<MoveCommand>());
    // 建立操作 L 和左转指令的映射关系
    cmderMap.emplace('L', std::make_unique<TurnLeftCommand>());
    // 建立操作 R 和右转指令的映射关系
    cmderMap.emplace('R', std::make_unique<TurnRightCommand>());
    // 建立操作 F 和加速指令的映射关系
    cmderMap.emplace('F', std::make_unique<FastCommand>());

    for (const auto cmd : commands)
    {
        // 根据操作查找表驱动
        const auto it = cmderMap.find(cmd);
        // 如果找到表驱动，执行操作对应的指令
        if (it != cmderMap.end())
        {
            it->second->DoOperate(poseHandler);
        }
    }
}

Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);  // 只在C++17下有效
}
}  // namespace adas
