#include <common.h>
#include <elf.h>

FILE *elf_fp = NULL;
Elf32_Shdr *symtab_header = NULL;
Elf32_Shdr *strtab_header = NULL;
void read_elf();

void init_elf(const char *elf_file) {
    if (elf_file != NULL && strcmp(elf_file, "--batch") != 0) {
        FILE *fp = fopen(elf_file, "r");
        Assert(fp, "Can not open '%s'", elf_file);
        elf_fp = fp;
        Log("Loading the ELF file at location %s", elf_file);
    }
    read_elf();
}

void read_elf() {
    Elf32_Ehdr elf_header;
    size_t read_size = fread(&elf_header, sizeof(Elf32_Ehdr), 1, elf_fp);
    Assert(read_size == 1, "Failed to read ELF headers");

    Elf32_Shdr *section_headers = malloc(elf_header.e_shnum * elf_header.e_shentsize);
    fseek(elf_fp, elf_header.e_shoff, SEEK_SET);
    read_size = fread(section_headers, elf_header.e_shentsize, elf_header.e_shnum, elf_fp);
    Assert(read_size == elf_header.e_shnum, "Failed to read section headers");   

    for (int i = 0; i < elf_header.e_shnum; i++) {
        if (section_headers[i].sh_type == SHT_SYMTAB)
        {
            symtab_header = &section_headers[i];
        }
        else if (section_headers[i].sh_type == SHT_STRTAB && i != elf_header.e_shstrndx) {
            strtab_header = &section_headers[i];
        }
    }
}
int header_num(char *type) {
    if (strcmp(type, "str") == 0) {
        return strtab_header->sh_size;
    }
    else if(strcmp(type, "sym") == 0){
        return symtab_header->sh_size / symtab_header->sh_entsize;
    }
    return -1;
}

Elf32_Sym *read_symbol() {

    Elf32_Sym *symbol_table = malloc(symtab_header->sh_size);
    fseek(elf_fp, symtab_header->sh_offset, SEEK_SET);
    size_t read_size = fread(symbol_table, symtab_header->sh_size, 1, elf_fp);
    Assert(read_size == 1, "Failed to read symbol headers");

    return symbol_table;
}

char *read_string() {
    char *string_table = malloc(strtab_header->sh_size);
    fseek(elf_fp, strtab_header->sh_offset, SEEK_SET);
    size_t read_size = fread(string_table, strtab_header->sh_size, 1, elf_fp);
    Assert(read_size == 1, "Failed to read string headers");


    return string_table;
}
