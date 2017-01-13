#ifndef __SECTION_ELF_H__
#define __SECTION_ELF_H__

#include <stdlib.h>
#include <elf.h>
#include <stdio.h>
#include "global_struct.h"

/**
	*BRIEF : Lance l'affichage de la section N°nSec ou de nom nSec
					 Cette fontion choisie quelle section afficher apres avoir traité le nSec.
	*PARAM : elf_struct:
						structure dans laquelle la section doit être lu.
					 nSec:
						String contenant soit un nombre soit un nom.
	*RETURN: Integer: 1->OK, -1->ERREUR
**/
int choix_section(ELF_STRUCT * elf_struct, char* nSec);

/**
	*BRIEF : Affiche la section N°id
	*PARAM : elf_struct:
						structure dans laquelle la section doit être lu.
					 id:
						indice de la section à afficher
	*RETURN: Integer: 1->OK, -1->ERREUR
**/
int display_section(ELF_STRUCT* elf_struct, int id) ;

/**
	*BRIEF : Charge en mémoire la partie de la section dans la structure.
	*PARAM : monShdr:
						Pointeur sur le section header.
					 elf_struct:
						Structure à charger (elf_struct->file est le fichier à lire).
					 section_content:
						Pointeur sur la section à charger, où stocker les données.
	*RETURN: Entier: 1->OK -1-> ERREUR.
**/
int read_section(Elf32_Shdr* monShdr, ELF_STRUCT* elf_struct, char* section_content);

#endif
