# Verilog

## 语法

### 数值系统

除了 0 和 1，还有：

* x：未知值
* z：高阻态

数字格式：
```
<bits>'<radix><value>
```

例如：
```
8'b1111_0000
16'h1234
32'd1000
```

位宽不足，高位可能被截断。

### wire 和 reg

`wire` 可以理解成“导线”，通常由连续赋值或模块输出驱动。

```verilog
wire y;
assign y = a & b;
```

`reg` 可以在 always、initial 等过程块中被赋值。

``` verilog
reg y;

always @(*) begin
    y = a & b;
end
```

reg 不代表一定会综合成寄存器。生成什么硬件，主要取决于代码描述的逻辑，而不是单纯取决于 wire 或 reg。

### 运算符

算术运算
`+ - * / %`

比较运算
`> < >= <= == !=`

逻辑运算
`&& || !`

位运算s
`& | ^ ~`

### assign 和 always

assign 连续赋值，通常用于描述组合逻辑。

左边通常是 wire。

```verilog
wire y;

assign y = sel ? a : b;
```

always 过程块：

```verilog
always @(*) begin
    y = a & b;
end
```

组合逻辑通常写：

```verilog
always @(*)
```

时序逻辑通常写：

```verilog
always @(posedge clk)
```

### 阻塞赋值 = 与非阻塞赋值 <=

阻塞赋值 `=` 语句按顺序立即执行。通常用于组合逻辑（`always @(*)`）：

```verilog
// 第二句读取到的是已经被第一句修改后的 a。
always @(*) begin
    a = b;
    b = a;
end
```

非阻塞赋值 `<=` 语句同时间执行，通常用于时序逻辑（`always @(posedge clk)`）：

```verilog
// 这里可以实现 a 和 b 的值交换。
// 因为两句话读取到的都是时钟沿到来之前的旧值。
always @(posedge clk) begin
    a <= b;
    b <= a;
end
```

***推荐规则***

```text
组合逻辑 always @(*)       -> 使用 =
时序逻辑 always @(posedge) -> 使用 <=
testbench initial          -> 通常使用 =
```

同一个过程块不要混用 = 和 <=。

### if 与 case

if 语句通常被综合成 2:1 MUX

### module

verilog 的基本单元是 module：

```verilog
module MyModule (
    input  a,
    input  b,
    output y
);

assign y = a & b;

endmodule
```

模块实例化：

```verilog
FA fa (
    .a(num1),
    .b(num2),
    .cout(cout)
);
```

`parameter` 用于创建可配置模块：

```verilog
// 模块定义
module MUX2 #(
    parameter WIDTH = 8
)(
    input  [WIDTH-1:0] num1,
    input  [WIDTH-1:0] num2,
    input              sel,
    output reg [WIDTH-1:0] ans
);

// 实例化修改参数
MUX2 #(
    .WIDTH(4)
) u_mux (
    .num1(num1),
    .num2(num2),
    .sel(sel),
    .ans(ans)
);
```

## 信号

### 时钟信号

时钟决定时序逻辑什么时候更新。

```verilog
always @(posedge clk) begin
    q <= d;
end
```

### 复位信号

上电后为系统设置一个稳定的初始状态：

```verilog
always @(posedge clk) begin
    if (rst)
        counter <= 0;
    else
        counter <= counter + 1;
end
```

不建议使用 `reg [7:0] counter  = 0`，`rst` 更加可控。

*同步复位* 是和时钟信号同步的复位：

```verilog
// rst 不在敏感列表中
always @(posedge clk) begin
    if (rst)
        q <= 0;
    else
        q <= d;
end
```

*异步复位* 可以立即触发，但**尽量避免**

```verilog
always @(posedge clk or posedge rst) begin
    if (rst)
        q <= 0;
    else
        q <= d;
end
```

## 并行

不同的 `always`、`initial`、`assign` 是并行执行的。

块内语句：

* 阻塞赋值 `=` 串行执行（但如果语句间没有依赖也会并行）
* `<=` 并行执行，同变量多次赋值时最后的覆盖前面

## 锁存器

组合逻辑里为了让它“保持旧值”，就会生成 Latch：

* 如果某些情况下没有给输出赋值
  * 解决方法：*组合电路中，每个输出，在所有路径下都有明确赋值*
* 如果一个信号的赋值源头，或者判断条件中有其信号本身的逻辑有其信号本身（需要记住旧值）
  * 解决方法：避免如此设计，或者使用时序逻辑

```verilog
// 为了 en = 0 时保持旧值 
always @(*) begin
    if (en)
        q = data;
end

reg a, b;
always @(*) begin
    if (a & b)  
        a = 1'b1;   // a 会生成锁存器
    else 
        a = 1'b0;
end

always @(*) begin
    if (en)
        a = a + 1; // a 会生成锁存器
end
```

解决方法：

```verilog
// 开头给默认值更好
always @(*) begin
    q = 0;

    if (en)
        q = data;
end

always @(*) begin
    if (en)
        q = data;
    else
        q = 0;
end

// 时序逻辑的触发器避免锁存器
always @(posedge clk) begin
    if (en)
        a <= a + 1;
end
```

## 信号处理

### 去毛刺

输入必须连续保持一段时间，才认为它真的有效。

```verilog
// 持续超过一定时钟数后，才认为按键真的按下
always @(posedge clk) begin
    if (!btn)
        cnt <= 0;
    else if (cnt < 8)
        cnt <= cnt + 1;
end

assign btn_clean = cnt[3];
```

### 边沿检测

边沿检测核心为：保存当前值和上一拍的值，然后比较。

同步检测，用两级触发器保存两次上升沿的值，进行比较：

```verilog
reg sig_r1, sig_r2;

always @(posedge clk) begin
    sig_r1 <= sig_in;   // sig_r1 = 当前采样值
    sig_r2 <= sig_r1;   // sig_r2 = 上一拍的 sig_r1
end

assign pos_edge = sig_r1 & ~sig_r2;
assign neg_edge = ~sig_r1 & sig_r2;
```

异步检测时，如果 `sig_in` 和 `clk` 不同步的，可能引起亚稳态导致输出不确定。

需要额外加一个触发器减少亚稳态概率（三个触发器，或者更多以确保更稳定）。

## 常见元件

### 编码器

编码器：将多路输入中有效的那一路，转换成二进制编号：

```verilog
always @(*) begin
    case (I)
        4'b1000: Y = 2'b11;
        4'b0100: Y = 2'b10;
        4'b0010: Y = 2'b01;
        4'b0001: Y = 2'b00;
        default: Y = 2'b00;
    endcase
end
```

优先编码器允许多路有效，按照优先级输出最高的一路：

```verilog
always @(*) begin
    valid = 1;

    if (I[3])
        Y = 2'b11;
    else if (I[2])
        Y = 2'b10;
    else if (I[1])
        Y = 2'b01;
    else if (I[0])
        Y = 2'b00;
    else begin
        Y = 2'b00;
        valid = 0;
    end
end
```

### 译码器

将二进制编号输入转换为 “选中某一路”：

```verilog
module Decoder2to4 (
    input  wire [1:0] in,
    output reg  [3:0] out
);

always @(*) begin
    case (in)
        2'b00: out = 4'b0001;
        2'b01: out = 4'b0010;
        2'b10: out = 4'b0100;
        2'b11: out = 4'b1000;
    endcase
end

endmodule
```

### 加法器

* 1bit 半加器 `A + B → Sum + Carry`
  * sum：当前位结果
  * carry：向高位的进位
* 1bit 全加器 `A + B + Cin → Sum + Cout`
  * Cin：来自低位的进位

### 移位寄存器

移位寄存器（Shift Register）：多个触发器串联，每来一个时钟，数据整体移动一格。

* 常用于：串并转换、延时、流水线、边沿检测等。

```verilog
always @(posedge clk) begin
    data[7:1] <= data[6:0];
    data[0]   <= data_in;
end
```

### 寄存器堆

寄存器堆（Register File）：时序逻辑元件，由多个可读写寄存器组成的存储器：

```verilog
module RegFile (
  input                     clk,          // 时钟信号
  input         [1:0]       ra1,          // 读端口 1 地址
  input         [1:0]       ra2,          // 读端口 2 地址
  input         [1:0]       wa,           // 写端口地址
  input                     we,           // 写使能信号
  input         [31:0]      din,          // 写数据
  output        [31:0]      dout1,        // 读端口1数据输出
  output        [31:0]      dout2         // 读端口2数据输出
);

reg [31:0] reg_file [3:0]; // 4 个 32 位寄存器，规模为 4×32 bits

// 读端口 1
assign dout1 = reg_file[ra1];

// 读端口 2
assign dout2 = reg_file[ra2];

// 写端口
always @(posedge clk) begin
    if (we) begin
        reg_file[wa] <= din;
    end
end

endmodule
```

### FIFO

双端口 RAM 的变体，实现先进先出功能，通常包含：

* 存储器 mem
* 写指针 wr_ptr
* 读指针 rd_ptr
* full：FIFO 已满
* empty：FIFO 已空

不像移位寄存器需要每次移动所有数据，而是靠内部的读写指针。

```verilog
reg [7:0] mem [0:15];
reg [3:0] wr_ptr;
reg [3:0] rd_ptr;

always @(posedge clk) begin
    if (wr_en) begin
        mem[wr_ptr] <= wr_data;
        wr_ptr <= wr_ptr + 1;
    end

    if (rd_en) begin
        rd_ptr <= rd_ptr + 1;
    end
end

assign rd_data = mem[rd_ptr];
```

### 有效状态机 FSM

有限状态机组成：

1. 当前状态 state
1. 根据当前状态和输入，计算下一状态 next_state
1. 根据状态产生输出

一段式状态机用一个 always 进行状态转移和执行动作，不易出错：

```verilog
always @(posedge clk) begin
    if (rst) begin
        state <= IDLE;
        done  <= 0;
    end else begin
        case (state)
            IDLE: begin
                done <= 0;
                if (start)
                    state <= WORK;
            end

            WORK: begin
                if (finish)
                    state <= DONE;
            end

            DONE: begin
                done  <= 1;
                state <= IDLE;
            end

            default:
                state <= IDLE;
        endcase
    end
end
```