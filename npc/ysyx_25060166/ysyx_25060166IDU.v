`include "RV32E.vh"

module ysyx_25060166_IDU(
    input   [`ysyx_25060166_WIDTH-1:0]  inst,
    output                  mem_en,
    output  [`ysyx_25060166_WIDTH-1:0]  mw_address,
    output  [4:0]           rs1_addr,
    output  [4:0]           rs2_addr,
    output                  reg_wen,
    output  [4:0]           write_rd,

    //二期工程端口
    output              ALU_EN,         //ALU驱动信号
    output    [3:0]     ALU_OP,
    output  [`ysyx_25060166_WIDTH-1:0]  imm
);

    wire [`ysyx_25060166_WIDTH-1:0]   immI;
  /*wire [`ysyx_25060166_WIDTH-1:0]   immU;
    wire [`ysyx_25060166_WIDTH-1:0]   immJ;
    wire [`ysyx_25060166_WIDTH-1:0]   immR;
    wire [`ysyx_25060166_WIDTH-1:0]   immB;*/
    wire [2:0]          funct3;
    wire [4:0]          rd;
    wire [4:0]          rs1;
    wire [4:0]          rs2;
    wire [6:0]          opcode;
    reg                 I_TYPE_ALU;
    reg                 R_TYPE_ALU;
    reg                 ALU_WORK;

    assign funct3 = inst[14:12];
    assign rs1  = inst[19:15];
    assign rs2  = inst[24:20];
    assign rd   = inst[11: 7];
    assign opcode = inst[6:0];
    assign ALU_EN = ALU_WORK;
    assign imm = immI;

    //imm立即数
    assign immI = {{20{inst[31]}},{inst[31:20]}};

    assign ALU_WORK = I_TYPE_ALU | R_TYPE_ALU;        //is that all?

    always @(*) begin
        if(opcode == 7'b 0010011) begin
            I_TYPE_ALU = 1;                         //this opcode will invoke ALU
            case (funct3)   
                3'b000: begin
                    ALU_OP = 4'b0000;
                    reg_wen = 1;
                    write_rd = rd;
                end

                default: begin
                    ALU_OP = 4'b1111;          //use default to avoid latch, but not finished
                    write_rd = 0;
                end
            endcase
        end else begin
            I_TYPE_ALU = 0;
            ALU_OP = 4'b0000;  // 默认值
            reg_wen = 0;       // 默认不写寄存器
            write_rd = 0;      // 默认地址0
        end
    end

endmodule
