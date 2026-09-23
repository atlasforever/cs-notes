// 题目 1：if 语句与锁存器（1 分）
module avoid_if_latch(
    input wire cpu_overheated,
    input wire arrived,
    input wire gas_tank_empty,
    output reg shut_off_computer,
    output reg keep_driving
);

always @(*) begin
    // if/else 配对避免锁存器
    if (cpu_overheated)
        shut_off_computer = 1'b1;
    else
        shut_off_computer = 1'b0;
end

always @(*) begin
    if (~arrived)
        keep_driving = ~gas_tank_empty;
    else
        keep_driving = 1'b0;
end
endmodule