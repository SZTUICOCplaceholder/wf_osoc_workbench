`include "RV32E.vh"

module ysyx_25060166CPU(
    input                   clk,
    input                   rst,
	input   [`ysyx_25060166_WIDTH-1:0]	INST,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_WDATA,
    output  [`ysyx_25060166_WIDTH-1:0]  RAM_ADDR,
	output	[`ysyx_25060166_WIDTH-1:0]	PC,
    output                  RAM_WEN,
	output					RAM_REN
);

    wire    [4:0]   rs1_addr;
    wire    [4:0]   rs2_addr;
    wire    [4:0]   reg_rd;
    wire            reg_wen;
	wire			jump_sig;
	wire			unconditional_jump;
	wire			conditional_jump;
	wire			mem_write_half;
	wire			mem_write_byte;
	wire	[15:0]	mem_half_data;
	wire	[7:0]	mem_byte_data;
	wire	[`ysyx_25060166_WIDTH-1:0]	imm_num;
	wire    [`ysyx_25060166_WIDTH-1:0]  programe_counter;
	wire    [`ysyx_25060166_WIDTH-1:0]  reg_write_data;
	wire	[`ysyx_25060166_WIDTH-1:0]	mem_address;
	wire	[`ysyx_25060166_WIDTH-1:0]	RAM_RDATA;
	
	assign PC = programe_counter;
	assign RAM_ADDR = mem_address;
	assign jump_sig = unconditional_jump | conditional_jump ;

    ysyx_25060166_IFU IFU(
        .clk        (clk),
        .rst        (rst),
        .jump_sig   (jump_sig),
        .jump_addr  (mem_address),
        .pc_count   (programe_counter)
    );

	wire [5:0]  alu_op;
	wire [`ysyx_25060166_WIDTH-1:0] read_data_0;	//read form register
	wire [`ysyx_25060166_WIDTH-1:0] read_data_1;

    ysyx_25060166_IDU IDU(
        .inst		(INST),
        .mem_wen	(RAM_WEN),
		.mem_ren	(RAM_REN),
		.uncon_jump	(unconditional_jump),
        .rs1_addr	(rs1_addr),
        .rs2_addr   (rs2_addr),
        .reg_wen    (reg_wen),
        .rwrd		(reg_rd),
		.imm		(imm_num),
        .ALU_OP     (alu_op)
        //more IO ports
    );

    ysyx_25060166_REG_ARRAY REG_ARR(
        .clk		(clk),
        .rst		(rst),
        .wen		(reg_wen),
        .raddr1		(rs1_addr),
        .raddr2		(rs2_addr),
        .write_rd	(reg_rd),
        .write_data	(reg_write_data),
        .read_data_1(read_data_0),
		.read_data_2(read_data_1)
    );

    //temporary
    ysyx_25060166_ALU ALU(
        .reg_data_0		(read_data_0),
        .reg_data_1		(read_data_1),	
		.pc				(programe_counter),
		.imm			(imm_num),
		.mem_rdata		(RAM_RDATA),
        .op				(alu_op),
		.con_jump		(conditional_jump),
		.half_write		(mem_write_half),
		.byte_write		(mem_write_byte),
		.mem_half_data	(mem_half_data),
		.mem_byte		(mem_byte_data),
		.mem_addr		(mem_address),
        .reg_write_data	(reg_write_data),
		.mem_write_data	(RAM_WDATA)
    );
    //temporary

	ysyx_25060166_MEM MEM_IF(
//		.clk		(clk),
		.write_en	(RAM_WEN),
		.read_en	(RAM_REN),
		.half_write	(mem_write_half),
		.byte_write	(mem_write_byte),
		.address	(mem_address),
		.write_data	(RAM_WDATA),
		.half_data	(mem_half_data),
		.byte_data	(mem_byte_data),
		.read_data	(RAM_RDATA)
	);

endmodule
