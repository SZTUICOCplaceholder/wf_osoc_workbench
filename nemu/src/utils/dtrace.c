#include <common.h>
#include <utils.h>

void dtrace_read(const char* name, paddr_t addr, int len, word_t data){
	printf(ANSI_FG_YELLOW"Read device: 0x%08x %s\n"
		"len: %d  data: 0x%08x\n"ANSI_NONE, addr, name, len, data);
}

void dtrace_write(const char* name, paddr_t addr, int len, word_t data){
	printf(ANSI_FG_YELLOW"Write device: 0x%08x %s\n"
		"len: %d  data: 0x%08x\n"ANSI_NONE, addr, name, len, data);
}