#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "elf_section_header.h"
#include "elf_header.h"
#include "elf_struct.h"
#include "util.h"

//FIXED
int display_section_headers_array(ELF_STRUCT* elf_struct) {

	int i; // for loops
	char *type;
	char *flags;
	int nbHeaders = elf_struct->elf_header->e_shnum;
	
	printf("Sections header:\n");
	printf("╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                    \e[1;31mSectionHeader:\e[0m                                                               ║\n");
    printf("╠════════╤═══════════════════════╤═════════════════╤══════════════╤════════════════╤══════════════╤════════╤══════════╤═════════╤═════════╤═══════╣\n");
	printf("║ Num:   │          Name         │      Type       │     Addr     │     Offset     │    Size      │   ES   │   FLAG   │   Lk    │  Inf    │   Al  ║\n");
	printf("╟────────┼───────────────────────┼─────────────────┼──────────────┼────────────────┼──────────────┼────────┼──────────┼─────────┼─────────┼───────╢\n");
	for (i = 0; i < nbHeaders; i++) {
		
		char *name;
		int size;

		// get type
		type = get_shdr_type(elf_struct->a_shdr[i].sh_type);
		if (type == NULL) { 
			printf("error : unknown type. Break.\n");
			return -1;
		}
		
		size = get_section_name_size(elf_struct, i);
		name = malloc(size * sizeof(char) + 1);
		if (name == NULL) {
			printf("malloc error. Break.\n");
			return -1;	
		}
		get_section_name(elf_struct, i, name);

		if (name == NULL) {
			printf("error while reading name number %d.\n",i);
			return -1;
		}		

		// get flags
		flags = get_shdr_flags(elf_struct->a_shdr[i].sh_flags);
		if (flags == NULL) {
			fprintf(stderr, "error while parsing flags. Break.\n");
			return -1;
		}

		// display
		printf("║   %2d   │ %-20s  │ %-14s  │   %08x   │     %06x     │    %06x    │   %02d   │    %2s    │   %2d    │   %2d    │  %2d   ║\n",
				i, name, type, elf_struct->a_shdr[i].sh_addr, elf_struct->a_shdr[i].sh_offset, 
				elf_struct->a_shdr[i].sh_size, elf_struct->a_shdr[i].sh_entsize, 
				flags, elf_struct->a_shdr[i].sh_link, elf_struct->a_shdr[i].sh_info, 
				elf_struct->a_shdr[i].sh_addralign);
		
		free(name);

	}
	printf("╚════════╧═══════════════════════╧═════════════════╧══════════════╧════════════════╧══════════════╧════════╧══════════╧═════════╧═════════╧═══════╝\n\n");
	printf("FLAGS :\n");
	printf("W (Write), A (Allocation), X (Executable instruction), M (Merge), S (Strings)\n");
	printf("I (Info), L (Link order), G (Groups), T (TLS), E (Exclusion), x (Unknown)\n");
	printf("O (Additionnal treatment requiered for OS), o (OS specific), p (Proc specific)\n\n"); 

	return 1;

}

// FIXED
int fill_shdr(ELF_STRUCT* elf_struct, int num_section) {
	
	int shdr_size = sizeof(Elf32_Shdr);

	if ( fread(&(elf_struct->a_shdr[num_section]), shdr_size, 1, elf_struct->elf_file) != 1 ) {
		fprintf(stderr, "Error while reading shdr\n");
		return -1;
	}

	if ( this_needs_reverse(elf_struct->elf_header->e_ident[EI_DATA]) ) {

		elf_struct->a_shdr[num_section].sh_name = reverse_4(elf_struct->a_shdr[num_section].sh_name);
		elf_struct->a_shdr[num_section].sh_type = reverse_4(elf_struct->a_shdr[num_section].sh_type);
		elf_struct->a_shdr[num_section].sh_flags = reverse_4(elf_struct->a_shdr[num_section].sh_flags);
		elf_struct->a_shdr[num_section].sh_addr = reverse_4(elf_struct->a_shdr[num_section].sh_addr);
		elf_struct->a_shdr[num_section].sh_offset = reverse_4(elf_struct->a_shdr[num_section].sh_offset);
		elf_struct->a_shdr[num_section].sh_size = reverse_4(elf_struct->a_shdr[num_section].sh_size);
		elf_struct->a_shdr[num_section].sh_link = reverse_4(elf_struct->a_shdr[num_section].sh_link);
		elf_struct->a_shdr[num_section].sh_info = reverse_4(elf_struct->a_shdr[num_section].sh_info);
		elf_struct->a_shdr[num_section].sh_addralign = reverse_4(elf_struct->a_shdr[num_section].sh_addralign);
		elf_struct->a_shdr[num_section].sh_entsize = reverse_4(elf_struct->a_shdr[num_section].sh_entsize);

	}

	return 1;

}

// FIXED
int fill_shdr_array(ELF_STRUCT* elf_struct) {
	
	int i = 0;	

	fseek(elf_struct->elf_file, elf_struct->elf_header->e_shoff, SEEK_SET);

	for(i = 0; i < elf_struct->elf_header->e_shnum; i++) {			
		
		if( !fill_shdr(elf_struct, i) ) {
			fprintf(stderr, "Section Header error. Break.\n");
			return -1;
		}

	}

	return 1;

}

// FIXED
int get_section_name_size(const ELF_STRUCT* elf_struct, int num_section) {

	int i = 0;
	char c = 10; // Must not equal 0
	int offset = (int) elf_struct->a_shdr[elf_struct->elf_header->e_shstrndx].sh_offset;

	// Get size
	fseek(elf_struct->elf_file, elf_struct->a_shdr[num_section].sh_name + offset, SEEK_SET);
	for (i = 0; c != '\0'; i++) {
		c = fgetc(elf_struct->elf_file);
	}

	return i + 1;

}

// FIXED
void get_section_name(const ELF_STRUCT* elf_struct, int num_section, char *name) {
	
	int i = 0;
	char c = 10; // Must not equal 0
	int offset = (int) elf_struct->a_shdr[elf_struct->elf_header->e_shstrndx].sh_offset;

	// Get name
	fseek(elf_struct->elf_file, elf_struct->a_shdr[num_section].sh_name + offset, SEEK_SET);
	for (i = 0; c != '\0'; i++) {
		c = fgetc(elf_struct->elf_file);
		name[i] = c;		
	}
	name[i + 1] = '\0';

}

// FIXED
int get_section_num(const ELF_STRUCT* elf_struct, char* name) {
	
	int i = 0;
	char* section_name = NULL;
	int section_name_size = 0;

	// for each section, looks if it's the one we're looking for (by its name)
	for (i = 0; i < elf_struct->elf_header->e_shnum; i++) {
		
		section_name_size = get_section_name_size(elf_struct, i);
		section_name = malloc( sizeof(char) * section_name_size );
		if (section_name == NULL) {
			fprintf(stderr, "error malloc: section_name\n");
			return -1;
		}
		get_section_name(elf_struct, i, section_name);

		if ( strcmp(section_name, name) == 0 ) {
			free(section_name);
			return i;
		}

		free(section_name);
	
	}

	return -1;

}

char* get_shdr_type(Elf32_Word sh_type) {

	switch(sh_type) {

		case(SHT_NULL): 			return "NULL";		    
		case(SHT_PROGBITS): 		return "PROGBITS";		         
		case(SHT_SYMTAB): 			return "SYMTAB";			              
		case(SHT_STRTAB): 			return "STRTAB";			             
		case(SHT_RELA): 			return "RELA";               			
		case(SHT_HASH): 			return "HASH";              			                
		case(SHT_DYNAMIC): 			return "DYNAMIC";              			             
		case(SHT_NOTE): 			return "NOTE";              			                
		case(SHT_NOBITS): 			return "NOBITS";              			              
		case(SHT_REL): 				return "REL";             		                 
		case(SHT_SHLIB):			return "SHLIB";             			              
		case(SHT_DYNSYM): 			return "DYNSYM";               			             
		case(SHT_LOPROC): 			return "LOPROC";              			     
		case(SHT_HIPROC): 			return "HIPROC";        
		case(SHT_LOUSER): 			return "LOUSER";               		     
		case(SHT_HIUSER): 			return "HIUSER";              		
		case(SHT_ARM_EXIDX): 		return "ARM_EXIDX";              		
		case(SHT_ARM_PREEMPTMAP): 	return "ARM_PREEMPTMAP";              			
		case(SHT_ARM_ATTRIBUTES): 	return "ARM_ATTRIBUTES"; 
		default: 					return "";

	}

}

char* get_shdr_flags(Elf32_Word sh_flags) {

	switch(sh_flags) {

		case(SHF_WRITE): 					return "W";	    
		case(SHF_ALLOC): 					return "A";			         
		case(SHF_WRITE + SHF_ALLOC): 		return "AW";			
		case(SHF_EXECINSTR): 				return "X";			              
		case(SHF_ALLOC + SHF_EXECINSTR): 	return "AX";		
		case(SHF_MASKPROC): 				return "p";			
		case(SHF_LINK_ORDER): 				return "L";		
		case(SHF_MERGE): 					return "M";
		default: 							return "";
			
	}

}