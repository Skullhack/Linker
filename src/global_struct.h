#ifndef __ELF_STRUCT_H__
#define __ELF_STRUCT_H__

#include <elf.h>
#include <stdio.h>

#define ERROR_MALLOC_HEADER -1
#define ERROR_READ_HEADER -2
#define ERROR_MALOC_A_SHDR -3
#define ERROR_FILL_A_SHDR -4

typedef struct ELF_STRUCT {

	FILE* elf_file; 				//Le fichier
	Elf32_Ehdr *elf_header;	//Le header
	Elf32_Shdr *a_shdr;			//Tableau de section header
	Elf32_Sym *a_sym;				//Tableau de symbole
	Elf32_Rel *a_rel;				//Tableau de réimplantation Rel
	Elf32_Rela *a_rela;			//Tableau de réimplantation Rela (toujours vide car non géré)
	char **sections_content;//Tableau de char*, chaque char* est le contenu d'un section
	int * tab_reimplant;		//
	int taille_rel;					//Nombre de réimplantation Rel
	int taille_rela;				//Nombre de réimplantation Rel (toujours vide car non géré)
	int error_code; 

} ELF_STRUCT;

#endif
