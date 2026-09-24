`timescale 1ns / 1ps

module Top (
    input            clk,
    input      [7:0] sw,
    output reg [7:0] led,
    output     [3:0] d,
    output     [2:0] an
);

    wire led_counter_out;
    reg  rst = 1;
    reg  rst_cnt = 0;

    always @(posedge clk) begin
        if (!rst_cnt) begin
            rst_cnt <= 1;
            rst <= 0;
        end
    end

    Counter #(
        .MAX_VALUE(32'd50000000)
    ) led_counter (
        .clk(clk),
        .out(led_counter_out),
        .rst(rst)
    );

    always @(posedge clk) begin
        if (rst) begin
            led = 8'b0;
        end else if (led_counter_out) begin
            led <= ~led;
        end
    end

    // 数码管显示8位数字
    Segment segment (
        .clk(clk),
        .rst(btn),
        .output_data(32'd19951208),
        .seg_data(d),
        .seg_an(an)
    );

endmodule
