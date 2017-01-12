/*
ELF Loader - chargeur/implanteur d'ex�cutables au format ELF � but p�dagogique
Copyright (C) 2012 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

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

/**
	*BRIEF : Fait un reverse 
	*PARAM : ei_data:
						variable � reverse
	*RETURN: version reverse de la variable.
**/
int need_reverse(unsigned char ei_data);

/**
	*BRIEF : Renvoi un le nom d'un section header
	*PARAM : elf:
						structure � lire
					 numero:
						num�ro du section header a lire le nom
	*RETURN: Une chaine de caract�re avec le nom.
**/
char* get_name(ELF_STRUCT * elf,int numero);

//D�cale l'offset des sections apr�s la section num de size
/**
	*BRIEF : Met a jour les offset des sections fusionn�es (dans le section header)
	*PARAM : elf:
						structure � mettre � jour
					 num:
						num�ro de la premiere section � mettre � jour
					 size:
						taille de la section
	*RETURN: void. Mise � jour seulement
**/
void maj_offset(ELF_STRUCT * elf, int num, int size);

//Renvoie l'indice de la section d'offset maximum
/**
	*BRIEF : Renvoie l'indice de la section d'offset maximum
	*PARAM : elf:
						structure � lire
	*RETURN: l'indice de cette section
**/
int max_offset_section(ELF_STRUCT * elf);

//Ajoute un nom de section dans la shstrtab de la structure
/**
	*BRIEF : Ajoute un nom de section dans la shstrtab de la structure
	*PARAM : elf:
						structure � modifier
					 nom:
						Nom de la section � ajouter
	*RETURN: void. Ajout seulement
**/
void ajout_nom_section(ELF_STRUCT * elf, char * nom);

//Ajoute le contenu de la section dans la structure elf
/**
	*BRIEF : Ajoute le contenu de la section dans la structure elf
	*PARAM : elf:
						structure o� mettre le contenu
					 elf2:
						structure o� lire le contenu
					 num:
						Num�ro de la section content � ajouter
	*RETURN: void. Ajout seulement
**/
void ajout_contenu_section(ELF_STRUCT * elf, ELF_STRUCT * elf2, int num);

//Concat�ne le contenue des sections
/**
	*BRIEF : Concat�ne le contenue des sections
	*PARAM : s1:
						Contenu de la premiere section
					 s2:
						Contenu de la deuxieme section
					 sf:
						Concat�nation des deux section
					 size1:
						Taille de la premiere section
					 size2
						Taille de la seconde section
	*RETURN: void. La chaine concat�n�e est sf
**/
void seccat(char * s1, char * s2, char * sf, int size1, int size2);

#define reverse_2(x) ((((x)&0xFF)<<8)|(((x)>>8)&0xFF))
#define reverse_4(x) ((((x)&0xFF)<<24)|((((x)>>8)&0xFF)<<16)|\
						((((x)>>16)&0xFF)<<8)|(((x)>>24)&0xFF))

#define min(x,y) ((x)<(y)?(x):(y))
#endif
