#include "ExecutorImpl.hpp"

#include <memory>
#include <new>

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
    for (const auto cmd : commands)
    {
        // 声明一个 ICommand 类型的智能指针
        std::unique_ptr<ICommand> cmder;

        if (cmd == 'M')
        {
            // 智能指针指向子类 MoveCommand 实例
            cmder = std::make_unique<MoveCommand>();
        }
        else if (cmd == 'L')
        {
            cmder = std::make_unique<TurnLeftCommand>();
        }
        else if (cmd == 'R')
        {
            cmder = std::make_unique<TurnRightCommand>();
        }
        else if (cmd == 'F')
        {
            cmder = std::make_unique<FastCommand>();
        }

        if (cmder)
        {
            // 多态：cmder 指向不同子类实例，调用不同的命令
            cmder->DoOperate(poseHandler);
        }
    }
}

Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);  // 只在C++17下有效
}
}  // namespace adas
