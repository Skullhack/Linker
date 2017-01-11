#ifndef _FUSION_H_
#define _FUSION_H_

void Fusion(ELF_STRUCT * elf1, ELF_STRUCT * elf2);
void seccat(char * s1, char * s2, char * sf, int size1, int size2);
void fusion_section(ELF_STRUCT * elf1, ELF_STRUCT * elf2);
void ajout_section(ELF_STRUCT * elf1, ELF_STRUCT * elf2, int num);

#endif
