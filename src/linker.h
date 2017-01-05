#ifndef __LINKER_H__
#define __LINKER_H__

#include "global_struct.h"

void affichageComplet(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void affichageHeader(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void affichageSectionHeader(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void affichageContenuSection(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void affichageSymbole(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void affichageReimplementation(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void fusion(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);

#endif
