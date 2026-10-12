# C++ Coding Style

遵守 C++ Core Guildlines <https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines>

## Naming & Layout

### 命名

* 文件
  * lower_sname_case.cpp
  * lower_sname_case.h
* 类型
  * UpperCamelCase
* 变量
  * lowerCamelCase
  * 成员变量 `m_` 前缀
* 函数
  * lowerCamelCase
* namespace
  * lower_snake_case

### 文件内部定义

SF.22: Use an unnamed (anonymous) namespace for all internal/non-exported entities

内部的辅助类型和函数优先放在匿名 namespace 中：

```cpp
// bad
static int f();
int g();
static bool h();
int k();

// good
namespace {
    int f();
    bool h();
}
int g();
int k();
```

### 括号

```cpp
/// Wrong.
if (condition)
    do_something;

/// Correct
if (condition) {
    do_something;
}
```

### 成员声明顺序

NL.16: Use a conventional class member declaration order

类成员顺序：

* types: classes, enums, and aliases (using)
* constructors, assignments, destructor
* functions
* data

控制顺序：

* public
* protected
* private

### 头文件顺序

```c
source.h
local headers
third-party headers
framework headers (QT)
c headers
cpp headers
```
