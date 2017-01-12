#ifndef _section_
#define _section_

#include "global_struct.h"

/**
	*BRIEF : Affiche le section header de la structure passée en paramètre.
	*PARAM : elf
						structure à afficher.
	*RETURN: Void (affiche simplement)
**/
void Affichage_section(ELF_STRUCT * elf );

/**
	*BRIEF : Charge en mémoire la partie du header dans la structure.
	*PARAM : elf
						Structure à charger (elf_struct->file est le fichier à lire).
	*RETURN: Entier: 1->OK -1-> ERREUR.
**/
int header_section(ELF_STRUCT * elf );

/**
	*BRIEF : Affiche la traduction d'un flag en texte.
	*PARAM : flag
						Variable à traduire
	*RETURN: void, affichage simple
**/
void case_flags(Elf32_Word flag);

/**
	*BRIEF : Traduit une variable de type en texte.
	*PARAM : sh_type
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* case_type(Elf32_Word sh_type);

#endif

