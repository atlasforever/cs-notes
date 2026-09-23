`timescale 1ns/1ps

module wave();

reg clk, zero, a;
reg [2:0] bus;

initial begin
    clk = 1;

    // 产生时钟
    // 每 5ns 翻转一次，所以周期是 10ns
    forever begin
        #5
        clk = ~clk;
    end
end

initial begin
    // 生成波形文件
    $dumpfile("wave.vcd");
    $dumpvars(0, wave);

    zero = 0;
    bus = 1;

    repeat(1) @(posedge clk)
    bus = 2;

    repeat(1) @(posedge clk)
    a = 0;

    repeat(1) @(posedge clk)
    a = 1;
    bus = 3;

    $finish;    
end

endmodule