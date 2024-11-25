#include <common.h>
#include <cpu/decode.h>

#define IRINGBUF_NUM 16

char iringbuf[IRINGBUF_NUM][128];
int buf_idex;

void init_iring() {
    buf_idex = 0;
}

void add_iring(Decode *s) {
    char *p = iringbuf[buf_idex];
    p += snprintf(p, 128, FMT_WORD ":", s->pc);
    int ilen = s->snpc - s->pc;
    uint8_t *inst = (uint8_t *)&s->isa.inst;
    for (int i = ilen - 1; i >= 0; i --) {
        p += snprintf(p, 4, " %02x", inst[i]);
    }
    int size = 128 - (p - iringbuf[buf_idex]);
    void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
    disassemble(p, size, MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.inst, ilen);
    buf_idex = (buf_idex + 1) % IRINGBUF_NUM;
}

void iring_trace() {
    if (buf_idex == 0) return;
    for (int i = 0; i < buf_idex - 1; i++) {
        printf("    %s\n", iringbuf[i]);
    }
    printf("\033[1;31m--> %s\033[0m\n", iringbuf[buf_idex - 1]);

    for (int i = buf_idex; i < IRINGBUF_NUM; i++) {
        printf("    %s\n", iringbuf[i]);
    }
}

void print_addr(vaddr_t addr, char sign) {
    if (sign == 'r')
        printf("Read ");
    else 
        printf("Write ");
    printf("Addr: 08%08x    %d\n", addr, addr);
}

