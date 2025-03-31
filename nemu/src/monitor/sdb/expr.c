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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

word_t eval(int p,int q,bool* success);
bool check_parentheses(int p, int q);
static int find_major_op_location(int p, int q);

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM,  
	/*——————————————//finished?——————————————*/
  /* TODO: Add more token types */
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"==", TK_EQ},        // equal
  {"\\+", '+'},         // plus
  {"\\-", '-'},
  //{"==", TK_EQ},        // equal
  {"\\(", '('},
  {"\\)", ')'},
  {"\\*", '*'},
  {"\\/", '/'},
  {"[0-9]+", TK_NUM},
}; 						//has finished       ?

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        int copy_len;
        switch (rules[i].token_type) {
        	case TK_NOTYPE: break;
        	case TK_NUM: 
        	if (nr_token >= 32) {
        		printf("Too many tokens\n");
        		return false;
        	}
        	tokens[nr_token].type = TK_NUM;
        	if (substr_len >=31) copy_len = 31;
        	else copy_len = substr_len;
        	strncpy(tokens[nr_token].str, substr_start, copy_len);
        	tokens[nr_token].str[copy_len] = '\0';
        	nr_token++;
        	break;
            default: 
            	if (nr_token > 31) {
            		printf("Too many tokens\n");
            		return false;
            	}
            	tokens[nr_token].type = rules[i].token_type;
            	tokens[nr_token].str[0] = '\0';
            	nr_token++;
            	break;
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char* e, bool* success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  return eval(0,nr_token-1,success);
}

bool check_parentheses(int p, int q){
	int count = 0;           //while see a '(', count +1, and -1  while see a ')' 
	if (tokens[p].type == '(' && tokens[q].type == ')'){
		for (int i = p; i <= q; i++){
			if (tokens[i].type == '(') count++;
			if (tokens[i].type == ')') count--;
			if (count < 0) return false;
		}
		if (count == 0) return true;
	}
	return false;
}

  /* TODO: Insert codes to evaluate the expression. */

static int level(int token_type){
	switch(token_type){
		case '+': case '-': return 1;
		case '*': case '/': return 2;
		case TK_NUM: return 3;
		default: return 4;
	}
}

static int find_major_op_location(int p,int q){
	int par = 0, op_level = 3,location = -1;
	for (int i = p; i <= q; i++){
		if (tokens[i].type == '(') par++;       //if it's in  parentheses
		else if (tokens[i].type == ')') par--;  //needn't think about it
		if (par != 0) continue;
		if (level(tokens[i].type) <= op_level && par == 0){
			location = i;
			op_level = level(tokens[i].type);
		}
	}
	return location;
}

word_t eval(int p,int q,bool* success){
	*success = true;
	if (p > q) {printf("error location\n"); *success = false; return 0;}
	else if (p == q){
		if (tokens[p].type == TK_NUM) return atoi(tokens[p].str);
		else {printf("Not a expression\n"); *success = false; return 0;}
	}
	else if (check_parentheses(p,q) == true) return eval(p+1,q-1,success);
	else {
		//word_t result;
		int major_op_location = find_major_op_location(p,q);
		word_t val_1 = eval(p,major_op_location-1,success);
		word_t val_2 = eval(major_op_location+1,q,success);
		switch (tokens[major_op_location].type){
			case '+': return val_1+val_2;
			case '-': return val_1-val_2;
			case '*': return val_1*val_2;
			case '/': 
				if (val_2 == 0) {
				*success = false;
				printf("You put a '0' behind '/', please check your expression\n");
				return 0;}
				else return val_1/val_2;
			default: printf("Unknown option\n"); return 0;
		}
	}
  return 0;
}
