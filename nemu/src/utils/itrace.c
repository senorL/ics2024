#include <common.h>
#include <cpu/decode.h>
#include <elf.h>

#define IRINGBUF_NUM 16
#define MAX_INST 200

Elf32_Sym *read_symbol();
char *read_string();

Elf32_Sym *symbol_table = NULL;
char *string_table = NULL;

char iringbuf[IRINGBUF_NUM][128];
int buf_idex;

char ftrace[MAX_INST][128];
int ftrace_idex = 0;

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
        printf("\t%s\n", iringbuf[i]);
    }
    printf("\033[1;31m--> %s\033[0m\n", iringbuf[buf_idex - 1]);

    for (int i = buf_idex; i < IRINGBUF_NUM; i++) {
        printf("\t%s\n", iringbuf[i]);
    }
}

void print_addr(vaddr_t addr, char sign) {
    if (sign == 'r')
        printf("Read ");
    else 
        printf("Write ");
    printf("Addr: 08%08x    %d\n", addr, addr);
}

void init_ftrace() {
    symbol_table = read_symbol();
    string_table = read_string();
}

void ftrace_call(vaddr_t pc) {
    vaddr_t call_pc = pc;
    char *p = ftrace[ftrace_idex];
    p += snprintf(p, 128, FMT_WORD ":", pc);

    p = strcat(p, "call");

    for (int i = 0; i < symbol_table->st_size; i++) {
        if (symbol_table[i].st_info == STT_FUNC && (call_pc >= symbol_table->st_value) && (call_pc < (symbol_table->st_value + symbol_table->st_size))) {
           p += snprintf(p, 128 - sizeof(p), "%d", string_table[symbol_table[i].st_name]); 
        }
    }

    ftrace_idex++;

}

void ftrace_ret(vaddr_t pc) {
    vaddr_t ret_pc = pc;
    char *p = ftrace[ftrace_idex];
    p += snprintf(p, 128, FMT_WORD ":", pc);

    p = strcat(p, "ret");

    for (int i = 0; i < symbol_table->st_size; i++) {
        if (symbol_table[i].st_info == STT_FUNC && (ret_pc >= symbol_table->st_value) && (ret_pc < (symbol_table->st_value + symbol_table->st_size))) {
           p += snprintf(p, 128 - sizeof(p), "%d", string_table[symbol_table[i].st_name]); 
        }
    }

    ftrace_idex++;
     
}

void print_ftrace() {
    for (int i = 0; i < ftrace_idex; i++) {
        printf("%s\n", ftrace[i]);
    }

}