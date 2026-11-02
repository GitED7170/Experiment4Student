# 实验1 完整实施与复盘报告

> 课程：华为《C++企业软件开发实践》实验1「高质量编程基础」
> 需求：任务1（实验1）需求1「支持基本控制指令」
> 工程：`D:\VSCodeProjects\C++Course\Experiment4Student`
> 远程：`git@github.com:GitED7170/Experiment4Student.git`
> 完成日期：2026-10-26

---

## 一、成果概览

| 项目 | 结果 |
|---|---|
| 编译 | ✅ `.\script\build.bat` 退出码 0 |
| 单元测试 | ✅ 14 / 14 PASSED（`.\script\run.bat` 退出码 0） |
| `ctest` | ✅ 100% tests passed |
| 提交数量 | ✅ 10 条，message 与需求 4.3 逐字一致 |
| 里程碑分支 | ✅ `lab1-cleancode-base` |
| 远程推送 | ✅ `origin/master` 已建立跟踪 |
| 工具链 | g++ 8.1.0（x86_64-posix-seh-rev0）+ CMake 4.4.3 + MinGW Makefiles |

---

## 二、10 步 Git 提交日志与作用

```
4ccee7d (HEAD -> master, lab1-cleancode-base) test: Pass turn right command test cases
0cad2f3 feat: Add test cases for turn right command
e7b00a0 test: Pass turn left command test cases
cc8d429 feat: Add test cases for turn left command
b45fd8c test: Add and Pass all move command test cases
42b66ef test: Pass one move command test cases
1af9dd4 feat: Add one test cases for move command
12ea963 test: Pass initial and default pose test cases
56e0f2d feat: Add test cases for initial and default poses
4290333 init project
```

| 序号 | 哈希 | commit message（原文） | 本步作用 | 构建/测试状态 | 变更范围 |
|---|---|---|---|---|---|
| 1 | `4290333` | `init project` | 由工程模板初始化仓库，配置 C++17 与工程名，提交构建脚本与三级 CMakeLists | 模板空骨架，暂不配置 | 仅工程骨架 |
| 2 | `56e0f2d` | `feat: Add test cases for initial and default poses` | 落地 `Executor` 接口、`ExecutorImpl` 骨架与 2 个初始姿态用例（含 `Pose::operator==`） | 编译成功，**2 个用例失败** | 接口 + 骨架 + 测试 |
| 3 | `12ea963` | `test: Pass initial and default pose test cases` | 构造在初始化列表 `: pose(pose)`，`Query` 返回 `pose` | 编译成功，**2/2 通过** | 仅实现 |
| 4 | `1af9dd4` | `feat: Add one test cases for move command` | 新增 E 朝向的 M 用例（`Execute` 仍为空实现） | 编译成功，**1 失败** | 仅测试 |
| 5 | `42b66ef` | `test: Pass one move command test cases` | 实现 M：按 E/W/N/S 调整 x/y | 编译成功，**3/3 通过** | 仅实现 |
| 6 | `b45fd8c` | `test: Add and Pass all move command test cases` | 补齐 W/N/S 三个 M 用例；通用实现使三者直接通过 | 编译成功，**6/6 通过** | 仅测试 |
| 7 | `cc8d429` | `feat: Add test cases for turn left command` | 新增 L 的 4 个用例（尚未实现） | 编译成功，**4 失败** | 仅测试 |
| 8 | `e7b00a0` | `test: Pass turn left command test cases` | 实现 L：E→N，W→S，N→W，S→E | 编译成功，**10/10 通过** | 仅实现 |
| 9 | `0cad2f3` | `feat: Add test cases for turn right command` | 新增 R 的 4 个用例（尚未实现） | 编译成功，**4 失败** | 仅测试 |
| 10 | `4ccee7d` | `test: Pass turn right command test cases` | 实现 R：E→S，W→N，N→E，S→W | 编译成功，**14/14 通过** | 仅实现 |

**提交纪律落实**：
- 每步只改一类文件：`feat:` 只动 `tests/ExecutorTest.cpp`（第 2 步含必要接口骨架，见“踩坑 5”），`test:` 只动 `src/ExecutorImpl.cpp`。
- 每条 message 与需求 4.3 表格逐字一致，无前导空格、无句号、不改大小写。
- 全程只追加提交，未使用 `reset --hard` / `amend` / 删分支。

---

## 三、核心代码结构分析

### 3.1 文件职责

| 文件 | 职责 | 设计要点 |
|---|---|---|
| `include/Executor.hpp` | 对外接口 | `namespace adas`；`Pose` 仅 3 个字段；`Executor` **无数据成员**；`= delete` 禁拷贝/赋值；`NewExecutor` 静态工厂，默认实参 `{0, 0, 'N'}`；`Query` 为 `const noexcept = 0`，`Execute` 为 `noexcept = 0` |
| `src/ExecutorImpl.hpp` | 具体实现声明 | 公有继承；构造 `explicit`；`Query`/`Execute` 带 `override`；状态唯一：`private: Pose pose;` |
| `src/ExecutorImpl.cpp` | 实现 | 构造 `: pose(pose)`；`Query` 返回 `pose`；`Execute` 逐字符分发；`new (std::nothrow)` 工厂 |
| `tests/ExecutorTest.cpp` | 14 个用例 | 套件名 `ExecutorTest`；统一 Given-When-Then；`ASSERT_EQ` + `Pose::operator==` |

### 3.2 实现策略：匿名命名空间 + 单一状态

移动与转向被拆为三个仅在当前翻译单元可见的函数，放在 `namespace adas { namespace { ... } }` 中：

```cpp
void Move(Pose& pose) noexcept;       // E=X+1, W=X-1, N=Y+1, S=Y-1
void TurnLeft(Pose& pose) noexcept;   // E→N, W→S, N→W, S→E
void TurnRight(Pose& pose) noexcept;  // E→S, W→N, N→E, S→W
```

这样做的好处：
- **最小可见原则**：辅助函数不进入头文件，客户与链接符号都看不到；
- **单一职责**：`Execute` 只负责“拆分指令 + 分发”，方向规则集中在各自函数；
- **易扩展**：新增指令只加 `case` 与一个函数，`Executor` 接口零改动。

`Execute` 的处理方式严格按需求“把字符串当 char 数组遍历”：

```cpp
for (const auto cmd : commands)
{
    switch (cmd)
    {
        case 'M': Move(pose);      break;
        case 'L': TurnLeft(pose);  break;
        case 'R': TurnRight(pose); break;
        default: break;
    }
}
```

因此 `MLMRM` 这类组合指令天然支持，且状态跨多次 `Execute` 调用保持。

### 3.3 构建结构（模板自带，未改动逻辑）

- 根 `CMakeLists.txt`：`CXX_STANDARD 17`、`bin/` 输出、`ENABLE_TESTING()`、`ADD_SUBDIRECTORY(src)` / `(tests)`。
- `src/CMakeLists.txt`：`FILE(GLOB_RECURSE)` 收集 cpp，剔除 `main.cpp`，构建静态库 `libExperiment4Student.a`。
- `tests/CMakeLists.txt`：`AUX_SOURCE_DIRECTORY` 收集测试，链接 `${PROJECT_NAME}` + `gtest_main`，产出 `Experiment4Student-main.exe`。

> 说明：`Executable` 名称来自 `${PROJECT_NAME}-main`，与 `launch.json` 的 `${workspaceFolderBasename}-main.exe` 一致，因此调试配置无需修改。

### 3.4 测试覆盖矩阵（14 个）

| 类别 | 用例数 | 覆盖 |
|---|---|---|
| 初始/默认姿态 | 2 | 指定初始位姿；无参默认 `(0,0,N)` |
| M | 4 | E/W/N/S 四个朝向的坐标增量 |
| L | 4 | E/W/N/S 四个朝向的左转结果 |
| R | 4 | E/W/N/S 四个朝向的右转结果 |

用例名已用脚本与需求文档逐字比对：**14/14 完全一致**。

---

## 四、踩坑与解决记录

### 坑 1：CMake 4.4.3 拒绝 `cmake_minimum_required(VERSION 3.0)`

**现象**：模板原样配置直接失败。

```
Compatibility with CMake < 3.5 has been removed from CMake.
```

**原因**：CMake 4.0 起移除了对 `< 3.5` 的兼容；模板写的是 3.0。

**解决**：根 `CMakeLists.txt` 改为 `cmake_minimum_required(VERSION 3.5)`。选择 3.5 而非降级 CMake，是因为工程内 googletest 自身本来就要求 3.5，取 3.5 对全套依赖都安全。

### 坑 2：模板中混入了本机绝对编译器路径

**现象**：早期为“锁定 8.1.0”曾在 `CMakeLists.txt` 写入：

```cmake
set(CMAKE_CXX_COMPILER "D:/Turing machine/Documents/mingw64/bin/g++.exe")
```

**风险**：这份工程要打包交给老师/助教，换一台机器该路径不存在，`cmake` 会**直接配置失败**。

**解决**：交付前删除这两行，工程保持可移植。本机需要 8.1.0 时，只在**执行构建的 shell 内**把 MinGW 8.1.0 放到 PATH 最前，不把环境差异写进工程。

### 坑 3：本机有两套 g++，MSYS2 15.2.0 会抢先

**现象**：新开终端执行 `g++ --version` 显示 15.2.0，而课程要求 8.1.0。

**原因**：`D:\apps\Code\msys2\mingw64\bin` 在系统 PATH 中位于课程工具链之前。

**解决**：构建命令前临时前置 `D:\Turing machine\Documents\mingw64\bin`，并通过 `build/CMakeFiles/*/CMakeCXXCompiler.cmake` 核验：

```cmake
set(CMAKE_CXX_COMPILER "D:/Turing machine/Documents/mingw64/bin/g++.exe")
set(CMAKE_CXX_COMPILER_VERSION "8.1.0")
```

### 坑 4：`Pose` 没有 `operator==`，`ASSERT_EQ` 无法编译

**原因**：需求中的 `Executor.hpp` 只定义字段，不含比较运算符。

**解决**：按需求“若工程模板未提供，需在测试文件中补充”，在 `tests/ExecutorTest.cpp` 的 `namespace adas` 内定义：

```cpp
bool operator==(const Pose& lhs, const Pose& rhs) noexcept
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.heading == rhs.heading;
}
```

放在 `namespace adas` 内，`ASSERT_EQ` 才能通过 ADL 找到它；同时**未改动** `Pose` 的字段与接口头文件。

### 坑 5：如何让“先写测试”的中间提交**稳定失败**，而不是靠运气

**问题**：课件演示“构造函数不初始化 `pose` 导致用例失败”，但直接读取未初始化成员是 **未定义行为**，可能偶发通过，违反测试的 Repeatable（可重复）原则，也可能触发 `-Werror` 下的未初始化告警。

**解决**：第 2 步的实现骨架用**确定性占位**代替 UB：

```cpp
ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept { }   // 故意忽略入参
Pose ExecutorImpl::Query(void) const noexcept { return Pose{}; }  // {0,0,'\0'}
```

结果：编译成功，两个用例稳定失败（`heading` 为 `\0` 而非 `N`）；第 3 步再一次性修正为 `: pose(pose)` + `return pose;`。既满足“测试先行、允许失败提交”，又不引入 UB。

### 坑 6：`.gitignore` 未显式忽略 `bin/`

**现象**：模板已有 `**/build` 与 `**/*.exe`、`**/*.a`，但没有 `**/bin`。

**解决**：增加 `**/bin`，确保构建产物目录本身也不入库。`build/`、`bin/` 均未进入任何提交。

### 坑 7：`tests/googletest` 被模板 `.gitignore` 排除

**现象**：`tests/googletest` 不进入 Git 历史。

**影响**：课程要求的交付方式是“打包整个工程目录（含 `.git`）”，gtest 源码在磁盘上存在，构建不受影响；但**若从 GitHub 重新 clone，则缺少 gtest**。

**处理**：遵循模板约定未改动 `.gitignore`；在此明确记录该限制。若需要仓库可独立构建，可另起提交取消忽略 `tests/googletest`。

### 坑 8：模板内 LF / CRLF 混用

**现象**：`CMakeLists.txt` 为 LF，`script/*.bat` 为 CRLF；Git 开启 `core.autocrlf=true` 时提示“LF will be replaced by CRLF”。

**处理**：不手工改换行符，交给 Git 规范化；对编译与 TortoiseGit 查看日志均无影响。

### 坑 9：PowerShell 下执行当前目录程序

**现象**：直接输入 `Experiment4Student-main` 无法执行。

**解决**：`script/run.bat` 已封装为 `%EXE_FILE%` 调用，直接运行脚本即可；如手工执行，需写 `.\Experiment4Student-main.exe`。

---

## 五、验收自检清单（逐条核对）

| 检查项 | 结果 | 依据 |
|---|---|---|
| 14 个用例名与需求逐字一致 | ✅ 是 | 脚本 diff 通过，14/14 |
| 套件名为 `ExecutorTest` | ✅ 是 | 所有 `TEST(ExecutorTest, ...)` |
| `Query` 为 `const noexcept`、`Execute` 为 `noexcept`，均带 `override` | ✅ 是 | `ExecutorImpl.hpp:20-21` |
| `Executor` 无数据成员、不可拷贝、纯虚 | ✅ 是 | `Executor.hpp` 仅声明接口 |
| `NewExecutor` 默认实参 `{0,0,'N'}`，返回 `new (std::nothrow)` | ✅ 是 | `Executor.hpp:20`、`ExecutorImpl.cpp:108` |
| 构造在初始化列表初始化 `pose` | ✅ 是 | `ExecutorImpl.cpp:76` |
| 未初始化时返回 `(0,0,N)` | ✅ 是 | 默认实参 + 构造初始化 |
| M/L/R 四方向与矩阵一致 | ✅ 是 | 14 个用例全部通过 |
| 组合指令逐字符执行 | ✅ 是 | `for (const auto cmd : commands)` |
| 未实现范围外功能 | ✅ 是 | 无参数校验、无溢出保护、无日志 |
| C++17，保留 `std::nothrow` 写法 | ✅ 是 | `CMAKE_CXX_STANDARD 17` |
| 功能与测试分开提交 | ✅ 是 | 见提交统计 |
| 已创建 `lab1-cleancode-base` | ✅ 是 | 指向 `4ccee7d` |
| 已推送远程 | ✅ 是 | `origin/master` -> `master` |

---

## 六、复现与验证

```bat
cd /d D:\VSCodeProjects\C++Course\Experiment4Student

rem 1. 构建（需要 cmake 与 mingw32-make 在 PATH 中）
.\script\build.bat

rem 2. 运行全部测试
.\script\run.bat
```

预期输出结尾：

```
[==========] 14 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 14 tests.
```

若需核对提交日志：

```bash
git log --oneline --decorate
git branch -vv
git status --short
```

---

## 七、备注

1. 本报告用于复盘与交付说明，**未纳入上述 10 步提交序列**，以保持课程要求的提交日志纯净；如需入库，可追加一条 `feat: Add experiment report` 提交。
2. 交付打包时请确保包含 `.git` 目录，否则评阅方无法检查提交日志。
3. 工程内未写入任何本机绝对路径，可在其他安装 MinGW-w64 8.1.0 + CMake 的机器上直接构建。
