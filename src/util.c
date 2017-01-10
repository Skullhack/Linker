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

	int offset = (int) elf->a_shdr[elf->elf_header->e_shstrndx].sh_offset;
	int nom = (int)  elf->a_shdr[numero].sh_name;

	fseek(elf->elf_file,offset+nom,SEEK_SET);

	char c;
	int cpt = 1;
	while ( (c = fgetc(elf->elf_file)) != '\0') {
		cpt++;
	}

	fseek(elf->elf_file,offset+nom,SEEK_SET);

	char* str = malloc(sizeof(char)*cpt);
	
	fgets(str,cpt,elf->elf_file);

	//printf("Section : %d, Offset : %d, Index dans str : %d, Nom : %s\n",numero, offset, nom, str);
	
	return str;
}
