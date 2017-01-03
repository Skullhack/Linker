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
#include <stdlib.h>
#include <stdarg.h>
#include "elf_struct.h"
#include "elf_gestion.h"

int is_big_endian() {
    static uint32_t one = 1;
    return ((* (uint8_t *) &one) == 0);
}


int this_needs_reverse(unsigned char ei_data) {
		
	if (ei_data == ELFDATA2LSB)
		return (ei_data == ELFDATA2LSB && is_big_endian());
	else 
		return (ei_data == ELFDATA2MSB && !is_big_endian());
	
}

int is_number(char *number) {

	int i = 0;

	if (number[0] == '-') {
		i++;
	}

	for (; number[i] != 0; i++) {
		if (!isdigit(number[i])) return 0;
	}

	return 1;

}

int fclose_free_everything(FILE *f, int count, ...) {

	va_list list;
	int i = 0;

	va_start(list, count);

	for (i = 0; i < count; i++) {
		free(va_arg(list, void *));
	}

	va_end(list);

	fclose(f);

	return count;

}

void print_usage(char* program_name) {

	fprintf(stderr, "Usage: %s <options> file_name\n", program_name);
	fprintf(stderr, 
			"  Options are:\n "
			"\t-H --help \t\tPrints this message.\n"
			"\t-a --all \t\tDisplays everything below.\n"
			"\t-h --file_header\tDisplays the information contained in the ELF header at the start of the file.\n"
			"\t-S --sections\t\tDisplays the information contained in the file's section headers, if it has any.\n"
			"\t-x --hex-dump\t\tDump the contents of section <number|name> as bytes.\n"
			"\t-s --symbols\t\tDisplays the entries in symbol table section of the file, if it has one.\n"
			"\t-r --relocs\t\tDisplays the contents of the file's relocation section, if it has one.\n");

}

int error_elf_struct(ELF_STRUCT* elf_struct, char *message) {
	fprintf(stderr, "%s\n", message);
	return EXIT_FAILURE;
}