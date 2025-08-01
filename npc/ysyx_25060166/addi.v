`include "RV32E.vh"

module addi(
    input       clk,
    input       rst,
    input   [`ysyx_25060166_WIDTH-1:0]  RAM_RDATA,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_RADDR,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_WDATA,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_WADDR,
    output                  RAM_WEN
);

    wire    [4:0]   rs1_addr;
    //wire    [4:0]   rs2_addr;
    wire    [4:0]   reg_rd;
    //wire            reg_wen;
    //wire            ALU_work;
    wire    [`ysyx_25060166_WIDTH-1:0]  read_data_1;
    reg     [`ysyx_25060166_WIDTH-1:0]  pc;
    wire    [`ysyx_25060166_WIDTH-1:0]  reg_write_data;
    wire    [`ysyx_25060166_WIDTH-1:0]  immediate;

    assign RAM_RADDR = pc;              //temporary?

    always @(posedge clk) begin
        if (rst) begin
            pc <= 32'h8000_0000;
        end else begin
            pc <= pc + 32'd4;
        end
    end

    ysyx_25060166_REG_ARRAY REG_ARR(
        .clk        (clk),
        .rst        (rst),
        .wen        (1'b1),
        .raddr1     (rs1_addr),
        .raddr2     (5'd0),
        .write_rd   (reg_rd),
        .write_data (reg_write_data),
        .read_data_1(read_data_1),
        .read_data_2()          //temporary
    );

    assign immediate = {{20{RAM_RDATA[31]}},RAM_RDATA[31:20]};
    assign rs1_addr = RAM_RDATA[19:15];
    assign reg_rd = RAM_RDATA[11:7];
    assign reg_write_data = immediate + read_data_1;

    assign RAM_WEN = 1'b0;
    assign RAM_WDATA = 32'd0;
    assign RAM_WADDR = 32'd0;

endmodule
