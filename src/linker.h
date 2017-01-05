#ifndef __LINKER_H__
#define __LINKER_H__

#include "global_struct.h"

void affichageComplet(ELF_STRUCT* elf_struct);
void affichageHeader(ELF_STRUCT* elf_struct);
void affichageSectionHeader(ELF_STRUCT* elf_struct);
void affichageContenuSection(ELF_STRUCT* elf_struct);
void affichageSymbole(ELF_STRUCT* elf_struct);
void affichageReimplementation(ELF_STRUCT* elf_struct);
void fusion(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);

#endif
