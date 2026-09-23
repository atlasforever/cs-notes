// 题目 3：计数器 Pro（3 分）
// 1. 现在的计数器复位值是 0，最小值是 0，最大值是 MAX_VALUE。请修改代码，使得计数器的最小值也可以由模块参数 MIN_VALUE 指定。
// 2.在 1 的基础上，增加一个 1bit 输入信号 enable 用于控制计数器的工作状态。要求：
//      enable 为高电平时，计数器在 MIN_VALUE 至 MAX_VALUE 之间正常工作；enable 为低电平时，counter 变量复位并保持在 0。
//      从 enable 变为低电平开始到 counter 复位并保持在 0 的间隔不超过三个时钟周期。此外，rst 信号的优先级应当高于 enable 信号。
module Counter #(
    parameter   MAX_VALUE = 8'd100,
    parameter   MIN_VALUE = 8'd0
)(
    input                   clk,
    input                   rst,
    input                   enable,
    output                  out
);

reg [7:0] counter;
always @(posedge clk) begin
    if (rst)
        counter <= MIN_VALUE;
    else if (!enable)
        counter <= 0;
    else begin
        if (counter >= MAX_VALUE)
            counter <= MIN_VALUE;
        else
            counter <= counter + 8'b1;
    end
end

assign out = (counter == MAX_VALUE);
endmodule