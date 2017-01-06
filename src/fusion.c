#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "global_struct.h"
#include "fusion.h"

int Fusion(ELF_STRUCT * elf1, ELF_STRUCT * elf2) {
	ELF_STRUCT * elf_fusion = NULL;
	Elf32_Shdr * shelf1 = elf1->a_shdr;
	Elf32_Shdr * shelf2 = elf2->a_shdr;
	int i = 0;
	int j = 0;
	int k = 0;

	while (i < elf1->elf_header->e_shnum) {
		if (shelf1-

		i++;
	}
		

	return 1;
}
