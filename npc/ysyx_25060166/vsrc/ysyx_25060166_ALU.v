`include "RV32E.vh"

module ysyx_25060166_ALU(
    input	[`ysyx_25060166_WIDTH-1:0]	reg_data_0,
    input	[`ysyx_25060166_WIDTH-1:0]	reg_data_1,
	input	[`ysyx_25060166_WIDTH-1:0]	pc,
	input	[`ysyx_25060166_WIDTH-1:0]	imm,
	input	[`ysyx_25060166_WIDTH-1:0]	mem_rdata,
    input   [5:0]       op,
	output	reg			con_jump,
	output	reg			half_write,
	output	reg			byte_write,
	output	reg	[15:0]	mem_half_data,
	output	reg	[7:0]	mem_byte,
	output	reg	[`ysyx_25060166_WIDTH-1:0]	mem_addr,
    output  reg [`ysyx_25060166_WIDTH-1:0]  reg_write_data,
	output	reg [`ysyx_25060166_WIDTH-1:0]	mem_write_data
);

	wire [31:0] word_align = 32'hFFFF_FFFC;
	wire [31:0] half_align = 32'hFFFF_FFFE;

	always @(*) begin

		con_jump = 0; mem_addr = 0; reg_write_data = 0;	mem_write_data = 0;
		mem_half_data = 0; half_write = 0; byte_write = 0; mem_byte = 0;

		case (op)
			`ADD: reg_write_data = reg_data_0 + reg_data_1;
			`ADDI: reg_write_data = reg_data_0 + imm;
			`AUIPC: reg_write_data = imm + pc;
			`LUI: reg_write_data = imm;
			`JAL: begin
				reg_write_data = pc + 4;
				mem_addr = pc + imm;
			end
			`JALR: begin
				mem_addr = reg_data_0 + imm;
				reg_write_data = pc + 4;
			end
			`SW: begin
				mem_addr = (reg_data_0 + imm) & word_align;
				mem_write_data = reg_data_1;
			end
			`SH: begin
				mem_addr = (reg_data_0 + imm) & half_align;
				mem_half_data = reg_data_1[15:0];
				half_write = 1;
			end
			`SB: begin
				mem_addr = reg_data_0 + imm;
				mem_byte = reg_data_1[7:0];
				byte_write = 1;
			end
			`LW: begin
				reg_write_data = mem_rdata;
				mem_addr = (reg_data_0 + imm) & word_align;
			end
			`LH: begin
				mem_addr = (reg_data_0 + imm) & half_align;
				reg_write_data = {{16{mem_rdata[15]}},mem_rdata[15:0]};
			end
			`LHU: begin
				mem_addr = (reg_data_0 + imm) & half_align;
				reg_write_data = {16'h0000,mem_rdata[15:0]};
			end
			`LBU: begin
				mem_addr = reg_data_0 + imm;
				reg_write_data = {24'h000000,mem_rdata[7:0]};
			end
			`SUB: reg_write_data = reg_data_0 - reg_data_1;
			`SLT: reg_write_data = $signed(reg_data_0)<$signed(reg_data_1)? 1:0;
			`SLTU: reg_write_data = reg_data_0<reg_data_1? 1:0;
			`SLTIU: reg_write_data = reg_data_0<imm? 1:0;
			`BLT: begin
				if($signed(reg_data_0) < $signed(reg_data_1)) begin
					con_jump = 1; mem_addr = pc + imm;
				end
			end
			`BLTU: begin
				if(reg_data_0 < reg_data_1) begin
					con_jump = 1; mem_addr = pc + imm;
				end
			end
			`BEQ: begin
				if(reg_data_0 == reg_data_1) begin
					con_jump = 1; mem_addr = pc + imm;
				end
			end
			`BNE: begin
				if(reg_data_0 != reg_data_1) begin
					con_jump = 1; mem_addr = pc+imm;
				end
			end
			`BGE: begin
				if($signed(reg_data_0) >= $signed(reg_data_1)) begin
					con_jump = 1; mem_addr = pc + imm;
				end
			end
			`BGEU: begin
				if(reg_data_0 >= reg_data_1) begin
					con_jump = 1; mem_addr = pc + imm;
				end
			end
			`OR: reg_write_data = reg_data_0 | reg_data_1;
			`XOR: reg_write_data = reg_data_0 ^ reg_data_1;
			`XORI: reg_write_data = reg_data_0 ^ imm;
			`AND: reg_write_data = reg_data_0 & reg_data_1;
			`ANDI: reg_write_data = reg_data_0 & imm;
			`SLLI: reg_write_data = reg_data_0 << imm[4:0];
			`SLL: reg_write_data = reg_data_0 << reg_data_1[4:0];
			`SRLI: reg_write_data = reg_data_0 >> imm[4:0];
			`SRL: reg_write_data = reg_data_0 >> reg_data_1[4:0];
			`SRA: reg_write_data = $signed(reg_data_0) >>> reg_data_1[4:0];
			`SRAI: reg_write_data = $signed(reg_data_0) >>> imm[4:0];

			default: begin end
		endcase
	end

endmodule
