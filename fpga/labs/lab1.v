// 阅读以下 Verilog 代码，写出当 a = 8’b0011_0011, b = 8’b1111_0000 时各输出信号的值。 
module test(
    input  [7:0]        a, b,
    output [7:0]        c, d, e, f, g, h, i, j, k, l
  );
  assign c = a & b;   // 8'b00110000
  assign d = a || b;  // 1'b1
  assign e = a ^ b;   // 8'b11000011
  assign f = ~a;      // 8'b11001100
  assign g = {a[2:0], b[3:0], {1'b1}};  // 8'b01100001
  assign h = b >>> 3; // 8’b0001_1110，无符号补0
  assign i = &b;      // 1'b0
  assign j = (a > b) ? a : b; // 8’b1111_0000
  assign k = a - b; // 67（-189截断）
  assign l = !a;  // 1'b0
endmodule


// Verilog 支持对于数组的声明。请自行查阅有关资料，编写 Verilog 代码，声明一个由 16 个位宽为 32 的 reg 型变量组成的数组。
module array(

  );
  reg [31:0] data [0:15];
endmodule

