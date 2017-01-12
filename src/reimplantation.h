#ifndef _reimpl_

#define _reimpl_

#include <elf.h>

#include "section_header.h"
#include "util.h"
#include "global_struct.h"

/**
	*BRIEF : Charge en mémoire la partie de la table de réimplantation dans la structure
	*PARAM : elf
						Structure à charger (elf_struct->file est le fichier à lire)
	*RETURN: Entier: 1->OK -1-> ERREUR
**/
int tab_Reimplantation(ELF_STRUCT * elf);

/**
	*BRIEF : Afficher la table de réimplantation des Rel.
	*PARAM : elf:
						Pointeur sur la structure à afficher.
	*RETURN: void. affichage simple
**/
void Affichage_Rel(ELF_STRUCT * elf);

/**
	*BRIEF : Afficher la table de réimplantation des Rela.
	*PARAM : elf:
						Pointeur sur la structure à afficher.
	*RETURN: void. affichage simple
**/
void Affichage_Rela(ELF_STRUCT * elf);
#endif
