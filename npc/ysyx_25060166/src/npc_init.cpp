#include <getopt.h>
#include "../include/vmem.h"
#include "../include/npc.h"

static char* img_file = NULL;

static const uint32_t default_img [5] = {
  0x00000297,  // auipc t0,0
  0x00028823,  // sb  zero,16(t0)
  0x0102c503,  // lbu a0,16(t0)
  0x00100073,  // ebreak (used as nemu_trap)
  0xdeadbeef,  // some data
};

static long load_img(){
	if (img_file == NULL) {
		printf("No image given, use default image\n");
		memcpy(guest_to_host(MEM_BASE), default_img, sizeof(default_img));
		return sizeof(default_img);
	}

	FILE *fp = fopen(img_file, "rb");
	if(fp == NULL){
		printf("can not open the file\n");
		assert(0);
	}

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);

	printf("The image is %s, size = %ld\n", img_file, size);

	fseek(fp, 0, SEEK_SET);
	int ret = fread(guest_to_host(MEM_BASE), size, 1, fp);
	assert(ret == 1);

	fclose(fp);
	return size;
}

static int parse_args(int argc, char* argv[]){
    const struct option table[] = {
    {"batch"    , no_argument      , NULL, 'b'},
    {"log"      , required_argument, NULL, 'l'},
    {"diff"     , required_argument, NULL, 'd'},
    {"port"     , required_argument, NULL, 'p'},
    {"elf"      , required_argument, NULL, 'e'},
    {"help"     , no_argument      , NULL, 'h'},
    {0          , 0                , NULL,  0 },
  };
    int o;
    while ( (o = getopt_long(argc, argv, "-bhl:d:p:e:", table, NULL)) != -1){
        switch(o){
            case 1: img_file = optarg; return 0;
        }
    }
	return 0;
}

void npc_init(int argc, char *argv[]){
	printf("argc: %d\n", argc);
	printf("argv: %s\n", *argv);
	parse_args(argc, argv);
	create_virtual_memory();
	long img_size = load_img();
}