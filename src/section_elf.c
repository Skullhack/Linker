#include "util.h"
#include "section_elf.h"
#include <stdint.h>



int display_section(ELF_STRUCT* elf_struct, int id) {

		printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                             \e[1;31mELF Section n°:%d\e[0m                             ║\n",id);
    printf("╠════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");

		printf("║ Contenu:%s",elf_struct->sections_content[id]);
		printf("║ AFFICHAGE A TERMINER APRES TEST");


	return 1;

}


int read_section(Elf32_Shdr* monShdr, ELF_STRUCT* elf_struct, unsigned char* section_content) {

	Elf32_Off offset = monShdr->sh_offset; //debut de la section
	uint32_t size = (uint32_t) monShdr->sh_size; //taille de la section

	fseek(elf_struct->elf_file, offset, SEEK_SET); //on deplace la tete sur le debut

	if ( fread(section_content, size, 1, elf_struct->elf_file) == -1) { //on read et la place dans le section_content passé en paramètre
		return -1;
	}


	return 1;

}
