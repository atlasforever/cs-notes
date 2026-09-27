`timescale 1ns/1ps

module tb_GrayCounter;

reg clk;
reg rst;
wire out;
wire [3:0] gray;

// 实例化
GrayCounter uut (
    .clk (clk),
    .rst (rst),
    .out (out)
);

// 10ns 时钟周期
initial begin
    clk = 0;
    forever #5 clk = ~clk;
end

initial begin
    // 波形文件
    $dumpfile("wave.vcd");
    $dumpvars(0, tb_GrayCounter);

    // 复位
    rst = 1;
    #12;
    rst = 0;

    // 跑 40 个时钟周期左右
    #400;

    $finish;
end

// 观察 out
always @(posedge clk) begin
    $display("time=%0t  rst=%b  out=%b",
             $time, rst, out);
end

endmodule