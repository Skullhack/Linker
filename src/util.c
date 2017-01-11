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
#include "util.h"
#include <stdint.h>

int is_big_endian() {
	static uint32_t one = 1;
	return ((* (uint8_t *) &one) == 0);
}

int need_reverse(unsigned char ei_data) {
	if (ei_data == ELFDATA2LSB)
		return (ei_data == ELFDATA2LSB && is_big_endian());
	else 
		return (ei_data == ELFDATA2MSB && !is_big_endian());
}

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
	
	str = malloc(sizeof(char)*taille_mot);
	for (i = offset; i < taille_mot+offset; i++) {
		str[j] = shstr[i];
		j++;
	}
	
	return str;
}

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

int max_offset_section(ELF_STRUCT * elf){
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
