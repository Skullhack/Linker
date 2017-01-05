#ifndef __ELF_GESTION_H__
#define __ELF_GESTION_H__

#include <stdio.h>
#include "global_struct.h"

int init_elf_struct(ELF_STRUCT* elf_struct, FILE *elf_file);
void close_elf_struct(ELF_STRUCT* elf_struct);
char* get_error(ELF_STRUCT* elf_struct);

#endif
