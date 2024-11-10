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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static int buf_pos = 0;
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static uint32_t choose(uint32_t n) {
  return (uint32_t)(rand() % n);
}

static void gen_num() {
  uint32_t num = choose(100);
  buf_pos += snprintf(buf + buf_pos, sizeof(buf) - buf_pos, "%u", num);
}

static void gen(const char r) {
  buf_pos += snprintf(buf + buf_pos, sizeof(buf) - buf_pos, "%c", r);
}

static void gen_space() {
  int space_num = choose(5);
  for (int i = 0; i <= space_num; i++) {
    gen(' ');
  }
}

static void gen_rand_op() {
  gen_space();
  switch (choose(4)) {
    case 0: gen('+'); break;
    // case 1: gen('-'); break;
    case 2: gen('*'); break;
    default: gen('/'); break;
  }
  gen_space();
}

static void gen_rand_expr(int max_depth) {
  if (max_depth <= 0) {
    gen_num();
    return;
  }
  switch (choose(3)) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(max_depth - 1); gen(')'); break;
    default: gen_rand_expr(max_depth - 1); gen_rand_op(); gen_rand_expr(max_depth - 1); break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    memset(buf, '\0', sizeof(buf));
    buf_pos = 0;
    gen_rand_expr(10);
    buf[buf_pos + 1] = '\0';

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    // [-Wdiv-by-zero] 
    int ret = system("gcc /tmp/.code.c -o /tmp/.expr 2> /tmp/.compile_output");
    if (ret != 0) continue;

    fp = fopen("/tmp/.compile_output", "r");
    assert(fp != NULL);
    char *fp_;
    fscanf(fp, "%s", fp_);
    fclose(fp);

    if (strstr(fp_, "[-Wdiv-by-zero]") != NULL) {
      continue;
    }

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);


    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
