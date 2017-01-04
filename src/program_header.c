#include <stdlib.h>
#include <stdio.h>

#include "struct_elf.h"
#include "gestion_elf.h"
#include "header_elf.h"
#include "util.h"

int main(int argc, char *argv[]) {

	FILE *f = NULL;
	ELF_STRUCT* elf_struct = NULL;

	// Checks for argc
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

	// Displays header
	display_header(elf_struct->elf_header);

	// Clean exit
	close_elf_struct(elf_struct);

	return EXIT_SUCCESS;

}
