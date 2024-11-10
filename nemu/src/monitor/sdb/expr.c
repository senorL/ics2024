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
#include <string.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_LP, TK_RP, TK_NUM,

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
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"\\-", '-'},
  {"\\*", '*'},
  {"/", '/'},
  {"\\(", TK_LP},
  {"\\)", TK_RP},
  {"[0-9]+", TK_NUM},


};

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

        switch (rules[i].token_type) {

          case TK_NOTYPE:
            break;
          case '+': case '-': case '*': case '/': case TK_LP: case TK_RP:
            tokens[nr_token++].type = rules[i].token_type;
            break;
          case TK_NUM:
            tokens[nr_token].type = rules[i].token_type;
            if (substr_len >= 32) {
              strncpy(tokens[nr_token].str, substr_start, 31);
              tokens[nr_token].str[31] = '\0';
            }
            else {
              strncpy(tokens[nr_token].str, substr_start, substr_len);
              tokens[nr_token].str[substr_len] = '\0';
              nr_token++;
            }
            break;

          default: 
            break;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n\033[31m%*.s^\033[0m\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

int op_priority(int r) {
	switch(r) {
		case '+': case '-':
			return 1;
		case '*': case '/':
			return 2;
		default:
			return 10;
}
}

int primary_operator(int p, int q) {
	int op = 0;
	int tmp_compare = 10;
  int num = 0;
	for (int i = p; i <= q; i++) {
		if (tokens[i].type == TK_LP) {
      num++;
			while(num != 0) {
				i++;
        if (tokens[i].type == TK_LP)
          num++;
        else if (tokens[i].type == TK_RP)
          num--;
			}}
		if (tokens[i].type != '+' && tokens[i].type != '-' && tokens[i].type != '*' && tokens[i].type != '/')
			continue;
		
		if (tmp_compare >= op_priority(tokens[i].type)) {
				op = i;
				tmp_compare = op_priority(tokens[i].type);
		}
		}
		return op;
}


int check_parentheses(int p, int q) {
    if (tokens[p].type != TK_LP || tokens[q].type != TK_RP) {
        return 0;
    }
    int num = 0;
    int flag = 0;
    for (int i = p; i <= q; i++) {
        if (tokens[i].type == TK_LP) num++;
        if (tokens[i].type == TK_RP) num--;
        if (i != q && num == 0) {
          flag = 1;
        }
    }
    if (num != 0) {
      assert(0);
    }
    return flag == 0 ? 1 : 2;
}


word_t eval(int p, int q) {
  if (p > q) {
    printf("\np > q! p: %d q: %d\n", p, q);
    assert(0);
  }
  else if (p == q) {
    word_t num;
    sscanf(tokens[p].str, "%u", &num);
    return num;
  }

  else if (check_parentheses(p, q) == 1) {
    return eval(p + 1, q - 1);
  }

  else {
    int op = primary_operator(p, q);
    word_t val1 = eval(p, op - 1);
    word_t val2 = eval(op + 1, q);

    switch (tokens[op].type) {
      case '+': 
        //printf ("%u\n", val1 + val2); 
        return val1 + val2;
      case '-': 
        //printf ("%u\n", val1 - val2); 
        return val1 - val2;
      case '*': 
        //printf ("%u\n", val1 * val2); 
        return val1 * val2;
      case '/': 
        //printf ("%u\n", val1 / val2); 
        return val1 / val2;
      default: assert(0);
    }
  }

}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  int p = 0;
  int q = nr_token - 1; 
  word_t res = 0;
  res = eval(p, q);

  return res;
}
