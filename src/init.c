#include <stdlib.h>

#include "global_struct.h"
#include "init.h"
#include "header_elf.h"
#include "section_header.h"
#include "section_elf.h"
#include "table_symbole.h"
#include "reimplantation.h"
/*#include "elf_relocate.h"*/

int init_elf_struct(ELF_STRUCT* elf_struct, FILE *elf_file) {

	int idx_tab_sym = 0;
	int nb_sym;
	int total_size_sections_content = 0;
	int i = 0;

	// Initialise le fichier
	elf_struct->elf_file = elf_file;
	
	// Charge le header dans la structure
	elf_struct->elf_header = malloc( sizeof(Elf32_Ehdr) );
	if (elf_struct->elf_header == NULL) {
		elf_struct->error_code = ERROR_MALLOC_HEADER;
		return -1;
	}
	if ( read_header(elf_struct) == -1 ) {
		elf_struct->error_code = ERROR_READ_HEADER;
		return -1;
	}
	
	// Charge les section headers
	elf_struct->a_shdr = malloc ( sizeof (Elf32_Shdr) * elf_struct->elf_header->e_shnum );
	if (elf_struct->a_shdr == NULL) {
		elf_struct->error_code = ERROR_MALOC_A_SHDR;
		return -1;
	}
	if ( header_section(elf_struct) == -1 ) {
		elf_struct->error_code = ERROR_FILL_A_SHDR;
		return -1;
	}




	
	// Les sections headers sont initialisés, on doit maintenant lire les sections
	//On a besoin de la taille totale des sections
	for (i = 0; i < elf_struct->elf_header->e_shnum; i++) {
		total_size_sections_content += elf_struct->a_shdr[i].sh_size;
	}
	// On alloue la mémoire pour les sections
	elf_struct->sections_content = malloc( sizeof(char *) * elf_struct->elf_header->e_shnum );
	// On remplis le section_content
	for (i = 0; i < elf_struct->elf_header->e_shnum; i++) {
		elf_struct->sections_content[i] = malloc ( sizeof(unsigned char) * elf_struct->a_shdr[i].sh_size );

		if ( read_section(&elf_struct->a_shdr[i], elf_struct, elf_struct->sections_content[i]) == -1 ) {
			fprintf(stderr, "Error while getting section %d's data\n", i);
			// error code
			return -1;
		}
	}

	//Récupère le nombre de symboles
	while(idx_tab_sym < elf_struct->elf_header->e_shnum && elf_struct->a_shdr[idx_tab_sym].sh_type != 2){
		idx_tab_sym += 1;	
	}

	nb_sym = elf_struct->a_shdr[idx_tab_sym].sh_size / sizeof(Elf32_Sym);

	// Charge la table des symboles dans la structure
	elf_struct->a_sym = malloc ( sizeof (Elf32_Sym) * nb_sym );
	if (elf_struct->a_sym == NULL) {
		elf_struct->error_code = ERROR_MALOC_A_SHDR;
		return -1;
	}
	if ( creer_table_symbole(elf_struct) == -1 ) {
		elf_struct->error_code = ERROR_FILL_A_SHDR;
		return -1;
	}
	/*

	
	// Now that symb_index is initialized, we can retrieve the number of symb entries
	nb_symb_entries = get_nb_symb_entries(elf_struct->a_shdr[elf_struct->symb_table_idx]);

	// Allocates memory + init symbol table
	elf_struct->a_sym = malloc( sizeof(Elf32_Sym) * nb_symb_entries );
	if (elf_struct->a_sym == NULL) {
		fprintf(stderr, "Error : symbol table malloc failed. Break.\n");
		// error code
		return -1;
	}
	if ( read_symbols_array_data(elf_struct) == -1 ) {
		fprintf(stderr, "Error while reading symbol table. Break.\n");
		// error code
		return -1;
	}*/

	//Réimplantation
	tab_Reimplantation(elf_struct);
	
	// Init error code
	elf_struct->error_code = 0;
		

	return 1;	

}	

void close_elf_struct(ELF_STRUCT* elf_struct) {

	for (int i = 0; i < elf_struct->elf_header->e_shnum; i++) {
		if (elf_struct->sections_content[i] != NULL) {
			free(elf_struct->sections_content[i]);
		}
	}

	if (elf_struct->elf_header != NULL) free(elf_struct->elf_header);
	if (elf_struct->a_shdr != NULL) free(elf_struct->a_shdr);
	if (elf_struct->a_sym != NULL) free(elf_struct->a_sym);
	if (elf_struct->sections_content != NULL) free(elf_struct->sections_content);
	if (elf_struct->a_rel != NULL) free(elf_struct->a_rel);
	if (elf_struct->a_rela != NULL) free(elf_struct->a_rela);
	if (elf_struct->elf_file != NULL) fclose(elf_struct->elf_file);
	if (elf_struct != NULL) free(elf_struct);

}

char* get_error(ELF_STRUCT* elf_struct) {
	
	switch(elf_struct->error_code) {
		case ERROR_MALLOC_HEADER: return "Error malloc header";
		case ERROR_READ_HEADER: return "Error while reading elf_header (most likely corrupted file)";
		case ERROR_MALOC_A_SHDR: return "Error malloc a_shdr";
		default: return "Unknown";
	}

}
