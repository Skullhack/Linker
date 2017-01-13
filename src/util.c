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
#include "util.h"
#include <stdint.h>

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Informe si le poste est en big_endian ou non
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int is_big_endian() {
	static uint32_t one = 1;
	return ((* (uint8_t *) &one) == 0);
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Fait un reverse 
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int need_reverse(unsigned char ei_data) {
	if (ei_data == ELFDATA2LSB)
		return (ei_data == ELFDATA2LSB && is_big_endian());
	else 
		return (ei_data == ELFDATA2MSB && !is_big_endian());
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi un le nom d'un section header
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* get_name(ELF_STRUCT * elf,int numero){
	int offset = elf->a_shdr[numero].sh_name;
	char * shstr = elf->sections_content[elf->elf_header->e_shstrndx];
	char * str;
	int taille_mot = 1;
	int j = 0;
	int i = offset;
	
	while (shstr[i] != '\0') {
		taille_mot++;
		i++;
	}
	taille_mot++;
	
	str = malloc(sizeof(char)*taille_mot);
	for (i = offset; i < taille_mot+offset-1; i++) {
		str[j] = shstr[i];
		j++;
	}
	str[taille_mot] = '\0';

	return str;
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Décale l'offset des sections après la section num de size
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void maj_offset(ELF_STRUCT * elf, int num, int size) {
	Elf32_Shdr * shelf = elf->a_shdr;
	int allign = shelf[num].sh_addralign;
	
	for (int i = 0; i < elf->elf_header->e_shnum; i++) {
		if ((shelf[i].sh_offset > shelf[num].sh_offset) || ((shelf[i].sh_offset == shelf[num].sh_offset) && (i > num))) {
			shelf[i].sh_offset = shelf[i].sh_offset + size;
			if ((shelf[i].sh_offset % allign) != 0)
				shelf[i].sh_offset = shelf[i].sh_offset + (allign - (shelf[i].sh_offset % allign));
		}
	}
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoie l'indice de la section d'offset maximum
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int max_offset_section(ELF_STRUCT * elf) {
	int i = 0, ind_max = 0, max_offset = 0;
	
	ind_max = i;
	max_offset = elf->a_shdr[i].sh_offset ;

	for(i = 1; i < elf->elf_header->e_shnum ; i++){
	
		if(elf->a_shdr[i].sh_offset > max_offset){
		
			ind_max = i;
			max_offset = elf->a_shdr[i].sh_offset;		
			
		}
	}
	
	return ind_max;
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ajoute un nom de section dans la shstrtab de la structure
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ajout_nom_section(ELF_STRUCT * elf, char * nom) {
	int taille_nom = strlen(nom)+2;
	int shstr_size = elf->a_shdr[elf->elf_header->e_shstrndx].sh_size;
	int i;
	nom[taille_nom] = '\0';
	
	//Modification de l'en-tête de section de shstr
	elf->a_shdr[elf->elf_header->e_shstrndx].sh_size = elf->a_shdr[elf->elf_header->e_shstrndx].sh_size + taille_nom;
	
	//Réallocation de la mémoire pour stocker le nom	
	elf->sections_content[elf->elf_header->e_shstrndx] = realloc(elf->sections_content[elf->elf_header->e_shstrndx], elf->a_shdr[elf->elf_header->e_shstrndx].sh_size+1);
	
	//Concaténation du nom dans la shstrtab
	for (i = 0; i < taille_nom; i++) {
		elf->sections_content[elf->elf_header->e_shstrndx][shstr_size + i + 1] = nom[i];
	}
	
	//Mise à jour des offsets suivants la shstrtab
	maj_offset(elf, elf->elf_header->e_shstrndx, taille_nom);
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ajoute le contenu de la section dans la structure elf
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ajout_contenu_section(ELF_STRUCT * elf, ELF_STRUCT * elf2, int num) {
	elf->sections_content = realloc(elf->sections_content, sizeof *(elf->sections_content) * elf->elf_header->e_shnum + elf->a_shdr[elf->elf_header->e_shnum - 1].sh_size);
	memcpy(&(elf->sections_content[elf->elf_header->e_shnum - 1]), &(elf2->sections_content[num]), elf->a_shdr[elf->elf_header->e_shnum - 1].sh_size);
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Concatène le contenue des sections
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void seccat(char * s1, char * s2, char * sf, int size1, int size2) {
	int i = 0;
	int j;
	sf[0] = '\0';
	
	while (i < size1) {
		sf[i] = s1[i];
		i++;
	}
	j = i;
	i = 0;
	while (i < size2) {
		sf[j] = s2[i];
		i++;
		j++;
	}
}
