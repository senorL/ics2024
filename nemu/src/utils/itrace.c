#include <common.h>

#define IRINGBUF_NUM 16

char iringbuf[IRINGBUF_NUM][128];
int buf_idex;

void init_iring() {
    buf_idex = 0;
}

void add_iring(uint32_t inst) {
    sprintf(iringbuf[buf_idex], "%d", inst);
    buf_idex = (buf_idex + 1) % IRINGBUF_NUM;
}

void iring_trace() {
    for (int i = 0; i < IRINGBUF_NUM; i++) {
        printf("%d\n", *iringbuf[i]);
    }
}



