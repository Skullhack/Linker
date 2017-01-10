#ifndef __SECTION_ELF_H__
#define __SECTION_ELF_H__

#include <stdlib.h>
#include <elf.h>
#include <stdio.h>
#include "global_struct.h"

/*
Demande le numéro ou le nom de la section à afficher avant de lancer l'affichage
*/
int choix_section(ELF_STRUCT * elf_struct);

/*
Affiche la section indiqué en paramètre par le "id". Il peut etre soit un nombre soit un nom de section
*/
int display_section(ELF_STRUCT* elf_struct, int id) ;

#endif
