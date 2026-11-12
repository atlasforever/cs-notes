# STM32 + FreeRTOS 学习进度

## 当前状态

- 当前章节：第 0 章——开发环境搭建
- 当前状态：基本完成，待补充 GDB 单步验收
- 当前掌握等级：L3（实验能力已达到，待完成最后一项形式验收）
- 下一章节：第 1 章——ARM Cortex-M3 处理器架构

## 第 0 章：开发环境搭建

### 已完成内容

| 小节 | 学习内容 | 状态 | 等级 |
|---|---|---|---|
| 0.1 | WSL2、Native GCC | 已完成 | L3 |
| 0.2 | Arm GNU Toolchain、交叉编译 | 已完成 | L3 |
| 0.3 | CMake 多目标工程配置 | 已完成 | L3 |
| 0.4 | FreeRTOS POSIX 环境 | 已完成 | L3 |
| 0.5 | QEMU Cortex-M3 环境 | 已完成 | L3 |
| 0.6 | GDB 远程调试 | 基本完成 | L3 |
| 0.7 | 综合验收 | 待完成 | - |

### 已掌握知识

- 区分 x86-64 Native GCC 和 ARM Cortex-M3 交叉编译工具链。
- 理解 ELF、目标文件、机器指令和反汇编之间的关系。
- 理解 CMake Toolchain File 和独立 Build Directory。
- 理解 FreeRTOS Kernel 与 Port Layer 的职责划分。
- 区分 FreeRTOS POSIX Port 和 Cortex-M3 Port。
- 理解 QEMU MPS2 AN385 的基本用途和局限。
- 理解裸机固件与 Linux 应用程序的运行环境区别。
- 理解 Reset_Handler、异常向量表和基本 C 运行环境初始化。
- 能够使用 GDB 连接 QEMU、设置断点、查看寄存器和内存。

### 已完成实验

1. Native GCC 编译并运行 Linux 原生程序。
2. Arm GCC 编译 Cortex-M3 目标文件。
3. 使用 readelf 和 objdump 分析 ARM ELF。
4. 使用 CMake 构建 Native 和 Cortex-M3 两种目标。
5. 编译并运行 FreeRTOS V11.3.3 POSIX 示例。
6. 编译并运行 Cortex-M3 裸机 UART 示例。
7. 编译并运行 FreeRTOS V11.3.3 Cortex-M3 示例。
8. 使用 GDB 连接 QEMU，观察 Reset_Handler、main 和 demo_task。

### 待完成事项

- 在 GDB 中执行一次 `si`，验证单指令步进。
- 完成第 0 章最终验收。

### 需要后续深入学习的知识

以下内容已经接触，但不在第 0 章展开：

- FreeRTOS 任务状态和调度机制：第 4 章。
- TCB、任务栈和任务管理：第 5 章。
- Cortex-M3 上下文切换：第 6 章。
- Cortex-M3 异常与 NVIC：第 3 章。
- 向量表、启动代码和链接脚本：第 2 章。

### 下一步

进入第 1 章：ARM Cortex-M3 处理器架构。

重点学习：
- ARMv7-M 架构概览。
- R0～R15 通用寄存器。
- PC、LR、SP、xPSR。
- Thread Mode 与 Handler Mode。
- 特权级。
- MSP、PSP 和 CONTROL。
- Cortex-M3 内存空间与寄存器映射。
