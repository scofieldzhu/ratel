# 通用 C++ 项目编码规范（C++20）

> 本文档定义通用 C++20 项目的代码风格与工程约定。
> 规范以「规则 + 正例/反例」形式给出，供代码评审与新人指引使用。
> 文档只描述风格与工程约定，不包含任何特定业务领域知识。

---

## 1. 语言与平台

- 使用 **C++20**（`CMAKE_CXX_STANDARD 20`）。
- 支持多平台；平台相关代码统一通过平台头中的条件编译宏隔离，不在业务代码中散落 `_WIN32` 等原生宏判断。
- 编译器警告与源文件编码选项由根构建脚本统一配置（例如 MSVC 的 `/W4 /utf-8`），各模块不自行重复。
- 注释、解释性文字使用**中文**（头文件 MIT 授权模板除外，见 §4）；日志消息使用英文。UI 可见字符串使用英文源文并经 Qt 翻译机制（`tr()` / Qt Linguist）处理，禁止硬编码中文。

## 2. 文件与目录组织

### 2.0 Qt 界面布局

- Qt 界面的静态控件、布局层级、间距和尺寸约束优先在 `.ui` 文件中定义，避免在 C++ 代码中通过 `new`、`addWidget`、`addLayout` 等方式构建界面。
- C++ 代码负责 ViewModel 绑定、信号槽连接、数据刷新及确有必要的运行时动态内容；只有布局内容在运行时不可预先确定或 Qt Designer 无法表达时，才允许用代码创建控件或布局。
- 同一组关联参数应按操作语义组织在同一行，例如速度与对应超时并排显示，并在 `.ui` 中定义其布局。

### 2.1 模块目录

源码按模块分层，每层一个目录，物理结构与逻辑模块一一对应：

```
src/
    app/                 # 应用装配、配置、日志、平台抽象
    core/                # 通用业务逻辑与领域服务
    model/               # 数据模型与接口
    ui/                  # 界面与视图（项目含 UI 时适用）
```

依赖规则：**低层模块不依赖高层模块**，高层依赖低层。
视图通过视图模型或直接接口调用业务能力，业务层依赖数据/接口抽象；依赖装配集中在应用层。

### 2.2 头文件命名与后缀

| 后缀 | 适用场景 | 示例 |
|---|---|---|
| `.h` | 普通类/函数声明 | `buffer.h`、`data_buffer.h` |
| `.hpp` | 模板/头文件实现（template 或 header-only） | `value_proxy.hpp`、`defer_exec.hpp` |
| `.proto` 等接口定义文件 | 跨模块或跨进程数据契约（适用时） | `sample.proto` |

模板实现不放入 `.cpp`，直接写在 `.hpp`。

## 3. 头文件规范

### 3.1 Include Guard

所有头文件使用 `#ifndef` 风格的 include guard，宏名为 `__<文件名去后缀>_h__`（模板头用 `_hpp__`）。

```cpp
#ifndef __buffer_h__
#define __buffer_h__
// ...
#endif
```

**反例**：不要用 `#pragma once` 与 `#ifndef` 混用；guard 宏必须唯一且与文件名一致。

### 3.2 包含顺序（.cpp）

```cpp
#include "本模块头文件"                      // 1. 自身头文件（最先，用于自检）
#include "module/other_component.h"          // 2. 跨模块头文件（按模块字母序）
#include <标准库头文件>                      // 3. 标准库
#include "third_party/library.h"             // 4. 第三方库
```

跨模块引用一律使用相对 `src/` 的完整路径：`#include "module/other_component.h"`。

### 3.3 头文件内容纪律

- 头文件中尽量**只做声明**；模板、inline、constexpr 除外。
- 公开头文件只包含它真正依赖的头，能用前向声明就不包含定义。
- 类成员包含 `#include <memory>` 相关智能指针（见 §9）。

## 4. 文件头注释模板

每个新文件必须携带 MIT 授权注释头，使用 `<起始年>`、`<当前年>` 和 `<创建者>` 占位。
IDE 模板（如 `.vscode/templates/` 下的 `h.lict` / `hpp.lict` / `cpp.lict`）中维护对应模板，新建文件时直接套用。

```cpp
/*
 *   <项目简介一行>
 *
 *   File: <文件名>
 *   Creator: <创建者>
 *   Copyright (c) <起始年> ~ <当前年> <版权所有者>. All rights reserved.
 */
```

## 5. 命名空间

遵循所在模块现有命名空间，不额外引入命名空间包装宏。

- **头文件中禁止 using namespace**；实现文件也优先显式限定第三方名称。
- 文件内部使用的辅助类型放入**匿名命名空间**，不要暴露到头文件：

```cpp
namespace {
    using Indices = std::array<unsigned int, 3>;
}
```

- **命名空间体内每一行代码**（含匿名命名空间、嵌套命名空间、注释行与预处理指令）相对 `namespace` 声明行统一缩进**四个空格字符**，内部层级再按 4 空格累加；`namespace` 声明行与闭合 `}` 行本身不缩进。这里说的「一个 tab 宽度」就是 4 个空格字符，**仍然不插入 `\t`**，§8.1「缩进一般为四个空格字符，不要使用 tab 键」不变 —— 两条是同一口径，不是特例。

机械校验：由项目约定的代码风格检查脚本执行（返回非零即有违规，代码审查必跑；同一脚本同时按 §8.1 检查全库是否残留真实 `\t`）；批量修正使用同一套判定逻辑，检查与改写不得各写一套，否则会出现「门禁 0 违规、改写器却把形态改回去」的静默失配。

## 6. 命名规范

### 6.1 标识符风格总览

| 对象 | 风格 | 示例 |
|---|---|---|
| 类 / 结构体 / 枚举类型 / 概念 | PascalCase | `DataBuffer`、`ImageData`、`RunContext` |
| 成员函数 / 普通函数 | camelCase，动词开头 | `getIntValue`、`serializeToBytes`、`createBuffer` |
| 类成员变量 | snake_case + 尾下划线 `_` | `id_`、`buffer_dict_`、`context_` |
| 结构体成员变量 | snake_case（无尾下划线） | `use_timer`、`on_complete`、`count` |
| 全局/局部变量 | snake_case | `file_path`、`total_size` |
| 编译期常量、静态 constexpr | `k` 前缀 + PascalCase | `kDefaultSize`、`kZeroValue` |
| 宏 / 预处理 | 全大写下划线 | `LOG_INFO`、`PLATFORM_WIN` |
| 模板形参 | 单大写字母或 Pascal | `T`、`K`、`V`、`Functor` |

### 6.2 命名细节

- **结构体成员变量**：snake_case、无尾下划线（结构体成员多为公开数据直接 `.` 访问，尾下划线仅作为类私有成员标记）。
- **布尔取值/查询**：`get`/`is`/`has` 前缀，如 `isStaticArray()`、`existsItem()`。
- **可变访问**：只读访问 `data()const`，可变访问 `mutableData()`，成对出现。
- **序列化对**：字节序、长度、版本和失败语义保持配对一致（见 §11），遵循现有接口。
- **静态工厂**：`static` 工厂方法用 PascalCase。
- **类型别名**：统一用 `using`，不用 `typedef`（历史兼容别名除外）。

### 6.3 类型别名后缀约定

| 后缀 | 含义 | 示例 |
|---|---|---|
| `SPtr` | `std::shared_ptr<T>` | `BufferSPtr` |
| `Ptr` | 裸指针 `T*`（非拥有） | `BufferPtr`、`SessionPtr` |
| `UPtr` | `std::unique_ptr<T>` | `BufferUPtr` |
| `List` | 容器列表别名 | `BufferList` |
| `Dict` | 键值容器 | `KeyValueDict` |

## 7. 枚举与常量

- 强类型枚举用 `enum class`，成员加 `k` 前缀：

```cpp
enum class DataType { kUnknown, kInt, kFloat, kString, kBytes, kImage };
```

- 跨 C 边界的朴素枚举，成员用模块前缀的 `MODULE_` 风格（如 `ImageData::DT_UCHAR`）。

- 整数常量使用 `constexpr`（如 `constexpr size_t kDefaultSize = sizeof(int);`），命名以 `k` 开头。

- 语义常量集中放在 `*_typedef.h` 或最近的命名空间内，不散落各处。

- 数学常量与单位换算不写裸字面量：弧度↔角度一律使用项目统一的数值工具函数（如 `math_util::radToDeg()` / `math_util::degToRad()`），π 使用标准库常量或工具提供的 `kPi`。禁止出现 `180.0 / 3.14159265358979323846`、`57.29577951308232`、`3.14159... / 180.0` 这类写法 —— 抄错一位不报错，只让日志与界面阈值悄悄偏掉。界面层不引底层数值模块，改用 Qt 的 `qRadiansToDegrees()` / `qDegreesToRadians()`（`#include <QtMath>`）。

## 8. 空格缩进与代码结构

### 8.1 空格与缩进

- 关键字后面跟大括号或者小括号的，中间不要加空格：`if(1)` 对，`if (1)` 错。
- 小括号后面跟大括号或者大括号后面跟关键字的，中间不要加空格。
- 三元操作符语句用一行写，不要分成多行。
- 缩进一般为四个空格字符，不要使用 tab 键（全库任何位置都不允许出现真实 `\t`，机械校验见 §5）。
- `switch` 的 `case` 标签相对 `switch` 关键字缩进四个空格。

### 8.2 语句与代码块

- **一行只写一条语句**：禁止在同一行用分号拼接多条语句；单条语句体也不得与 `if` 条件写在同一行。
  - 反例：`int a = 1; int b = 2;`、`if(ok) return;`
- **控制流必须加大括号**：`if` / `while` / `for` 的语句体必须用 `{}` 包裹，即使只有一条语句也不得省略大括号。
  - 反例：`if(ok) return true;`
  - 正例：`if(ok) { return true; }`
- **大括号与后接的else if必须写在同一行，不要分成两行来写**

### 8.3 类声明结构

- **访问域唯一**：类声明中 `public:` / `protected:` / `private:` 各只出现一次，成员按访问域分组排列，不得重复声明同一访问域。
- **构造/析构置于访问域末尾**：构造函数与析构函数必须放在其所在访问域（public/protected/private）的最后，普通成员函数在前。

  ```cpp
  class Foo
  {
  public:
      void doSomething();      // 普通成员函数在前
      int  getValue() const;

      Foo();                   // 构造函数放 public 域末尾
      ~Foo();                  // 析构函数紧随其后

  private:
      int value_;
  };
  ```

### 8.4 构造初始化列表

- **逗号后置**：类初始化成员列表中成员变量赋值时，如需逗号，逗号一律放在上一项末尾（行尾），禁止放在下一项开头（行首）。

  ```cpp
  // 正例：逗号放在上一项末尾
  Foo::Foo(int a, int b)
      :a_(a),
       b_(b)
  {}

  // 反例：逗号放在下一项开头
  Foo::Foo(int a, int b)
      :a_(a)
      , b_(b)
  {}
  ```

### 8.5 参数列表换行

适用于**函数调用、函数声明、函数实现的形参列表**（含回调注册、信号槽连接等调用）。`if` / `for` / `while` / `switch` 的条件表达式不是参数列表，不适用本节。

- **参数 ≤ 5 个且无 lambda**：全部参数写在**同一行**，禁止折行。
- **参数 > 5 个**：**一个参数一行**，非最后一个参数行尾保留逗号，以右括号和分号单独作为一行结束。
- **含 lambda 参数**：整个参数列表按「一个参数一行」展开，lambda 从自己那行开始，函数体内语句相对 lambda 所在行缩进四个空格。
- **续行缩进**：所有续行统一取**语句缩进 +4**，不对齐左括号（左括号靠右时对齐会产生超深缩进）。
- **列宽优先**：合并后单行超过 160 列时改为**一个参数一行**（行宽见 §8.6）。这是本节唯一的例外，不许为了「塞进一行」把行撑过 160 列。
- **折行形态**：凡折行的参数列表，一律 `(` 后即换行、**首参数另起一行**（缩进与其余续行相同），每层 `)` 独占一行，最外层 `)` 与 `;` 同处语句缩进的独立一行；嵌套调用层层展开，不许把内层 `)` 与外层 `)` 挤成 `));`。唯一的链式例外：`QString::arg()` 链紧贴其右括号写在同一行（形如 `).arg(text);`），不另起一行。
- **字符串不人为分段**：一个字符串字面量在「合并后整行 ≤160 列」时必须写成**单个字面量、占一行**，不许拆成多个相邻字面量拼接。确实超宽时才按行宽分段，切点只允许落在原有字面量边界；前段尾部存在未完成转义（`\x` / `\u` / 八进制未吃饱 / 奇数个 `\`）时禁止在该点合并，否则会吞掉下一段首字符、改变字符串语义。

正例（4 个参数 → 单行）：

```cpp
connect(ui_.filterButton, &QPushButton::clicked, this, &Page::applyFilter);
```

反例（4 个参数却折行）：

```cpp
connect(
    ui_.filterButton,
    &QPushButton::clicked,
    this,
    &Page::applyFilter
);
```

正例（6 个参数 → 一参一行，续行 = 语句缩进 +4）：

```cpp
const int converted = convertText(
    source,
    offset,
    length,
    flags,
    output,
    error
);
```

反例（参数挤成两行、续行还对左括号）：

```cpp
const int converted = convertText(source, offset, length,
                                  flags, output, error);
```

正例（含 lambda → 整个列表展开）：

```cpp
connect(
    model_,
    &ItemModel::dataChanged,
    this,
    [this](){
        refreshCounters();
    }
);
```

反例（lambda 塞在参数行末尾）：

```cpp
connect(model_, &ItemModel::dataChanged, this, [this]() {
    refreshCounters();
});
```

正例（嵌套调用层层展开，每层 `)` 独占一行，首参数一律另起一行）：

```cpp
backgroundWatcher.setFuture(
    QtConcurrent::run(
        [task] {
            return task->execute();
        }
    )
);
```

反例（首参数紧跟 `(`，内外层 `)` 挤成 `));`）：

```cpp
backgroundWatcher.setFuture(QtConcurrent::run([task] {
    return task->execute();
}));
```

机械校验：由项目约定的参数格式检查脚本执行（返回非零即有违规，代码审查必跑）；批量改写使用同一套检测逻辑，检查器与改写器不得各写一套，否则会出现「门禁 0 违规、改写器却把形态改回去」的静默失配。改写器只增删空白，落盘前比对 token 流，超行宽或含注释/原始字符串的列表一律跳过交人工处理。

### 8.6 行宽

- **每行最多 160 列**（中文/全角字符按 2 列计），超出一律折行。注释行同样受限。
- 折行位置按下列优先级选：参数列表按 §8.5；赋值/返回的表达式按 §8.7 先在操作符处保持单行，放不下就把表达式拆成具名中间量或辅助函数；字符串拼接拆成多条语句；大括号初始化列表一项一行。
- 行宽是**硬上限**：任何其它排版规则（含 §8.5「≤5 个参数写一行」）与它冲突时，先满足行宽。

```cpp
// 反例：注释跟着值一起拖长
file << "  max_retry: " << config.max_retry << "   # 最大重试次数（0 表示不限制）\n";

// 正例：注释独立成行，代码行保持短
file << "  # 最大重试次数（0 表示不限制）\n";
file << "  max_retry: " << config.max_retry << "\n";
```

机械校验：由项目约定的行布局检查脚本执行（返回非零即有违规）。

### 8.7 二元操作符与条件运算符

- **二元操作符不得断行**：`+ - * / % << >> & | ^ && || == != < > <= >=` 及其复合赋值，
  整个表达式写在一行；不许「行尾留操作符」，也不许「续行以操作符开头」。
- **`?:` 条件运算符同样不跨行**：`?` 与 `:` 必须和三个操作数在同一行；
  「`cond` 一行、`? a` 一行、`: b` 一行」是最常见的违规形态。
- 表达式长到一行放不下时（§8.6 硬上限），**改写而不是断行**：提具名布尔量、抽辅助函数、
  把链式判断换成 `switch`、把三元换成 `if/else`。禁止用「操作符换行」当折行手段。

```cpp
// 反例：`||` 换行
const bool blocked = state == State::jog
                  || state == State::jogging
                  || state == State::drag;

// 正例：抽成辅助函数，调用点一行
const bool blocked = isMotionBlockedState(state);
```

```cpp
// 反例：三元拆三行（`?` / `:` 各占一行）
const Config& config = isPrimary
    ? primaryConfig()
    : fallbackConfig();

// 正例一（合起来不超 160）：提公共前缀，压成一行
const bool use_primary = isPrimary;
const Config& config = use_primary ? primaryConfig() : fallbackConfig();

// 正例二（合起来必然超 160）：三元改写成分支，别硬凑一行
if(isPrimary) {
    applyConfig(primaryConfig());
} else {
    applyConfig(fallbackConfig());
}
```

- 分支各自是**独立翻译条目**时（如 `tr("Open Device")` 与 `tr("Close Device")`），
  不得为了缩短行宽把字符串拼起来（`tr("Open ") + tr("Device")` 会让翻译失效），
  一律展开成 `if/else if/else` 逐条 `setText`。

机械校验：由项目约定的行布局检查脚本执行（二元与三元共用同一套断行判定）；
能安全合并的由批量工具直接合并，合并后仍 >160 列的只报告行号，交给人工改写。

## 9. 内存与指针管理

- **拥有关系**一律用 `std::shared_ptr` / `std::unique_ptr`，对应别名 `*SPtr` / `*UPtr`。
- **非拥有引用**用裸指针 `*Ptr`，并在注释或命名上表明不拥有。
- 工厂方法返回 `shared_ptr` 时可使用自定义删除器：

```cpp
BufferSPtr buffer(new_buffer, destroy_buffer_func);
```

- 禁止裸 `new`/`delete` 散落业务代码；优先用 `std::make_shared` / `std::make_unique` 管理生命周期。
- 成员变量尽量在声明处初始化（`int number = 0;`、`bool stopped_ = false;`），构造列表只做必要初始化：

```cpp
Session::Session(const std::string& id)
    : id_(id)
{}
```

## 10. 导出宏与平台宏

### 10.1 导出宏

平台导出基础宏统一定义在项目的平台头（如 `platform.h`）：

```cpp
#include "platform.h"
// PROJECT_WIN_DLL_EXPORT：导出符号
// PROJECT_WIN_DLL_IMPORT：导入符号
```

- 项目含动态库时，按实际导出边界定义模块 API 宏；纯可执行目标不为内部类强加 DLL 导出宏。
- 模板、inline 函数不需要导出宏。
- 各模块**不重复**定义平台导出基础宏。

### 10.2 平台宏

平台检测统一在平台头中完成，业务代码**不要自己判断** `_WIN32` 等原生宏：

- `PLATFORM_WIN`、`PLATFORM_LINUX`、`PLATFORM_UNKNOWN`

### 10.3 C 接口

DLL 对外导出 C 函数时用 `PROJECT_EXTERN_C_BEGIN` / `PROJECT_EXTERN_C_END`（定义于平台头）包裹。

## 11. 序列化约定

新增字节序列化接口可使用标准容器，以下仅为签名示例，不改变既有接口：

```cpp
std::vector<std::uint8_t> serializeToBytes() const;
std::size_t loadBytes(const std::uint8_t* buffer, std::size_t size); // 返回消费字节数，失败返回 0
```

- 组合对象按协议约定顺序编码，显式检查长度、字节序和边界。
- `loadBytes` 必须返回**已消费的字节数**，供外层顺序反序列化；失败/越界返回 `0`。
- 不直接序列化结构体内存；新增格式不得破坏既有协议与配置兼容性。

## 12. 错误处理与日志

- 失败返回约定：返回 `bool` 的函数 `true=成功`；返回指针的失败返回 `nullptr`；序列化返回 `0`。
- **禁止吞异常**：本仓库风格不使用异常做流程控制，错误以返回码 + 日志呈现。
- 日志统一使用项目日志头（如 `log.h`）中的封装宏，按严重级别选择：
  - `LOG_TRACE(...)` — 调试跟踪
  - `LOG_WARN(...)` — 可恢复异常情况
  - `LOG_ERROR(...)` — 失败并带有具体上下文（传入参数值）
- 日志消息一律使用英文。
- 日志消息要带**上下文信息**（如：`"Load item:[{}] {}!"`），便于定位。

## 13. 并发

- 共享状态用 `std::mutex` + `std::lock_guard` 保护，锁生命周期最小化：

```cpp
bool Session::run()
{
    std::lock_guard lock(run_mutex_);
    // ...
}
```

- 线程池、异步任务优先使用项目已有的 `QtConcurrent::run` / `QFutureWatcher` 模式，不自行裸管理线程生命周期。
- 跨线程自增 ID 使用原子操作；时间戳单位和时钟来源须保持明确，不能以普通变量读写替代同步。

## 14. 构建与模块（CMake）

项目由根 `CMakeLists.txt` 统一管理库与可执行目标，关键结构示例：

```cmake
add_library(core SHARED ${CORE_SOURCES})
add_executable(app ${APP_SOURCES})
set_target_properties(core PROPERTIES
    DEBUG_POSTFIX "_d"
)
```

- 目标名称由项目自身约定，不引入虚构的编译选项或头文件辅助目标。
- Debug 产物默认带 `_d` 后缀；构建目录与输出布局由构建预设统一管理。
- 多配置生成器按构建或安装时选择的配置处理依赖，不用 `CMAKE_BUILD_TYPE` 判断部署文件。
- 源文件、包含路径、依赖与安装规则统一维护在根构建脚本中。

## 15. 代码示例

### 15.1 头文件（声明）

```cpp
#ifndef __sample_config_h__
#define __sample_config_h__

#include <string>

// 命名与错误处理示例，不代表已实现的业务组件。
class SampleConfig
{
public:
    bool validate()const;

    explicit SampleConfig(const std::string& name);

private:
    std::string name_;
};

#endif
```

### 15.2 实现文件

```cpp
#include "sample_config.h"
#include "log.h"

SampleConfig::SampleConfig(const std::string& name)
    :name_(name)
{}

bool SampleConfig::validate()const
{
    if(name_.empty()){
        LOG_ERROR("Configuration validation failed: name is empty, length={}", name_.size());
        return false;
    }
    return true;
}
```

## 16. 评审要点清单

新增/修改代码时逐条自查：

- [ ] 文件头 MIT 注释与作者、年份正确
- [ ] include guard 唯一且与文件名一致
- [ ] 头文件无 `using namespace`；.cpp 包含顺序正确（自身头文件优先）
- [ ] 命名空间（含匿名）体内每行缩进 4 个空格字符，内部层级继续 +4；**全库不出现真实 `\t`**（§5 / §8.1，由项目约定的检查脚本执行）
- [ ] 命名符合 §6 风格（成员尾下划线、`k` 前缀常量、`SPtr/Ptr` 别名），注意空格问题
- [ ] 一行一条语句；`if`/`while`/`for` 语句体必加 `{}`（§8.2）
- [ ] `switch` 的 `case` 标签相对 `switch` 缩进四个空格（§8.1）
- [ ] 类声明访问域唯一；构造/析构函数置于所在访问域末尾（§8.3）
- [ ] 构造初始化列表逗号后置，不放行首（§8.4）
- [ ] 参数列表换行符合 §8.5：≤5 个且无 lambda 单行写完；>5 个、含 lambda 或合并后超 160 列时一参一行、续行缩进 +4；折行时首参数另起一行、每层 `)` 独占一行，字符串在行宽内不分段（由项目约定的参数格式检查脚本执行）
- [ ] 每行 ≤160 列，超长靠折行/具名中间量/辅助函数解决，不靠注释跟值硬拖（§8.6，由项目约定的行布局检查脚本执行）
- [ ] 二元操作符与 `?:` 都不换行：无行尾操作符、无续行首操作符，`?` `:` 与三个操作数同一行（§8.7）
- [ ] UI 可见字符串使用英文源文并经 `tr()` 处理，无硬编码中文
- [ ] Qt 界面静态布局优先在 `.ui` 中定义，C++ 代码只处理绑定与动态内容（§2.0）
- [ ] 翻译条目保持独立，不拼接 `tr()` 字符串（§8.7）
- [ ] 异步任务使用 `QtConcurrent::run` / `QFutureWatcher` 统一模式（§13）
- [ ] 平台宏复用平台头，仅在实际 DLL 导出边界使用导出宏
- [ ] 序列化遵循既有协议；示例 `loadBytes` 返回消费字节数
- [ ] 日志用 `LOG_*`、英文且带上下文；失败返回码约定统一
- [ ] 跨模块引用使用相对 `src/` 的完整路径
- [ ] 新增模块按 §14 接入根 CMakeLists
