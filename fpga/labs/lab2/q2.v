// 题目 2：case 语句与锁存器（1 分）
module avoid_case_latch (
    input       [15:0]                  scancode,
    output reg                          left,
    output reg                          down,
    output reg                          right,
    output reg                          up
);

always @(*) begin
    // 指定默认输出，避免出现锁存器
    up = 1'b0; down = 1'b0; left = 1'b0; right = 1'b0;
    case (scancode)
        16'he06b: left = 1;
        16'he072: down = 1;
        16'he074: right = 1;
        16'he075: up = 1;
    endcase
end

endmodule
