#pragma once

#include <functional>
#include <list>
#include <string>
#include <unordered_map>

#include "Command.hpp"

namespace adas
{
class CmderFactory final
{
public:
    CmderFactory(void) = default;
    ~CmderFactory(void) = default;

    // 删除构造和赋值函数，防止生成多个实例
    CmderFactory(const CmderFactory&) = delete;
    CmderFactory& operator=(const CmderFactory&) = delete;

public:
    std::list<std::function<void(PoseHandler&)>> GetCmders(const std::string& commands) const noexcept;

private:
    const std::unordered_map<char, std::function<void(PoseHandler&)>> cmderMap{
        {'M', MoveCommand()},        // 前进
        {'L', TurnLeftCommand()},    // 左转
        {'R', TurnRightCommand()},   // 右转
        {'F', FastCommand()},        // 快速
        {'B', ReverseCommand()},     // 后退
    };
};
}  // namespace adas
