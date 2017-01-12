#ifndef _FUSION_H_
#define _FUSION_H_

void Fusion(ELF_STRUCT * elf1, ELF_STRUCT * elf2);
void fusion_section(ELF_STRUCT * elf1, ELF_STRUCT * elf2);
void ajout_section(ELF_STRUCT * elf1, ELF_STRUCT * elf2, int num);

#endif
