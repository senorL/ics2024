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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  uint32_t old_value;
  uint32_t new_value;
  char *wp_var;

  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp() {
  if (free_ == NULL) {
    assert(0);
  }

  WP *wp = free_;
  free_ = free_->next;

  if (head == NULL) {
    head = wp;
  }
  else {
    WP *last = head;
    while(last->next != NULL) {
      last = last->next;
    }
    last->next = wp;
  }

  wp->next = NULL;
  return wp;
}

void free_wp(int no) {
  if (head == NULL) {
    assert(0);
  }

  WP *p = NULL;
  WP *current = head;

  while(current != NULL && current->NO != no) {
    p = current;
    current = current->next;
  }

  if (current == NULL) {
    puts("not found!");
    assert(0);
  }

  if (p == NULL) {
    head = current->next;
  } else {
    p->next = current->next;
  }
  
  //释放内存
  if (current->wp_var != NULL) {
    free(current->wp_var);
    current->wp_var = NULL;
  }

  if (free_ == NULL) {
    free_ = current;
  }
  else {
    WP *last = free_;
    while (last->next != NULL) {
      last = last->next;
    }
    last->next = current;
  }
  current->next = NULL;

}

void new_watchpoint(char *e) {
  WP *wp = new_wp();
  wp->wp_var = strdup(e);
  bool success = true;
  wp->old_value = expr(e, &success);
  wp->new_value = wp->old_value;
  printf("New watchpoint %d: %s\n", wp->NO, wp->wp_var);
}

void print_watchpoint() {
  WP *p = head;
  if (p == NULL){
    puts("No watchpoint!");
    return;
  } 
  printf("Num What\n");

  while(p != NULL) {
    printf("%d %s\n", p->NO, p->wp_var);
    p = p->next;
  }

}

void delete_watchpoint(int no) {
  free_wp(no);
}


void check_watchpoint() {
  WP *wp = head;
  if (wp == NULL) {
    return;
  }
  while (wp != NULL) {
    bool success = true;
    uint32_t tmp_value = expr(wp->wp_var, &success);
    if (wp->old_value != tmp_value) {
      wp->new_value = tmp_value;
      printf("Watchpoint %d: %s\n", wp->NO, wp->wp_var);
      printf("Old value: %u\n", wp->old_value);
      printf("New value: %u\n", wp->new_value);
      wp->old_value = wp->new_value;
      nemu_state.state = NEMU_STOP;
      return;
    }
    wp = wp->next;
  }
  return;
}