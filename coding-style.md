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
  * UpperCamelCase
* namespace
  * lower_snake_case

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
