/*
ELF Loader - chargeur/implanteur d'exécutables au format ELF à but pédagogique
Copyright (C) 2012 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
#ifndef __UTIL_H__
#define __UTIL_H__

#include "global_struct.h"

#include <stdlib.h>
#include <elf.h>
#include <string.h>

int is_big_endian();

//If we need to reverse the bytes, return 1
int need_reverse(unsigned char ei_data);

// Renvoi le nom de la section
char* get_name(ELF_STRUCT * elf,int numero);

//Décale l'offset des sections après la section num de size
void maj_offset(ELF_STRUCT * elf, int num, int size);

//Renvoie l'indice de la section d'offset maximum
int max_offset_section(ELF_STRUCT * elf);

//Ajoute un nom de section dans la shstrtab de la structure
void ajout_nom_section(ELF_STRUCT * elf, char * nom);

//Ajoute le contenu de la section dans la structure elf
void ajout_contenu_section(ELF_STRUCT * elf, ELF_STRUCT * elf2, int num);

//Concatène le contenue des sections
void seccat(char * s1, char * s2, char * sf, int size1, int size2);

#define reverse_2(x) ((((x)&0xFF)<<8)|(((x)>>8)&0xFF))
#define reverse_4(x) ((((x)&0xFF)<<24)|((((x)>>8)&0xFF)<<16)|\
						((((x)>>16)&0xFF)<<8)|(((x)>>24)&0xFF))

#define min(x,y) ((x)<(y)?(x):(y))
#endif
