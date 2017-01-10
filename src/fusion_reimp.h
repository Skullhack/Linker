#ifndef __FUSION_REIMP_H__
#define __FUSION_REIMP_H__

void offset_section_update(int ind_sect, Elf32_Word size, ELF_STRUCT * elf);
int max_offset_section(ELF_STRUCT * elf);
void fusion_reimp(ELF_STRUCT * elf_file1, ELF_STRUCT * elf_file2)

#endif
