# 实验4 综合实战复盘报告

> 课程：华为《C++企业软件开发实践》实验4「综合实战」
> 需求：需求4「支持掉头功能」+ 面向接口编程 + 任务编排 + 物理设计 + 车型适配
> 工程：`D:\VSCodeProjects\C++Course\Experiment4Student`
> 基线：`master@7155c04`（实验1~3，25 个用例全绿）
> 完成：`master@a6b9d03`（52 个用例全绿，已推送 GitHub）

---

## 一、成果概览

| 项目 | 结果 |
|---|---|
| 开发分支 | `lab4-dev`（全程与 `master` 隔离） |
| 编译 | ✅ `.\script\build.bat` 退出码 0 |
| 单元测试 | ✅ **52 / 52 PASSED** |
| `ctest` | ✅ 100% tests passed |
| Git 提交 | ✅ 8 条，message 逐字一致（含双空格与拼写错误） |
| 里程碑分支 | ✅ `lab4-comprehensive-actual-combat` |
| 合并 | ✅ `master` 快进合并 `lab4-dev`，合并后 52/52 |
| 远程 | ✅ `master`、`lab4-dev`、`lab4-comprehensive-actual-combat` 均已推送 |

测试构成：`ExecutorTest`(14) + `ExecutorFastTest`(4) + `ExecutorReverseTest`(4) + `ExecutorReverseFastTest`(3) + `ExecutorTurnRoundTest`(3) + `SportsCarTest`(12) + `BusTest`(12) = **52**。

---

## 二、分支隔离策略与执行结果

| 项 | 实际执行 |
|---|---|
| 隔离 | 从 `master@7155c04` 创建 `lab4-dev`；开发期间 `master` 指针未移动 |
| 安全网 | 每步 `build.bat` + 全量测试通过后才提交 |
| 回退 | 全程未触发 `git reset --hard` |
| 合并 | 最后 `checkout master` + `merge lab4-dev`（Fast-forward，无冲突） |
| 里程碑 | 第 6 次提交后创建 `lab4-comprehensive-actual-combat` |

---

## 三、8 次提交与验证

| # | 哈希 | commit message（原文） | 作用 | 提交时测试 |
|---|---|---|---|---|
| 1 | `8f32301` | `Singleton factory proivde unified interfaces` | 引入模板单例与 `CmderFactory`，`Execute` 不再每次建 map | 25/25 ✅ |
| 2 | `701db87` | `use using feature to  optimize code` | `using Cmder / CmderList` 简化类型 | 25/25 ✅ |
| 3 | `0b2773f` | `introduce action to as  real command` | 引入 `ActionGroup` 与 8 个原子动作，命令改为返回动作组 | 25/25 ✅ |
| 4 | `add0bb7` | `feat: Add test cases  for TR command` | TR 三个用例（测试先行） | 28 ran，3 失败 ✅ |
| 5 | `2a739f3` | `test: Pass TR command  test cases, TR command easy implement` | `TR→Z` 映射 + `TurnRoundCommand` | 28/28 ✅ |
| 6 | `07aed42` | `physical design and code layering  improve code readability` | `src/cmder` + `src/core` 分层并修正 include | 28/28 ✅ |
| 7 | `03d8e40` | `feat: Add test cases for Bus&SportCar` | 24 个车型用例（测试先行） | 52 ran，20 失败 ✅ |
| 8 | `a6b9d03` | `test: Pass Bus&SportCar test cases` | 三个 Orchestrator + 多态接线 | **52/52** ✅ |

> 脚本读取每个 commit 的原始 body 逐字符比对，8/8 完全一致，`to  optimize`、`to as  real`、`cases  for`、`command  test`、`layering  improve` 的双空格以及 `proivde` 均原样保留。

---

## 四、架构演进

### 4.1 调用链（实验4 完成后）

```text
ExecutorImpl::Execute
  → Singleton<CmderFactory>::Instance().GetCmders(commands)   // TR→Z 后查表
  → cmder(poseHandler, *orchestrator)                          // 命令 → ActionGroup
  → ActionGroup::DoOperate(poseHandler)                        // 逐条执行原子动作
  → ForwardAction / TurnLeftAction / ... → PoseHandler
```

### 4.2 依赖方向

```text
ExecutorImpl ──▶ CmderFactory ──▶ Command ──▶ CmderOrchestrator
      │                              │              │
      └──▶ PoseHandler ◀─────────────┘              │
                │                                  │
                └──▶ Direction ──▶ Point            │
                                                   │
NormalOrchestrator ──▶ SportsCarOrchestrator        │
        └──────────────▶ BusOrchestrator ◀──────────┘
```

- `Command` 不再直接操作状态，只把“指令语义”翻译为由 Orchestrator 产生的动作序列；
- `CmderOrchestrator` 只依赖 `ActionGroup` 与 `PoseHandler`，不依赖 `ExecutorImpl`；
- `ExecutorImpl` 只持有 `PoseHandler` 与 `unique_ptr<CmderOrchestrator>`。

### 4.3 物理分层

```text
src/
├── cmder/
│   ├── ActionGroup.hpp/.cpp
│   ├── CmderFactory.hpp/.cpp
│   ├── Command.hpp
│   ├── CmderOrchestrator.hpp
│   ├── NormalOrchestrator.hpp
│   ├── SportsCarOrchestrator.hpp
│   └── BusOrchestrator.hpp
├── core/
│   ├── Point.hpp/.cpp
│   ├── Direction.hpp/.cpp
│   ├── PoseHandler.hpp/.cpp
│   └── Singleton.hpp
├── ExecutorImpl.hpp/.cpp
└── CMakeLists.txt
```

所有 include 以 `src/` 为根（`core/...`、`cmder/...`），由 `TARGET_INCLUDE_DIRECTORIES` 中的 `${CMAKE_SOURCE_DIR}/src` 解析。

---

## 五、关键设计实现

### 5.1 单例 + 工厂

```cpp
static T& Instance(void) noexcept { static T instance; return instance; }
```

- C++11 起静态局部变量初始化线程安全；
- `CmderFactory` 的 map 只在首次调用时构造一次，`Execute` 不再重复建表。

### 5.2 原子动作表

`ActionType` 的 8 个枚举值与 `actionVec` 的下标严格一一对应，`static_cast<uint16_t>` 直接索引：

```cpp
actionVec[static_cast<uint16_t>(actionType)](poseHandler);
```

`static` 保证动作对象只构造一次。

### 5.3 命令返回 `ActionGroup`

`MoveCommand` 的关键点：

```cpp
ActionGroup actionGroup;
const auto action = poseHandler.IsReverse() ? BACKWARD_1_STEP_ACTION : FORWARD_1_STEP_ACTION;
if (poseHandler.IsFast()) { actionGroup.PushAction(action); }
actionGroup.PushAction(action);
return actionGroup;   // 课件示例漏写，已补上
```

### 5.4 TR 双字符映射

`GetCmders` 先 `ParseCommandString` 把 `"TR"` 全部替换为 `"Z"`，再按单字符查表；`Z → TurnRoundCommand`。
`ReplaceAll` 使用 `inout.find(what.data(), pos, what.length())`，避免 `std::string_view` 非空终止问题。

### 5.5 车型多态

| 车型 | Move | TurnLeft / TurnRight |
|---|---|---|
| Normal | 1 步（F 时 2 步） | 原地转向（F 时先 1 步；B 时反向） |
| SportsCar | 2 步（F 时 4 步） | 转向后再 1 步（F 时先 1 步；B 时反向且步骤取反） |
| Bus | 1 步（同 Normal） | 先 1 步再转向（F 时先 1 步；B 时反向） |

- `GetStepAction` 统一处理“倒车时前进变后退”；
- `OnFast` 统一处理“加速时额外一步”；
- `ActionGroup::operator+=` 用于拼接 `OnFast` 与主体动作。

---

## 六、踩坑与解决记录

### 坑 1：课件 `MoveCommand` 漏写 `return actionGroup;`

若直接照抄会导致“控制流到达非 void 函数末尾”编译错误。已在实现中补上，且所有命令的每条分支都有返回值。

### 坑 2：提交信息中的双空格与拼写错误

- `use using feature to  optimize code`
- `introduce action to as  real command`
- `feat: Add test cases  for TR command`
- `test: Pass TR command  test cases, ...`
- `physical design and code layering  improve code readability`
- `proivde`

**关键点**：Git 默认 `-m` 会清除行尾空格（实测），因此统一使用 `git commit --cleanup=verbatim -m "..."`；双空格位于行内不会被清除，但逐字校验仍然必要。已用脚本读取原始 body 比对，8/8 一致。

### 坑 3：原文笔误必须照抄

- 类名 `BeFaseAction`（不是 `BeFastAction`）；
- Bus BL 用例名 `should_returny_x_minus_1_...`（不是 `should_return_...`）；
- 分支名 `lab4-comprehensive-actual-combat`；
- `TurnRoundCommand` 的第三个用例名 `in_the_B_state_the_reverse_command_will_be_ignored`。

以上均按原文保留，未“纠正”。

### 坑 4：目录搬迁后的 include 路径

用 `git mv` 移动 12 个文件后，逐个检查：

- `cmder/ActionGroup.hpp` → `core/PoseHandler.hpp`
- `cmder/Command.hpp` → `cmder/ActionGroup.hpp` + `core/PoseHandler.hpp`
- `ExecutorImpl.hpp` → `core/PoseHandler.hpp`
- `ExecutorImpl.cpp` → `cmder/CmderFactory.hpp` + `core/Singleton.hpp`

搬完立即删除 `build/` 做**从零全量构建**，28/28 通过后才提交。

### 坑 5：`const PoseHandler&` 与状态修改的矛盾

命令签名统一为 `(const PoseHandler&, const CmderOrchestrator&)`，但 `F`/`B` 需要改状态。
解决：`FastCommand` / `ReverseCommand` 不直接改状态，而是返回包含 `BE_FAST_ACTION` / `BE_REVERSE_ACTION` 的 `ActionGroup`；真正的修改发生在 `ActionGroup::DoOperate(PoseHandler&)`（非 const）。这样既满足签名统一，又保持状态变更集中。

### 坑 6：测试先行的“部分通过”现象

- TR 测试先行：3 个全部失败（符合预期）。
- 车型测试先行：24 个中 **20 失败、4 通过**（Bus 的 `M/BM/FM/FBM` 与 Normal 行为恰好一致）。
- 整个测试套件均处于失败状态（退出码 1），TDD 门槛成立。报告如实记录，不伪造“全部失败”。

另：为让车型测试在提交 7 时能够编译，该提交同时加入了 `ExecutorType` 枚举与 `NewExecutor` 的默认参数（仍走 Normal 行为），属于“测试先行所必需的接口脚手架”。

### 坑 7：`ActionGroup` 的构造与拼接

- `explicit ActionGroup(const std::list<ActionType>&)` 配合 `ActionGroup({...})` 可正常工作；
- `operator+=` 通过 `actions.insert(actions.end(), rhs.actions.begin(), rhs.actions.end())` 实现，供 Orchestrator 拼接 `OnFast` 结果。

### 坑 8：`PushAction(const ActionType ActionType)` 参数同名

课件参数名与枚举类型同名，GCC 可正常解析（参数名在函数作用域内遮蔽类型名），未做改写。

### 坑 9：`std::string_view` 与 C++17

`ParseCommandString(std::string_view)` 依赖 C++17；工程 `CMAKE_CXX_STANDARD 17` 已满足。

---

## 七、验收自检

| 检查项 | 结果 |
|---|---|
| `Singleton.hpp`、`CmderFactory.hpp/.cpp` 就位，`Execute` 不重复建 map | ✅ |
| `ActionGroup`（含 `+=`）、8 个 `ActionType`、8 个 Action 类可用 | ✅ |
| 所有命令返回 `ActionGroup`，执行链 `cmder(...).DoOperate(...)` | ✅ |
| TR：正常 3 步、加速 4 步、倒车忽略；`TR→Z` | ✅ |
| `src/cmder` + `src/core` 分层，include 全部更新 | ✅ |
| `ExecutorType` 与 `NewExecutor` 重载，旧测试不受影响 | ✅ |
| 三种车型 4 个编排方法正确 | ✅ |
| 52 个用例全部通过 | ✅ |
| 8 条提交与 1 个里程碑分支 | ✅ |
| 工作区干净、构建产物未入库 | ✅ |

---

## 八、复现与验证

```bat
cd /d D:\VSCodeProjects\C++Course\Experiment4Student
.\script\build.bat
.\script\run.bat
```

预期结尾：

```
[==========] 52 tests from 7 test suites ran. (0 ms total)
[  PASSED  ] 52 tests.
```

独立复核（链接工程库，不依赖测试断言）：

```
NORMAL TR     -> 0,1,W
NORMAL FTR    -> 1,1,W
NORMAL BTR    -> 0,0,E
NORMAL FMLMLM -> -3,1,S
SPORTS M      -> 2,0,E
SPORTS FL     -> 1,1,N
SPORTS FBM    -> -4,0,E
BUS    L      -> 1,0,N
BUS    FBL    -> -2,0,S
```

---

## 九、备注

1. 本报告在推送完成后生成，**未纳入 8 条提交序列**，以保持课程要求的提交日志纯净；如需入库可追加一条提交。
2. 交付打包时务必包含 `.git` 目录。
3. `tests/googletest` 仍按模板 `.gitignore` 排除：整包交付可正常构建，从 GitHub 单独 clone 需另行补齐 gtest。
