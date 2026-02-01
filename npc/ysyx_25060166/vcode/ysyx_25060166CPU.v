`include "RV32E.vh"

module ysyx_25060166CPU(
    input                   clk,
    input                   rst,
    input   [`ysyx_25060166_WIDTH-1:0]  RAM_RDATA,
	input   [`ysyx_25060166_WIDTH-1:0]	INST,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_RADDR,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_WDATA,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_WADDR,
	output	[`ysyx_25060166_WIDTH-1:0]	PC,
    output                  RAM_WEN
);

	assign RAM_RADDR = `ysyx_25060166_WIDTH'h0;
	assign RAM_WDATA = `ysyx_25060166_WIDTH'h0;
	wire [`ysyx_25060166_WIDTH-1:0] unused_1 = RAM_RDATA | INST;	//to occupy a unused port (temporarily)

    wire    [4:0]   rs1_addr;
    wire    [4:0]   rs2_addr;
    wire    [4:0]   reg_rd;
    wire            reg_wen;
    wire            ALU_work;
	wire			jump_sig;
	wire    [`ysyx_25060166_WIDTH-1:0]  programe_counter;
	wire    [`ysyx_25060166_WIDTH-1:0]  reg_write_data;
	wire	[`ysyx_25060166_WIDTH-1:0]	alu_output;
	
	assign PC = programe_counter;

    ysyx_25060166_IFU IFU(
        .clk        (clk),
        .rst        (rst),
        .jump_sig   (jump_sig),                 //temporary empty
        .jump_addr  (alu_output),                 //temporary empty
        .pc_count   (programe_counter)
    );

	wire [3:0]  alu_op;     //maybe temporary
	wire [`ysyx_25060166_WIDTH-1:0]	ALU_input_0;	//data for ALU
	wire [`ysyx_25060166_WIDTH-1:0] ALU_input_1;
	wire [`ysyx_25060166_WIDTH-1:0] read_data_0;	//read form register
	wire [`ysyx_25060166_WIDTH-1:0] read_data_1;
	wire [`ysyx_25060166_WIDTH-1:0] re_addr;		//jal inst use

    ysyx_25060166_IDU IDU(
        .inst		(INST),
		.pc			(programe_counter),
		.reg_data_0	(read_data_0),
		.reg_data_1	(read_data_1),
        .mem_en     (RAM_WEN),
        .mw_address (RAM_WADDR),
        .rs1_addr   (rs1_addr),
        .rs2_addr   (rs2_addr),
        .reg_wen    (reg_wen),
        .rwrd		(reg_rd),
        .ALU_EN     (ALU_work),
        .ALU_OP     (alu_op),
		.data_0		(ALU_input_0),
        .data_1		(ALU_input_1),
		.jump_sig	(jump_sig),
		.return_addr(re_addr)
        //more IO ports
    );

	assign reg_write_data = jump_sig? re_addr:alu_output;

    ysyx_25060166_REG_ARRAY REG_ARR(
        .clk		(clk),
        .rst		(rst),
        .wen		(reg_wen),
        .raddr1		(rs1_addr),
        .raddr2		(rs2_addr),
        .write_rd	(reg_rd),
        .write_data	(reg_write_data),
        .read_data_1(read_data_0),
		.read_data_2(read_data_1)          //temporary
    );

    //temporary
    ysyx_25060166_ALU ALU(
        .in_1(ALU_input_0),			//temporary
        .in_2(ALU_input_1),			//temporary
        .ALU_EN(ALU_work),
        .op(alu_op),
        .ALU_OUTPUT(alu_output)
    );
    //temporary

endmodule
