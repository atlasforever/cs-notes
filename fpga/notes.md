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
