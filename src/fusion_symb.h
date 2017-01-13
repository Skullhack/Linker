#ifndef _fusion_symb_
#define _fusion_symb_

#include "section_header.h"
#include "util.h"
#include "global_struct.h"

typedef struct cel {
	Elf32_Sym sym;
	char * name;
} cel;

typedef struct noeud {
	cel donnee;
	struct noeud * suiv;
}noeud;

typedef struct noeud * liste; 

void fusion_symb(ELF_STRUCT * elf,ELF_STRUCT * elf1);
void Ajouter(ELF_STRUCT * elf,int i, liste *p) ;
void Supprimer(liste * lis,int i);
void Ajout(Elf32_Sym element,cel * res,int taille);
int globaux(Elf32_Sym tab);
int def(Elf32_Sym tab);
int ndef(Elf32_Sym tab);
int taille_tab_symb(ELF_STRUCT * elf);
char * recup_name(ELF_STRUCT * elf,uint32_t name);

void copie_str(char * str_name, char * str, int debut,int taille_mot );

void afficher_table_sym(ELF_STRUCT * elf);
char* afficher_type_s(unsigned char st_info);
char* afficher_bind_s(unsigned char st_info);
char* afficher_vis_s(unsigned char st_other);
void afficher_ndx_s(Elf32_Section st_shndx);

#endif
