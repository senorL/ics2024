#include <common.h>
#include <cpu/decode.h>

#define IRINGBUF_NUM 16

char iringbuf[IRINGBUF_NUM][128];
int buf_idex;

void init_iring() {
    buf_idex = 0;
}

void add_iring(Decode *s) {
    snprintf(iringbuf[buf_idex], 128, "0x%08x", s->pc);

    buf_idex = (buf_idex + 1) % IRINGBUF_NUM;
}

void iring_trace() {
    for (int i = 0; i < buf_idex; i++) {
        printf("%s\n", iringbuf[i]);
    }
    printf("--> %s\n", iringbuf[buf_idex - 1]);

    for (int i = buf_idex; i < IRINGBUF_NUM; i++) {
        printf("%s\n", iringbuf[i]);
    }
}



