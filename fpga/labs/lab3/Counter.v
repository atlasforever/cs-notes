`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 2026/09/24 10:39:19
// Design Name:
// Module Name: Counter
// Project Name:
// Target Devices:
// Tool Versions:
// Description:
//
// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////

module Counter #(
    parameter MAX_VALUE = 8'd100
) (
    input  clk,
    input  rst,
    output out
);

    reg [31:0] counter;
    always @(posedge clk) begin
        if (rst) counter <= 0;
        else begin
            if (counter >= MAX_VALUE) counter <= 0;
            else counter <= counter + 8'b1;
        end
    end

    assign out = (counter == MAX_VALUE);
endmodule
