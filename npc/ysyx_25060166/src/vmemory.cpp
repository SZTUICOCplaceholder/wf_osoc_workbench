#include "../include/memory.h"
#include <bits/mman-linux.h>
#include <cstdlib>

byte_t* vmem = NULL;  //用全局变量方便操作

void/***/ create_virtual_memory(){

    //检查是否真的需要分配内存
    if(vmem != NULL){
        printf("Virtual memory already exists\n");
        //return vmem;
    }

    // 使用 mmap 分配匿名私有内存映射
    void* virtual_memory = mmap(NULL, MEMSIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    //检测内存分配是否成功
    if (virtual_memory == MAP_FAILED) {
        perror("mmap virtual memory failed\n");
        assert(0);
    }
    
    //将分配到的地址转到全局变量
    vmem = (byte_t*)virtual_memory;

    //输出分配成功信息
    printf("Virtual memory created via mmap, size: %dMB\n", MEM_SIZE);
    printf("Memory address range: [0x%08x - 0x%08x]\n", MEM_BASE, MEM_BASE+MEMSIZE-1);
	//return virtual_memory;
}

void destory_virtual_memory(byte_t* memory){
    if(memory == NULL)  printf("no memory can destory\n");
    else{
        munmap(memory, MEM_SIZE);
        printf("memory has been destory\n");
    }
}

void memory_not_use(){
	destory_virtual_memory(vmem);
}

uint8_t* guest_to_host(paddr_t paddr){return vmem+paddr-MEM_BASE;}

void pmem_write(paddr_t addr, int len, word_t data){
    if(addr-MEM_BASE < MEMSIZE){
        host_write(guest_to_host(addr), len, data);
        return;
	}
    else{
        printf("address = %08x out of bound of memory\n", addr);
        assert(0);
    }
}

word_t pmem_read(paddr_t addr, int len){
    if(addr-MEM_BASE < MEMSIZE){
        word_t ret = host_read(guest_to_host(addr), len);
        return ret;
    }
    else{
        printf("address = %08x out of bound of memory\n", addr);
        assert(0);
    }
}

word_t vmem_read(vaddr_t addr, int len){
    return pmem_read(addr, len);
}

void vmem_write(vaddr_t addr, int len, word_t data){
    return pmem_write(addr, len, data);
}