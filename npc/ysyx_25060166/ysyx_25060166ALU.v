`include "RV32E.vh"

module ysyx_25060166_ALU(
    input   [`ysyx_25060166_WIDTH-1:0] in_1,
    input   [`ysyx_25060166_WIDTH-1:0] in_2,
    input               ALU_EN,
    input   [3:0]       op,
    output  [`ysyx_25060166_WIDTH-1:0] ALU_OUTPUT
);

    wire    [`ysyx_25060166_WIDTH-1:0] result;

    always @(*) begin
        if (op == 4'b000) begin
            result = in_1 + in_2;
        end else if (op == 4'b0001) begin
            result = in_1 - in_2;
        end
        else begin                      //unfinished, but use an 'else' to avoid latch
            result = in_1;
        end 
    end

    assign ALU_OUTPUT = ALU_EN ? result : {`ysyx_25060166_WIDTH{1'b0}};

endmodule
