#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>

#define MEM_SIZE 128
#define MEMSIZE MEM_SIZE*1024*1024
#define MEM_BASE 0x80000000

typedef uint32_t paddr_t;
typedef uint32_t word_t;
typedef word_t vaddr_t;
typedef uint8_t byte_t;

uint8_t* guest_to_host(paddr_t paddr);

static inline word_t host_read(void* addr, int len){
    switch(len){
        case 1: return *(uint8_t  *)addr;
        case 2: return *(uint16_t *)addr;
        case 4: return *(uint32_t *)addr;
        default: printf("ATENTION: HERE HAPPENED AN UNDEFINED MEMORY READ!\n"); return 0;
    }
}

static inline void host_write(void* addr, int len, word_t data){
    switch(len){
        case 1: *(uint8_t  *)addr = data; return;
        case 2: *(uint16_t *)addr = data; return;
        case 4: *(uint32_t *)addr = data; return;
        default: printf("ATENTION: HAPPENED AN UNDEFINED MEMORY WRITE\n"); assert(0);
    }
}

