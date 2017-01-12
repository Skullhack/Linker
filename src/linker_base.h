#ifndef __LINKER_H__
#define __LINKER_H__

#include "global_struct.h"

int GetInteger(const char *prompt, int *i);
void affichageComplet(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void affichageHeader(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void affichageSectionHeader(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void affichageContenuSection(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void affichageSymbole(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void affichageReimplantation(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);
void lancer_fusion(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);

#endif
