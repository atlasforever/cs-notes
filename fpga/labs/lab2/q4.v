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

    // 等待一个clk
    repeat(1) @(posedge clk);
    bus = 2;

    repeat(1) @(posedge clk);
    a = 0;

    repeat(1) @(posedge clk);
    a = 1;
    bus = 3;

    repeat(1) @(posedge clk);
    a = 0;

    repeat(2) @(posedge clk);
    a = 1;
    bus = 4;

    repeat(3) @(posedge clk);
    a = 0;

    repeat(1) @(posedge clk);
    a = 1;
    bus = 5;

    repeat(2) @(posedge clk);
    a = 0;

    repeat(1) @(posedge clk);
    zero = 1;

    repeat(2) @(posedge clk);
    bus = 6;

    repeat(2) @(posedge clk);

    $finish;    
end

endmodule