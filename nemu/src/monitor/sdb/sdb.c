/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include "memory/paddr.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
bool exihibit_watchpoint();
void wp_create(char* monitor_expr);
bool wp_delete(int NO);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_si(char *args){
	char *arg = strtok(args," ");
	int steps;
	if (arg == NULL) steps = 1;
	else steps = atoi(arg);
	cpu_exec(steps);
	printf("Command si %d has finished\n",steps);
	/*if (nemu_state.state == NEMU_QUIT){
	printf("Didn't run such many steps because your program ended before taht");
	}*/   //Why can not do that?
	return 0;
}

static int cmd_q(char *args) {
	nemu_state.state = NEMU_QUIT;
	return -1;
}

static int cmd_info(char *args){
	char *arg = strtok(NULL," ");
	if(arg == NULL){
		printf("'r' for register and 'w' for watchpoints \n");
		return 0;
		}
	else{
	if(strcmp(arg,"r") == 0){
		isa_reg_display();
		printf("all register have been printed.\n");
		}
	else if (strcmp(arg,"w") == 0){
		bool wp_exisit = exihibit_watchpoint();
		if (wp_exisit) printf("All watchpoint have been printed.\n");
		else printf("No watchpoint can print.\n");
		}
	else printf("Unknown command!\n"); 
	}
	return 0;
}

static int cmd_x(char *args){
	char* str_len = strtok(args," ");
	bool success;
	char* addr_str = strtok(NULL," ");
	if (str_len == NULL || addr_str == NULL) {
		printf("Invalid input!\n");
		return -1;
		}
	int len = atoi(str_len); //int addr = strtol(addr_str,NULL,16);
	word_t address = expr(addr_str, &success); int addr = (int)address;
	if (!in_pmem(addr) || !in_pmem(addr + len - 1)) {
		printf("Not a valid address.\n");
		return 0;
		}
	for (int i = 0; i < len; i++){
		word_t data = paddr_read(addr + 4*i,4);
		printf("Address: 0x%08x, Data: 0x%08x\n", addr + 4*i, data);
		}
	return 0;
}

static int cmd_p(char* args){
	bool success;
	word_t result = expr(args,&success);
	if (!success) printf("Invalid expression\n");
	else printf("Result: %u\n",result);
	return 0;
}

static int cmd_w(char* args){
	//printf("receive a expr: %s\n",args);
	if (!args) printf("Where is your f**king expression?\n");
	else wp_create(args);
	bool wp_exisit = exihibit_watchpoint();
	if (wp_exisit) printf("Gift you a watchpoint exhibit.\n");
	else printf("There isn't any watchpoint right now.\n");
	return 0;
}

static int cmd_d(char* args){
	char* endptr;
	int NO = strtol(args, &endptr, 10);
	bool dlt_suc = wp_delete(NO);
	if(dlt_suc) printf("Watchpoint with NO.%d has deleted.\n",NO);
	bool wp_exisit = exihibit_watchpoint();
	if (wp_exisit) printf("Gift you a watchpoint exhibit.\n");
	else printf("There isn't any watchpoint right now.\n");
	return 0;
}

static int ext(char* args){
    FILE *fp = fopen("tools/gen-expr/build/input", "r");
    if (fp == NULL) {
        perror("test_expr error: cannot open input file");
        return -1;
    }
    char *e = NULL;
    size_t len = 0;
    ssize_t read;
    bool success = false;
    int all = 0;        
    int pass = 0;      
    while (true) {
        uint32_t correct_res;
        if(fscanf(fp, "%u", &correct_res) != 1) {
            if (feof(fp)) break;
            fprintf(stderr, "Error reading expected result\n");
            break;
        }
        int c;
        while ((c = fgetc(fp)) != EOF && isspace(c) && c != '\n') {}
        if (c == EOF) break;
        ungetc(c, fp);
        read = getline(&e, &len, fp);
        if (read == -1) break;
        if (read > 0 && e[read-1] == '\n') {
            e[read-1] = '\0';
            read--;
        }
        all++;
        word_t res = expr(e, &success);
        if (!success) {
            fprintf(stderr, "Expression evaluation failed: %s\n", e);
            continue;
        }
        if (res == correct_res) pass++; 
        else {
            fprintf(stderr, "Mismatch:\n");
            fprintf(stderr, "  Expression: %s\n", e);
            fprintf(stderr, "  Expected: %u, Got: %u\n", correct_res, res);
        }
    }
    fclose(fp);
    if (e) free(e);
    printf("pass:%d all:%d\n",pass,all);
    printf("Pass rate: %.2f%%\n", (all > 0) ? (100.0 * pass / all) : 0.0);
    if (pass == all) Log("All expr tests passed");
    else Log("Some expr tests failed");
    return 0;
}

/*static int cmd_lab(){
	printf("diff_port: %d\n", difftest_port);
	return 0;
}*/

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Continue the execution by n steps", cmd_si },
  { "info", "Exhibit registers or watchpoints", cmd_info },
  { "x", "Scan memory", cmd_x },
  { "p", "Calculate the expression", cmd_p },
  { "w", "Add a new watchpoint", cmd_w },
  { "d", "delete an unwanted watchpoint by NO.", cmd_d },
  { "ext", "examine the expr.", ext},
  //{ "lab", "A temporary instruction used during development and testing.", cmd_lab }

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) {
        return;}
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
