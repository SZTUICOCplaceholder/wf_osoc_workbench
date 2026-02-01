`include "RV32E.vh"

module ysyx_25060166_IDU(
	input	[`ysyx_25060166_WIDTH-1:0]	inst,
	input	[`ysyx_25060166_WIDTH-1:0]	pc,
	input	[`ysyx_25060166_WIDTH-1:0]	reg_data_0,
	input	[`ysyx_25060166_WIDTH-1:0]	reg_data_1,
	output					mem_en,
	output		[`ysyx_25060166_WIDTH-1:0]  mw_address,
	output			[4:0]	rs1_addr,
	output			[4:0]	rs2_addr,
	output					reg_wen,
	output			[4:0]	rwrd,
	output					ALU_EN,         //ALU驱动信号
	output	reg [3:0]		ALU_OP,			//magic number
	output	reg [`ysyx_25060166_WIDTH-1:0]	data_0,
	output	reg	[`ysyx_25060166_WIDTH-1:0]	data_1,
	output	reg				jump_sig,
	output	reg	[`ysyx_25060166_WIDTH-1:0]	return_addr
);

	reg memory_en;
	reg	[`ysyx_25060166_WIDTH-1:0]	memory_waddr;

	assign mem_en = memory_en;
	assign mw_address = memory_waddr;

	wire [`ysyx_25060166_WIDTH-1:0]   immI;
	wire [`ysyx_25060166_WIDTH-1:0]   immJ;
	wire [`ysyx_25060166_WIDTH-1:0]   immU;
/*	wire [`ysyx_25060166_WIDTH-1:0]   immR;
	wire [`ysyx_25060166_WIDTH-1:0]   immB;*/
    wire [2:0]          funct3;
	wire [6:0]			funct7;
    wire [4:0]          rd;
    wire [4:0]          rs1;
    wire [4:0]          rs2;
    wire [6:0]          opcode;
    reg                 I_TYPE_ALU;
	reg					J_TYPE_ALU;
	reg					U_TYPE_ALU;
    reg                 R_TYPE_ALU;
    reg                 ALU_WORK;
	reg					register_wen;
	reg	[4:0]			reg_wrd;

    assign funct3 = inst[14:12];
	assign funct7 = inst[31:25];
    assign rs1  = inst[19:15];
    assign rs2  = inst[24:20];
    assign rd   = inst[11: 7];
    assign opcode = inst[6:0];
    assign ALU_EN = ALU_WORK;
	assign reg_wen = register_wen;
	assign rwrd = reg_wrd;

    assign immI = {{20{inst[31]}},{inst[31:20]}};	//I立即数
	assign immJ = {{12{inst[31]}},{inst[19:12]},{inst[20]},{inst[30:21]},1'b0};	//J立即数
	assign immU = {inst[31:12],12'b0};				//U立即数

	assign ALU_WORK = I_TYPE_ALU | R_TYPE_ALU | J_TYPE_ALU | U_TYPE_ALU;        //is that all?

	import "DPI-C" function void sim_finish();

    always @(*) begin

		//default
		I_TYPE_ALU = 0; J_TYPE_ALU = 0; U_TYPE_ALU = 0; R_TYPE_ALU = 0; ALU_OP = 4'b1111;	//ALU
		register_wen = 0; reg_wrd = 0; 		//寄存器
		memory_en = 0; memory_waddr = 0;	//内存
		jump_sig = 0; return_addr = 0;		//don't care, because return address would not used by any unit while jump signal is 0
		data_0 = 0; data_1 = 0;				//数据
		//default

		if(inst == `ysyx_25060166_WIDTH'h00100073) begin		//ebreak
			sim_finish();
			$display("ebreak stop simulation by DPI-C");
		end

		//I_TYPE
        if(opcode == 7'b 0010011) begin
            I_TYPE_ALU = 1;                         //this opcode will invoke ALU
			data_1 = immI;
            case (funct3)   
                3'b000: begin						//addi
                    ALU_OP = 4'b0000;				
					data_0 = reg_data_0;
                    register_wen = 1;
                    reg_wrd = rd;
                end

                default: begin
                    ALU_OP = 4'b1111;				//use default to avoid latch, but not finished
                    reg_wrd = 0;
                end
            endcase
		end else if(opcode == 7'b1100111) begin
			data_1 = immI;
			I_TYPE_ALU = 1;
			case (funct3)							//jalr
				3'b000: begin
					if(!reg_data_0[0]&immI[0]) begin
						data_0[0] = 0;
						data_1[0] = 0;
					end
					data_0 = reg_data_0;
					return_addr = pc+4;
					jump_sig = 1;
					reg_wrd = rd;
					register_wen = 1;
				end
				default: begin
					ALU_OP = 4'b1111;				//use default to avoid latch, but not finished
                    reg_wrd = 0;
				end
			endcase
		end
		//I_TYPE

		//R_TYPE
		else if(opcode == 7'b0110011) begin
			if(funct7 == 7'b0 && funct3 == 3'b0) begin	//add
				data_0 = reg_data_0;
				data_1 = reg_data_1;
				R_TYPE_ALU = 1;
				ALU_OP = 4'b0000;
				reg_wrd = rd;
				register_wen = 1;
			end
		end
		//R_TYPE

		//U_TYPE
		else if(opcode == 7'b0010111) begin				//auipc
			data_0 = pc;
			data_1 = immU;
			U_TYPE_ALU = 1;
			ALU_OP = 4'b0000;
			reg_wrd = rd;
			register_wen = 1;
		end else if(opcode == 7'b0110111) begin		//lui
			register_wen = 1;
			reg_wrd = rd;
			data_0 = 0;
			data_1 = immU;
		end
		//U_TYPE

		//J_TYPE
		else if(opcode == 7'b1101111) begin			//jal
			J_TYPE_ALU = 1;
			ALU_OP = 4'b0000;
			jump_sig = 1;
			register_wen = 1;
			reg_wrd = rd;
			data_0 = pc;
			data_1 = immJ;
			return_addr = pc+4;
		end
		//J_TYPE

		else begin									//缺省情况
/*			$display("unknown command");
			sim_finish();
*/			I_TYPE_ALU = 0;
			J_TYPE_ALU = 0;
			ALU_WORK = 0;
			ALU_OP = 4'b1111;
			jump_sig = 0;
			register_wen = 0;						// 默认不写寄存器
			reg_wrd = 0;							// 默认地址0
			data_0 = 0; data_1 = 0;
		end
		
    end

	assign rs1_addr = rs1;
	assign rs2_addr = rs2;

endmodule
