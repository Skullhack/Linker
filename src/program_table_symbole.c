#include <stdlib.h>
#include <stdio.h>

#include "global_struct.h"
#include "init.h"
#include "header_elf.h"
#include "section_header.h"
#include "table_symbole.h"
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

	afficher_table(elf_struct);

	close_elf_struct(elf_struct);

	return EXIT_SUCCESS;

}
