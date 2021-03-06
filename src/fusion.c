#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "global_struct.h"
#include "fusion.h"
#include "util.h"
#include "section_header.h"
#include "fusion_reimp.h"
#include "fusion_symb.h"

void Fusion(ELF_STRUCT * elf1, ELF_STRUCT * elf2) {
	fusion_section(elf1, elf2);
	//fusion_symb(elf1, elf2); //Erreur de segmentation après ajout au reste du code
	fusion_reimp(elf1, elf2);
}

void ajout_section(ELF_STRUCT * elf1, ELF_STRUCT * elf2, int num) {
	//Incrémentation du nombre de section
	elf1->elf_header->e_shnum++;
	int idxsec_max = elf1->elf_header->e_shnum-1;
	
	//Réallocation de l'espace alloué aux sections headers
	elf1->a_shdr = realloc(elf1->a_shdr, sizeof(Elf32_Shdr)*elf1->elf_header->e_shnum);
	
	//Copie de la nouvelle section header
	elf1->a_shdr[idxsec_max] = elf2->a_shdr[num];
	//Modifications de la nouvelle section header
	elf1->a_shdr[idxsec_max].sh_name = elf1->a_shdr[elf1->elf_header->e_shstrndx].sh_size+1;
	elf1->a_shdr[idxsec_max].sh_offset = elf1->a_shdr[max_offset_section(elf1)].sh_offset + elf1->a_shdr[idxsec_max].sh_size;
	
	//Ajout du nom dans shstrtab (via un fonction de util)
	ajout_nom_section(elf1, get_name(elf2, num));
	
	//Ajout du contenu de la section
	ajout_contenu_section(elf1, elf2, num);
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
				if (strcmp(get_name(elf1,i), get_name(elf2,j)) == 0) {
					trouve = 1;
				}
				i++;
			}
			if (!trouve) {
				ajout_section(elf1, elf2, j);
			}
		}
		j++;
	}	
}
