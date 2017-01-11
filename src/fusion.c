#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "global_struct.h"
#include "fusion.h"
#include "util.h"
#include "section_header.h"
#include "fusion_reimp.h"

void Fusion(ELF_STRUCT * elf1, ELF_STRUCT * elf2) {
	fusion_section(elf1, elf2);
	//fusion_reimp(elf1, elf2);
}

void ajout_section(ELF_STRUCT * elf1, ELF_STRUCT * elf2, int num) {
	int offset_supp = 0;
	
	//Incrémentation du nombre de section
	elf1->elf_header->e_shnum++;
	int idxsec_max
	
	//Réallocation de l'espace alloué aux sections headers
	elf1->a_shdr = realloc(elf1->a_shdr, sizeof(Elf32_Shdr)*elf1->elf_header->e_shnum);
	
	//Copie de la nouvelle section header
	elf1->a_shdr[e_shnum-1] = elf2->a_shdr[num];
	
	//Modifications de la nouvelle section header
	elf1->a_shdr[e_shnum-1].sh_name = elf1->a_shdr[elf1->elf_header->e_shstrndx].sh_size+1;
	elf1->a_shdr[e_shnum-1]
}

void seccat(char * s1, char * s2, char * sf, int size1, int size2) {
	int i = 0;
	int j;
	sf[0] = '\0';
	
	while (i < size1) {
		sf[i] = s1[i];
		i++;
	}
	j = i;
	i = 0;
	while (i < size2) {
		sf[j] = s2[i];
		i++;
		j++;
	}
}

void fusion_section(ELF_STRUCT * elf1, ELF_STRUCT * elf2) {
	Elf32_Shdr * shelf1 = elf1->a_shdr;
	Elf32_Shdr * shelf2 = elf2->a_shdr;
	char * cont_section1;
	char * cont_section2;
	char * cont_final;
	int i = 0;
	int j = 0;
	int trouve = 0;
	//unsigned char varAff;

	while (i < elf1->elf_header->e_shnum) {
		if (shelf1[i].sh_type == SHT_PROGBITS) {
			j = 0;
			while (j < elf2->elf_header->e_shnum) {
				if (shelf2[j].sh_type == SHT_PROGBITS && strcmp(get_name(elf1,i), get_name(elf2,j)) == 0) {
					cont_section1 = malloc(sizeof(char)*shelf1[i].sh_size);
					cont_section2 = malloc(sizeof(char)*shelf2[j].sh_size);
					cont_final = malloc(sizeof(char)*(shelf1[i].sh_size+shelf2[j].sh_size));
					cont_section1 = elf1->sections_content[i];
					cont_section2 = elf2->sections_content[j];
					seccat(cont_section1, cont_section2, cont_final, shelf1[i].sh_size, shelf2[j].sh_size);
					
					/*printf("Début section %s\n", get_name(elf1,i));
					for (int k = 0; k < shelf1[i].sh_size; k++) {
						varAff = cont_section1[k];
						printf("%x",varAff);
					}
					printf("\n");
					for (int k = 0; k < shelf2[j].sh_size; k++) {
						varAff = cont_section2[k];
						printf("%x",varAff);
					}
					printf("\n");
					for (int k = 0; k < shelf1[i].sh_size+shelf2[j].sh_size; k++) {
						varAff = cont_final[k];
						printf("%x",varAff);
					}
					printf("\nFin section %s\n\n", get_name(elf1,i));*/
					
					elf1->a_shdr[i].sh_size = shelf1[i].sh_size+shelf2[j].sh_size;
					elf1->sections_content[i] = realloc(elf1->sections_content[i], elf1->a_shdr[i].sh_size);
					elf1->sections_content[i] = cont_final;
					maj_offset(elf1, i, shelf2[j].sh_size);
				}
				j++;
			}
		}
		i++;
	}
	
	j = 0;
	while (j < elf2->elf_header->e_shnum) {
		if (shelf2[j].sh_type == SHT_PROGBITS) {
		i = 0;
		trouve = 0;
		while (i < elf1->elf_header->e_shnum) {
			if (shelf1[i].sh_type == SHT_PROGBITS && strcmp(get_name(elf1,i), get_name(elf2,j)) == 0) {
				trouve = 1;
			}
		}
		
		if (!trouve) {
			ajout_section(elf1, elf2, j);
		}
	}





















	
}

