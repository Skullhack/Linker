#ifndef _table_symbole_
#define _table_symbole_

void afficher_type(unsigned char st_info);
void afficher_bind(unsigned char st_info);
void afficher_vis(unsigned char st_other);
void afficher_ndx(Elf32_Section st_shndx);
int creer_table_symbole(ELF_STRUCT * elf);
void afficher_table(ELF_STRUCT * elf);

#endif
