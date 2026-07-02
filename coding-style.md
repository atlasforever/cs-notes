# C++ Coding Style

使用 https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines

## Naming & Layout

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

## Source files

### 头文件顺序

```c
// 1. 当前 .cpp 对应头文件
#include "xxx.h"

// 2. C/C++ 标准库
#include <...>

// 3. 第三方库 / SDK
#include <...>

// 4. 项目内部其他头文件
#include "..."
```