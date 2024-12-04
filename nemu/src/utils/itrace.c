#include <common.h>
#include <cpu/decode.h>
#include <elf.h>

#define IRINGBUF_NUM 16
#define MAX_INST 2000

Elf32_Sym *read_symbol();
char *read_string();
int header_num(char *type);

Elf32_Sym *symbol_table = NULL;
char *string_table = NULL;
void clean_section();

char iringbuf[IRINGBUF_NUM][128];
int buf_idex;

char ftrace[MAX_INST][128];
int ftrace_idex = 0;
int symbol_size;
int string_size;
int space_num = 0;

void init_iring() {
    buf_idex = 0;
}

// void add_iring(Decode *s) {
//     char *p = iringbuf[buf_idex];
//     p += snprintf(p, 128, FMT_WORD ":", s->pc);
//     int ilen = s->snpc - s->pc;
//     uint8_t *inst = (uint8_t *)&s->isa.inst;
//     for (int i = ilen - 1; i >= 0; i --) {
//         p += snprintf(p, 4, " %02x", inst[i]);
//     }
//     p += snprintf(p, 2, " ");
//     int size = 128 - (p - iringbuf[buf_idex]);
//     void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
//     disassemble(p, size, MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.inst, ilen);
//     buf_idex = (buf_idex + 1) % IRINGBUF_NUM;
// }

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

    symbol_size = header_num("sym");
    string_size = header_num("str");

}


void ftrace_call(Decode *s) {


    vaddr_t call_pc = s->dnpc;
    char *p = ftrace[ftrace_idex];
    p += snprintf(p, 128, FMT_WORD ":", s->pc);
    for (int i = 0; i < space_num; i++) {
        *p++ = ' ';
    }
    p += snprintf(p, 128 - (p - ftrace[ftrace_idex]), "call ");
    for (int i = 0; i < symbol_size; i++) {
        if (ELF32_ST_TYPE(symbol_table[i].st_info) == STT_FUNC && (call_pc >= symbol_table[i].st_value) && (call_pc < (symbol_table[i].st_value + symbol_table[i].st_size))) {
           p += snprintf(p, 128 - (p - ftrace[ftrace_idex]), "[%s@0x%08x]", &string_table[symbol_table[i].st_name], call_pc); 
        }
    }
    printf("%s\n", ftrace[ftrace_idex]);
    ftrace_idex++;
    space_num += 2;

}

void ftrace_ret(Decode *s) {

    vaddr_t ret_pc = s->pc;
    char *p = ftrace[ftrace_idex];
    p += snprintf(p, 128, FMT_WORD ":", s->pc);
    space_num -= 2;
    for (int i = 0; i < space_num; i++) {
        *p++ = ' ';
    }
    p += snprintf(p, 128 - (p - ftrace[ftrace_idex]), "ret ");

    for (int i = 0; i < symbol_size; i++) {
        if (ELF32_ST_TYPE(symbol_table[i].st_info) == STT_FUNC && (ret_pc >= symbol_table[i].st_value) && (ret_pc < (symbol_table[i].st_value + symbol_table[i].st_size))) {
           p += snprintf(p, 128 - (p - ftrace[ftrace_idex]), "[%s@0x%08x]", &string_table[symbol_table[i].st_name], ret_pc); 
        }
    }

    printf("%s\n", ftrace[ftrace_idex]);
    ftrace_idex++;
     
}


void clean_header() {
    clean_section();
    free(symbol_table);
    free(string_table);
}