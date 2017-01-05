#ifndef _section_
#define _section_

void Affichage_section(ELF_STRUCT * elf );
char* case_flags(Elf32_Word flag);
char* case_type(Elf32_Word sh_type);
char* get_name(ELF_STRUCT * elf,Elf32_Word name ,int numero);
int header_section(ELF_STRUCT * elf );

#endif

