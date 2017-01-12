#ifndef _table_symbole_
#define _table_symbole_

/**
	*BRIEF : Charge en mémoire la partie de la table des symbole dans la structure.
	*PARAM : elf
						Structure à charger (elf_struct->file est le fichier à lire).
	*RETURN: Entier: 1->OK -1-> ERREUR.
**/
int creer_table_symbole(ELF_STRUCT * elf);

/**
	*BRIEF : Affiche la table des symbole de la structure passée en paramètre.
	*PARAM : elf
						structure à afficher.
	*RETURN: Void (affiche simplement)
**/
void afficher_table(ELF_STRUCT * elf);


/**
	*BRIEF : Traduit une variable de type de symbole en texte.
	*PARAM : st_info
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* case_typeSym(unsigned char st_info);


/**
	*BRIEF : Traduit une variable info en texte.
	*PARAM : st_info
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* case_bind(unsigned char st_info);


/**
	*BRIEF :  Traduit une variable de visibilité en texte.
	*PARAM : st_other
						Variable à traduire
	*RETURN: String contenant le texte
**/
char* case_vis(unsigned char st_other);

/**
	*BRIEF : Affiche la traduction d'un ndx en texte.
	*PARAM : st_shndx
						Variable à traduire
	*RETURN: void, affichage simple
**/
void afficher_ndx(Elf32_Section st_shndx);

#endif
