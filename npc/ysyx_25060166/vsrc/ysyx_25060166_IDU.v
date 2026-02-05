`include "RV32E.vh"

module ysyx_25060166_IDU(
	input	[`ysyx_25060166_WIDTH-1:0]	inst,
	output	reg				mem_wen,
	output	reg				mem_ren,
	output	reg				uncon_jump,
	output			[4:0]	rs1_addr,
	output			[4:0]	rs2_addr,
	output	reg				reg_wen,
	output			[4:0]	rwrd,
	output	reg	[`ysyx_25060166_WIDTH-1:0]	imm,
	output	reg [5:0]		ALU_OP			//magic number
);

	wire [`ysyx_25060166_WIDTH-1:0]   immI;
	wire [`ysyx_25060166_WIDTH-1:0]   immJ;
	wire [`ysyx_25060166_WIDTH-1:0]   immU;
	wire [`ysyx_25060166_WIDTH-1:0]   immS;
//	wire [`ysyx_25060166_WIDTH-1:0]   immR;
	wire [`ysyx_25060166_WIDTH-1:0]   immB;
    wire [2:0]          funct3;
	wire [6:0]			funct7;
    wire [6:0]          opcode;

    assign funct3 = inst[14:12];
	assign funct7 = inst[31:25];
    assign rs1_addr  = inst[19:15];
    assign rs2_addr  = inst[24:20];
    assign rwrd   = inst[11: 7];
    assign opcode = inst[6:0];

    assign immI = {{20{inst[31]}},{inst[31:20]}};	//I立即数
	assign immJ = {{12{inst[31]}},{inst[19:12]},{inst[20]},{inst[30:21]},1'b0};	//J立即数
	assign immU = {inst[31:12],12'b0};				//U立即数
	assign immS = {{20{inst[31]}},inst[31:25],inst[11:7]};	//S立即数
	assign immB = {{20{inst[31]}},inst[7],inst[30:25],inst[11:8],1'b0};	//B立即数


	import "DPI-C" function void sim_finish();

	always @(*) begin

		mem_wen = 0; mem_ren = 0; reg_wen = 0;			//default assignment to avoide latch
		ALU_OP = `ALU_DEFAULT; imm = 0;	uncon_jump = 0; 

		if(inst == 32'h00100073) begin
			$display("ebreak finish simulation");
			sim_finish();
		end

		//I_TYPE
		if(opcode == 7'b0010011) begin	
			imm = immI;	reg_wen = 1;
			case (funct3)
				3'b000: begin
					ALU_OP = `ADDI; 
				end 
				3'b011: begin
					reg_wen = 1; ALU_OP = `SLTIU;
				end
				3'b001: begin
					if(immI[5] == 0) begin
						ALU_OP = `SLLI; reg_wen = 1;
					end else begin
						$display("invalid slli, NPC choices to skip it");
						reg_wen = 0;
					end
				end
				3'b100: begin
					reg_wen = 1; ALU_OP = `XORI;
				end
				3'b101: begin
					if(immI[5] == 0) begin
						reg_wen = 1; ALU_OP = funct7[5]? `SRAI : `SRLI;
					end else begin
						$display("invalid srli/srai, NPC choices to skip it");
						reg_wen = 0;
					end
				end
				3'b111: begin
					ALU_OP = `ANDI; reg_wen = 1;
				end
				default: begin end
			endcase
		end
		if(opcode == 7'b1100111) begin
			imm = immI; reg_wen = 1;
			case (funct3)
				3'b000: begin
					ALU_OP = `JALR; uncon_jump = 1;
				end 
				default: begin end
			endcase
		end
		if(opcode == 7'b0000011) begin
			imm = immI;
			case (funct3)
				3'b010: begin
					mem_ren = 1; ALU_OP = `LW; reg_wen = 1;
				end
				3'b100: begin
					reg_wen = 1; ALU_OP = `LBU; mem_ren = 1;
				end
				3'b001: begin
					ALU_OP = `LH; reg_wen = 1; mem_ren = 1;
				end
				3'b101: begin
					ALU_OP = `LHU; reg_wen = 1; mem_ren = 1;
				end
				default: begin end
			endcase
		end

		//R_TYPE
		if(opcode == 7'b0110011) begin
			if(funct7 == 7'b0100000 && funct3 == 3'b000) begin
				ALU_OP = `SUB; reg_wen = 1;
			end else if(funct7 == 7'b0100000 && funct3 == 3'b101) begin
				ALU_OP = `SRA; reg_wen = 1;
			end
		end
		if(opcode == 7'b0110011) begin
			if(funct7 == 7'b000_0000 && funct3 == 3'b000) begin
				ALU_OP = `ADD; reg_wen = 1;
			end else if(funct7 == 7'b0000000 && funct3 == 3'b110) begin
				ALU_OP = `OR; reg_wen = 1;
			end else if(funct7 == 7'b0000000 && funct3 == 3'b100) begin
				ALU_OP = `XOR; reg_wen = 1;
			end else if(funct7 == 7'b0000000 && funct3 == 3'b011) begin
				reg_wen = 1; ALU_OP = `SLTU;
			end else if(funct7 == 7'b0000000 && funct3 == 3'b001) begin
				reg_wen = 1; ALU_OP = `SLL;
			end else if(funct7 == 7'b0000000 && funct3 == 3'b101) begin
				reg_wen = 1; ALU_OP = `SRL;
			end else if(funct7 == 7'b0000000 && funct3 == 3'b111) begin
				ALU_OP = `AND; reg_wen = 1;
			end else if(funct7 == 7'b0000000 && funct3 == 3'b010) begin
				reg_wen = 1; ALU_OP = `SLT;
			end
		end

		//U_TYPE
		if(opcode == 7'b0010111) begin
			imm = immU; ALU_OP = `AUIPC; reg_wen = 1;
		end
		if(opcode == 7'b0110111) begin
			imm = immU; reg_wen = 1; ALU_OP = `LUI;
		end

		//J_TYPE
		if(opcode == 7'b1101111) begin
			reg_wen = 1;
			imm = immJ; uncon_jump = 1; ALU_OP = `JAL;
		end

		//S_TYPE
		if(opcode == 7'b0100011) begin
			imm = immS;
			case (funct3)
				3'b010: begin
					ALU_OP = `SW; mem_wen = 1;
				end 
				3'b001: begin
					mem_wen = 1; ALU_OP = `SH;
				end
				3'b000: begin
					mem_wen = 1; ALU_OP = `SB;
				end
				default: begin end
			endcase
		end

		//B_TYPE
		if(opcode == 7'b1100011) begin
			imm = immB;
			case (funct3)
				3'b000: begin
					ALU_OP = `BEQ;
				end
				3'b001: begin
					ALU_OP = `BNE;
				end 
				3'b100: begin
					ALU_OP = `BLT;
				end
				3'b101: begin
					ALU_OP = `BGE;
				end
				3'b110: begin
					ALU_OP = `BLTU;
				end
				3'b111: begin
					ALU_OP = `BGEU;
				end
				default begin end
			endcase
		end

	end

endmodule
