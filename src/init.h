#ifndef __ELF_GESTION_H__
#define __ELF_GESTION_H__

#include <stdio.h>
#include "global_struct.h"

/**
	*BRIEF : Initialise la structure donnée en paramètre avec le fichier.
	*PARAM : elf_struct
						Structure à charger.
					 elf_file
						Fichier à lire. Doit être ouvert avant.
	*RETURN: Entier: 1->OK -1-> ERREUR.
**/
int init_elf_struct(ELF_STRUCT* elf_struct, FILE *elf_file);

/**
	*BRIEF : Libère la mémoire de la structure donnée en paramètre.
	*PARAM : elf_struct
						Structure à libérer.
	*RETURN: Void.
**/
void close_elf_struct(ELF_STRUCT* elf_struct);
char* get_error(ELF_STRUCT* elf_struct);

#endif
