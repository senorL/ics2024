#include <am.h>
#include <klib-macros.h>
#include <klib.h>

#define N 32
uint8_t data[N];

void reset() {
    for (int i = 0; i < N; i++) {
        data[i] = i + 1;
    }
}

void reset_one() {
    for (int i = 0; i < N; i++) {
        data[i] = 1;
    }
}

void check_seq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val + i - l);
  }
}

void check_eq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val);
  }
}

void test_memset() {
  int l, r;
  for (l = 0; l < N; l ++) {
    for (r = l + 1; r <= N; r ++) {
      reset();
      uint8_t val = (l + r) / 2;
      memset(data + l, val, r - l);
      check_seq(0, l, 1);
      check_eq(l, r, val);
      check_seq(r, N, r + 1);
    }
  }
}

void test_memcpy() {
  for (int l = 0; l < N; l ++) {
    for (int r = l + 1; r <= N; r ++) {
      reset();
      uint8_t temp[N];
      memcpy(temp, data, N);
      reset_one();
			memcpy(data + l, temp + l, r - l);
      check_eq(0, l, 1);
      check_seq(l, r, l + 1);
      check_eq(r, N, 1);
    }
  }
}
int main() {
    test_memset();
    reset();
    test_memcpy();
    return 0;
}