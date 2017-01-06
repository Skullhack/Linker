#ifndef __SECTION_ELF_H__
#define __SECTION_ELF_H__

#include <stdlib.h>
#include <elf.h>
#include <stdio.h>
#include "global_struct.h"

/*
Affiche la section indiqué en paramètre par le "id". Il peut etre soit un nombre soit un nom de section
*/
int display_section(ELF_STRUCT* elf_struct, int id) ;

/*
Remplis la structure avec toutes les sections
*/
int read_section(Elf32_Shdr* shdr, ELF_STRUCT* elf_struct, unsigned char* section_content);

#endif
