#ifndef __FUSION_REIMP_H__
#define __FUSION_REIMP_H__

/*	Décale l'offset de toutes les sections qui se trouvent après l'offset de la 
	section d'indice passé en paramètre, d'une taille "size"	*/
void offset_section_update(int ind_sect, Elf32_Word size, ELF_STRUCT * elf);

//	Renvoie l'indice de la section d'offset maximum
int max_offset_section(ELF_STRUCT * elf);

//	Programme principale de fusion des tables de réimplantation de 2 fichiers objets
void fusion_reimp(ELF_STRUCT * elf_file1, ELF_STRUCT * elf_file2);

#endif
