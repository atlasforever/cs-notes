module RegFile (
    input                       clk,          // 时钟信号
    input           [4:0]       ra1,          // 读端口 1 地址
    input           [4:0]       ra2,          // 读端口 2 地址
    input           [4:0]       wa,           // 写端口地址
    input                       we,           // 写使能信号
    input           [31:0]      din,          // 写数据
    output  reg     [31:0]      dout1,        // 读端口 1 数据输出
    output  reg     [31:0]      dout2         // 读端口 2 数据输出
);
// Write your code here

reg [31:0] mem [31:0];

always @(*) begin
    if (ra1 == 0) begin
        dout1 = 0;
    end
    else if (we && ra1 == wa) begin
        dout1 = din;
    end else begin
        dout1 = mem[ra1];
    end

    if (ra2 == 0) begin
        dout2 = 0;
    end
    else if (we && ra2 == wa) begin
        dout2 = din;
    end else begin
        dout2 = mem[ra2];
    end
end

always @(posedge clk) begin
    if (we && wa != 0) begin
        mem[wa] = din;
    end
end
// End of your code
endmodule