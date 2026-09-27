module GrayCounter(
input                               clk,
input                               rst,
output                              out
);
    reg [3:0] count;
    reg _out;

    always @(posedge clk) begin
        if (rst) begin
            count <= 4'b0000;
            _out <= 0;
        end
        else begin
            _out = 0;

            case (count)
            4'b0000:
                count <= 4'b0001;
            4'b0001:
                count <= 4'b0011;
            4'b0011:
                count <= 4'b0010;
            4'b0010:
                count <= 4'b0110;
            4'b0110:
                count <= 4'b0111;
            4'b0111:
                count <= 4'b0101;
            4'b0101:
                count <= 4'b0100;
            4'b0100:
                count <= 4'b1100;
            4'b1100:
                count <= 4'b1101;
            4'b1101:
                count <= 4'b1111;
            4'b1111:
                count <= 4'b1110;
            4'b1110:
                count <= 4'b1010;
            4'b1010:
                count <= 4'b1011;
            4'b1011:
                count <= 4'b1001;
            4'b1001:
                count <= 4'b1000;
            4'b1000: begin
                count <=  4'b0000;
                _out <= 1;
            end
        endcase
        end
    end

    assign out = _out;
endmodule