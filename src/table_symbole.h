#ifndef _table_symbole_
#define _table_symbole_

int creer_table_symbole(ELF_STRUCT * elf);
void afficher_table(ELF_STRUCT * elf);
void afficher_type(unsigned char st_info);
void afficher_bind(unsigned char st_info);

#endif
