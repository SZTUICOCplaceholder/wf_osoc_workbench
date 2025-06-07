#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

static int token_count = 0;
static int max_tokens = 24;  // 最大 token 数量
static int depth = 0;        // 递归深度计数器
static int max_depth = 6;   // 最大递归深度

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static char *buf_start = NULL;
static char *buf_end = buf + sizeof(buf);

static int random_choice(int range) {
    return rand() % range;
}

static void generate_whitespace() {
    int space_count = random_choice(4);
    if (buf_start < buf_end) {
        int bytes_written = snprintf(buf_start, buf_end-buf_start, "%*s", space_count, "");
        if (bytes_written > 0) buf_start += bytes_written;
    }
}

static void generate_number() {
    int number_value = random_choice(INT8_MAX);
    if (buf_start < buf_end) {
        int bytes_written = snprintf(buf_start, buf_end-buf_start, "%d", number_value);
        if (bytes_written > 0) buf_start += bytes_written;
    }
    token_count++; 
    generate_whitespace();
}

static void write_character(char ch) {
    if (buf_start < buf_end) {
        int bytes_written = snprintf(buf_start, buf_end-buf_start, "%c", ch);
        if (bytes_written > 0) buf_start += bytes_written;
    }
}

static char operators[] = {'+', '-', '*', '/'};
static void generate_operator() {
    int operator_index = random_choice(sizeof(operators));
    write_character(operators[operator_index]);
    token_count++;
}

static void generate_expression();

static void generate_simple_expression() {
    // 当 token 数量接近上限时，只生成简单表达式
    if (token_count >= max_tokens - 2) {
        generate_number();
        return;
    }
    switch (random_choice(3)) {
    case 0: generate_number(); break;
    case 1: 
        write_character('('); 
        token_count++;
        generate_expression(); 
        write_character(')'); 
        token_count++;
        break;
    default: 
        generate_expression(); 
        generate_operator(); 
        generate_expression(); 
        break;
    }
}

static void generate_expression() {
    // 检查 token 数量和递归深度限制
    if (token_count >= max_tokens || depth >= max_depth) {
        generate_number();
        return;
    }
    depth++;
    generate_simple_expression();
    depth--;
}

int main(int argc, char *argv[]) {
    int seed_value = time(0);
    srand(seed_value);
    int iterations = 1;
    if (argc > 1) sscanf(argv[1], "%d", &iterations);
    for (int i = 0; i < iterations; i++) {
        // 重置计数器和缓冲区
        token_count = 0;
        depth = 0;
        buf_start = buf;
        *buf = '\0';  // 清空缓冲区
        generate_expression();
        if (buf_start < buf_end) *buf_start = '\0';
        sprintf(code_buf, code_format, buf);
        FILE *output_file = fopen("/tmp/.code.c", "w");
        assert(output_file != NULL);
        fputs(code_buf, output_file);
        fclose(output_file);
        int compile_status = system("gcc /tmp/.code.c -Wall -Werror -o /tmp/.expr");
        if (compile_status != 0) continue;
        output_file = popen("/tmp/.expr", "r");
        assert(output_file != NULL);
        uint32_t calculation_result;
        int fw = fscanf(output_file, "%u", &calculation_result);
        fw = fw; //使用"fw"(无恶意)去接受fscanf的返回值确保函数正常调用，自赋值防止产生警告
        pclose(output_file);
        printf("%u %s\n", calculation_result, buf);
    }
    return 0;
}
