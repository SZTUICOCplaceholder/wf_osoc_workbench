`include "RV32E.vh"

module ysyx_25060166_IFU(
    input               clk,
    input               rst,
    input               jump_sig,
    input   [`ysyx_25060166_WIDTH-1:0]  jump_addr,
    output  [`ysyx_25060166_WIDTH-1:0]  pc_count
);

    reg [`ysyx_25060166_WIDTH-1:0] pc;

	import "DPI-C" function void halt();

    always @(posedge clk) begin
        if (rst) begin
            pc <= `ysyx_25060166_MEMBASE;
        end else begin
            if (jump_sig) begin
				if(jump_addr == pc) begin
					halt();
					$display("stop simulation by DPI-C");
				end
                pc <= jump_addr;
            end else begin
                pc <= pc + `ysyx_25060166_WIDTH'h0000_0004;
            end
        end
    end

    assign pc_count = pc;

endmodule
