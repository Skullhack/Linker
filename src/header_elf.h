#ifndef __ELF_HEADER_H__
#define __ELF_HEADER_H__

#include <elf.h>
#include <stdio.h>
#include "global_struct.h"

/**
	*BRIEF : Afficher le header.
	*PARAM : elf_header:
						Pointeur sur la variable Elf38_Ehdr à afficher.
**/
int display_header(Elf32_Ehdr *elf_header);

/**
	*BRIEF : Charge en mémoire la partie du header dans la structure
	*PARAM : elf_struct
						Structure à charger (elf_struct->file est le fichier à lire)
	*RETURN: Entier: 1->OK -1-> ERREUR
**/
int read_header(ELF_STRUCT* elf_struct);

/**
	*BRIEF : Traduit une variable classe de fichier en texte.
	*PARAM : ei_class
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* get_file_class(unsigned char ei_class);


/**
	*BRIEF : Traduit une variable de données encodées en texte.
	*PARAM : ei_data
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* get_data_encoding(unsigned char ei_data);

/**
	*BRIEF : Traduit une variable de version de header en texte.
	*PARAM : ei_version
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* get_header_version(unsigned char ei_version);

/**
	*BRIEF : Traduit une variable de système d'exploitation en texte.
	*PARAM : ei_osabi
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* get_operating_system(unsigned char ei_osabi);

/**
	*BRIEF : Traduit une variable de type d'objet en texte.
	*PARAM : e_type
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* get_object_file_type(Elf64_Half e_type);

/**
	*BRIEF : Traduit une variable d'architecture de fichier en texte.
	*PARAM : e_machine
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* get_architecture(Elf64_Half e_machine);

/**
	*BRIEF : Traduit une variable de version d'objet de fichier en texte.
	*PARAM : e_version
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* get_object_file_version(Elf32_Word e_version);


#endif
