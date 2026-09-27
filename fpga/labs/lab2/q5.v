// 题目 5：1的个数pro（2 分）
// 设计一个Verilog模块，统计32位输入数据 a 中从第 b 位到第 c 位（包含两端）这个区间内1的个数。其中 b 和 c 都是5位无符号数，且保证 b < c。
module CountOnes(
    input       [31:0]         a,
    input       [4:0]          b,
    input       [4:0]          c,
    output reg  [5:0]         out
);

integer i;

always @(*) begin
    out = 0;

    for (i = 0; i <= 31; i++) begin
        if (i >= b && i <= c) begin
            if (a[i])
                out = out + 1; 
        end
    end
end

endmodule