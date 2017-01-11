#ifndef __FUSION_REIMP_H__
#define __FUSION_REIMP_H__

//	Renvoie l'indice de la section d'offset maximum
int max_offset_section(ELF_STRUCT * elf);

//	Programme principale de fusion des tables de réimplantation de 2 fichiers objets
void fusion_reimp(ELF_STRUCT * elf_file1, ELF_STRUCT * elf_file2);

#endif
