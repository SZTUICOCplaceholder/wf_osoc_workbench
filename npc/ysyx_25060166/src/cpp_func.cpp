#include "../include/npcpp.hpp"
#include "../include/vmem.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstdint>

extern void sim_finish();
extern void halt();

void exec_once(){
	top->INST = vmem_read(top->PC, 4);		//取指
	top->clk = 1; top->eval(); tfp->dump(wave_count++);	//时钟拉高
	//仿真结束逻辑
	if(Verilated::gotFinish()){
		std::cout<<"get finish signal by DPI-C at PC=0x"<<std::hex<<top->PC<<std::endl;
		return;
	}
	top->clk = 0; top->eval(); tfp->dump(wave_count++);	//时钟拉低
	
	//等待添加运行状态更新
	return;
}

extern "C" void execute(uint64_t n){
	for( ; n > 0; n--){
		if(Verilated::gotFinish()) return;
		exec_once();
	}
	//HIT GOOD/BAD TRAP 应该写在这里，并且这里还需要读寄存器
}

extern "C" void halt(){
	Verilated::gotFinish(true);
	std::cout<<"simulation stop, npc go in a halt() function at \nPC = 0x"<<std::hex<<top->PC<<"\nINST: "
	<<std::setw(8)<<std::setfill('0')<<top->INST<<std::dec<<std::endl;
}

extern "C" void sim_finish(){
	Verilated::gotFinish(true);
	return;
}