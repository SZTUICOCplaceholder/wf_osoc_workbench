#include <common.h>
#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>

#ifdef CONFIG_MTRACE
void mtrace_read_print(paddr_t addr, int len);
void mtrace_write_print(paddr_t addr, int len, word_t data);

void mtrace_print(const char* operation, paddr_t addr, int len, word_t data){
    if(strcmp(operation, "read")==0) mtrace_read_print(addr, len);
    else if(strcmp(operation, "write")==0) mtrace_write_print(addr, len, data);
    else    printf("你的软件代码能力真的很差\n");
    return;
}

void mtrace_read_print(paddr_t addr, int len){
    if(addr > CONFIG_MTRACE_START && addr < CONFIG_MTRACE_END){
        printf("pread at " FMT_PADDR " len:%d\n", addr, len);
    }
}

void mtrace_write_print(paddr_t addr, int len, word_t data){
    if(addr > CONFIG_MTRACE_START && addr < CONFIG_MTRACE_END){
        printf("pwrite at " FMT_PADDR "len: %d   data: " FMT_WORD"\n", addr, len, data);
    }
}
#else
void mtrace_print(const char* operation, paddr_t addr, int len, word_t data){
    return;
}
#endif