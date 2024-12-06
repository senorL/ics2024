#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <limits.h>

int data[] = {0, INT_MAX / 17, INT_MAX, INT_MIN, INT_MIN + 1,
              UINT_MAX / 17, INT_MAX / 17, UINT_MAX};
char data_str[][12] = {
    "0",
    "126322567",
    "2147483647",
    "-2147483648",
    "-2147483647",
    "252645135",
    "126322567",
    "-1"
};

int main() {
    char str[20] = "\0";
    int len = sizeof(data) / sizeof(data[0]);
    for (int i = 0; i < len; i++) {
        sprintf(str, "%d", data[i]);
        assert(strcmp(str, data_str[i]) == 0);
    }
    return 0;
}