`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 2026/09/24 15:03:33
// Design Name:
// Module Name: Segment
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


module Segment (
    input        clk,
    input        rst,
    input [31:0] output_data,

    output reg [3:0] seg_data,
    output reg [2:0] seg_an
);

    // clk 100MHz，400Hz 切换一个数码管
    localparam [31:0] COUNT_MAX = 32'd250000 - 1;

    reg [31:0] counter;
    always @(posedge clk) begin
        if (rst) counter <= 0;
        else if (counter >= COUNT_MAX) counter <= 0;
        else counter <= counter + 1;
    end

    // 切换数码管
    reg [2:0] seg_id;
    always @(posedge clk) begin
        if (rst) seg_id <= 0;
        else if (counter == COUNT_MAX) seg_id <= seg_id + 1;
    end

    // 设置数码管值
    always @(*) begin
        seg_data = 0;
        seg_an   = seg_id;  // <- Same for all cases

        // Update seg_data according to seg_id. Hint: Use "case".
        case (seg_id)
            3'd0: seg_data = output_data % 10;
            3'd1: seg_data = (output_data / 10) % 10;
            3'd2: seg_data = (output_data / 100) % 10;
            3'd3: seg_data = (output_data / 1000) % 10;
            3'd4: seg_data = (output_data / 10000) % 10;
            3'd5: seg_data = (output_data / 100000) % 10;
            3'd6: seg_data = (output_data / 1000000) % 10;
            3'd7: seg_data = (output_data / 10000000) % 10;
            default: seg_data = 0;
        endcase
    end

endmodule
