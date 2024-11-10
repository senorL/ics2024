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

#include <common.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
word_t expr(char *e, bool *success);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif
  // test !
  // FILE *fp = fopen("/home/senor/ics2024/nemu/tools/gen-expr/input", "r");
  // assert(fp != NULL);
  // int i = 0;
  // char e[65536];
  // uint32_t num = 0;
  // while(fgets(e, sizeof(e), fp)) {
  //   printf("%d", i++);
  //    e[strcspn(e, "\n")] = '\0';
  //    char *arg = strtok(e, " ");
  //    sscanf(arg, "%u", &num);
  //    char *expr_str = strtok(NULL, "");
  //    bool success = true;
  //    if (num != expr(expr_str, &success)) {
  //      printf("num: %u\nexpr: %s\nret: %u\n", num, expr_str, expr(expr_str, &success));
  //      assert(0);
  //    }
  //  }
  // fclose(fp);
  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
