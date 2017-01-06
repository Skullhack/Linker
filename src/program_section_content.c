#include <stdlib.h>
#include <stdio.h>

#include "global_struct.h"
#include "init.h"
#include "header_elf.h"
#include "section_header.h"
#include "section_elf.h"
#include "util.h"

int main(int argc, char *argv[]) {

	FILE *f = NULL;
	ELF_STRUCT* elf_struct = NULL;

	if (argc != 2) {
		fprintf(stderr, "must have exactly 1 arg (file name)\n");
		return EXIT_FAILURE;
	}
	
	f = fopen(argv[1], "r");
	if (f == NULL) {
		fprintf(stderr, "Error: can't open file %s in read mode\n", argv[1]);
		return EXIT_FAILURE;
	}

	elf_struct = malloc( sizeof(ELF_STRUCT) );
	if (elf_struct == NULL) {
		fprintf(stderr, "Error malloc elf_struct\n");
		return EXIT_FAILURE;
	}

	if ( !init_elf_struct(elf_struct, f) ) {
		fprintf(stderr, "Error init: %s", get_error(elf_struct));
		return EXIT_FAILURE;
	}
	
	printf("Quelle section afficher ?\n");

	char sec[100];
	int nbSection = elf_struct->elf_header->e_shnum;

	printf("Saisir le numéro (la recherche par nom n'est pas implanté pour le moment...):");
	fgets(sec, sizeof(sec),stdin);

	if (sec[0]=='.') { //a terminer, si le nom est saisie.
		return -1;
		/*int nbSection = elf_struct->elf_header->e_shnum;
		int nametmp;		

		for (int i=0; i<nbSection ; i++ ) {
			
			nametmp = (int)  elf_struct->a_shdr[i].sh_name;
			if ( elf_struct->a_shdr[i].sh_name==sec) {
				display_section(elf_struct,i);
			}
		}
		display_section(elf_struct,atoi(sec));*/
	} else {
		if (atoi(sec)>=nbSection || atoi(sec)<0) {
			printf("La section %d n'existe pas.",atoi(sec));
			return -1;
		}
		display_section(elf_struct,atoi(sec));
	}

	close_elf_struct(elf_struct);

	return EXIT_SUCCESS;

}
