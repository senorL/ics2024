#include <am.h>
#include <klib-macros.h>
#include <klib.h>

#define N 32
char data[N];
char temp[N];

void reset(int n) {
    for (int i = 0; i < n; i++) {
        data[i] = 'a' + i % 26;
        temp[i] = 'a' + i % 26;
    }
}
void test_memcmp() {
    int n = N;
    reset(n);
    assert(memcmp(data, temp, N) == 0);
    for (int i = 0; i < N; i++) {
        data[i] += 1;
        assert(memcmp(data, temp, i + 1) != 0);
    }
    assert(memcmp(data, temp, 0) == 0);
}
void test_strlen() {
    for (int i = 0; i < N; i++) {
        reset(i);
        assert(strlen(data) == i);
    }
}
int main() {
    //test_memcmp();
    test_strlen();
    return 0;
}