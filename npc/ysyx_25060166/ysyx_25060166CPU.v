`include "RV32E.vh"

module ysyx_25060166CPU(
    input                   clk,
    input                   rst,
    input   [`ysyx_25060166_WIDTH-1:0]  RAM_RDATA,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_RADDR,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_WDATA,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_WADDR,
    output                  RAM_WEN
);

    wire    [4:0]   rs1_addr;
    wire    [4:0]   rs2_addr;
    wire    [4:0]   reg_rd;
    wire            reg_wen;
    wire            ALU_work;
    wire    [`ysyx_25060166_WIDTH-1:0]  pc;
    wire    [`ysyx_25060166_WIDTH-1:0]  reg_write_data;
    wire    [`ysyx_25060166_WIDTH-1:0]  immediate;

    assign RAM_RADDR = pc;              //temporary?

    ysyx_25060166_IFU IFU(
        .clk        (clk),
        .rst        (rst),
        .jump_sig   (1'b0),                 //temporary empty
        .jump_addr  (32'd0),                 //temporary empty
        .pc_count   (pc)
    );

    ysyx_25060166_IDU IDU(
        .inst       (RAM_RDATA),        //maybe temporary
        .mem_en     (RAM_WEN),
        .mw_address (RAM_WADDR),
        .rs1_addr   (rs1_addr),
        .rs2_addr   (rs2_addr),
        .reg_wen    (reg_wen),
        .write_rd   (reg_rd),
        .ALU_EN     (ALU_work),
        .ALU_OP     (alu_op),
        .imm        (immediate)
        //more IO ports
    );

    wire [3:0]  alu_op;     //maybe temporary

    ysyx_25060166_REG_ARRAY REG_ARR(
        .clk        (clk),
        .rst        (rst),
        .wen        (reg_wen),
        .raddr1     (rs1_addr),
        .raddr2     (rs2_addr),
        .write_rd   (reg_rd),
        .write_data (reg_write_data),
        .read_data_1(read_data_1),
        .read_data_2()          //temporary
    );

    wire [`ysyx_25060166_WIDTH-1:0] read_data_1;    //temporary signal

    //temporary
    ysyx_25060166_ALU ALU(
        .in_1(read_data_1),
        .in_2(immediate),       //temporary
        .ALU_EN(ALU_work),
        .op(alu_op),
        .ALU_OUTPUT(reg_write_data)
    );
    //temporary

endmodule
