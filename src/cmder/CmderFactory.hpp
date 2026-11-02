#pragma once

#include <functional>
#include <list>
#include <string>
#include <string_view>
#include <unordered_map>

#include "cmder/CmderOrchestrator.hpp"
#include "cmder/Command.hpp"

namespace adas
{
// 用 using 定义类型别名，替代冗长的 std::function / std::list 写法
using Cmder = std::function<ActionGroup(const PoseHandler& poseHandler, const CmderOrchestrator& orchestrator)>;
using CmderList = std::list<Cmder>;

class CmderFactory final
{
public:
    CmderFactory(void) = default;
    ~CmderFactory(void) = default;

    // 删除构造和赋值函数，防止生成多个实例
    CmderFactory(const CmderFactory&) = delete;
    CmderFactory& operator=(const CmderFactory&) = delete;

public:
    CmderList GetCmders(const std::string& commands) const noexcept;

private:
    std::string ParseCommandString(std::string_view commands) const noexcept;
    void ReplaceAll(std::string& inout, std::string_view what, std::string_view with) const noexcept;

private:
    const std::unordered_map<char, Cmder> cmderMap{
        {'M', MoveCommand()},        // 前进
        {'L', TurnLeftCommand()},    // 左转
        {'R', TurnRightCommand()},   // 右转
        {'F', FastCommand()},        // 快速
        {'B', ReverseCommand()},     // 后退
        {'Z', TurnRoundCommand()},   // 掉头（由 TR 替换而来）
    };
};
}  // namespace adas
