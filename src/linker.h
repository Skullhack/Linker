#ifndef __LINKER_H__
#define __LINKER_H__

#include "global_struct.h"

/**
	*BRIEF : Renvoi le nombre ecrit dans la console
	*PARAM : prompt:
						Message affiché avant la récupération du nombre
					 i
						pointeur ou stocker la réponse
	*RETURN: Entier: 0->OK 1-> ERREUR
**/
int GetInteger(const char *prompt, int *i);

/**
	*BRIEF : Lance l'affichage en mode menu (surtout pour le debug)
	*PARAM : elf_struct1
						Structure à afficher
					 elf_struct2
						Structure à afficher si deux fichier sont chargés
	*RETURN: Void. Affichage seulement
**/
void menu(ELF_STRUCT * elf_struct1, ELF_STRUCT * elf_struct2); 

/**
	*BRIEF : Lance l'affichage complet (utilisé seulement dans le mode menu)
	*PARAM : elf_struct1
						Structure à afficher
					 elf_struct2
						Structure à afficher si deux fichier sont chargés
	*RETURN: Void. Affichage seulement
**/
void affichageComplet(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);

/**
	*BRIEF : Lance l'affichage du header
	*PARAM : elf_struct1
						Structure à afficher
					 elf_struct2
						Structure à afficher si deux fichier sont chargés
	*RETURN: Void. Affichage seulement
**/
void affichageHeader(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);

/**
	*BRIEF : Lance l'affichage du section header
	*PARAM : elf_struct1
						Structure à afficher
					 elf_struct2
						Structure à afficher si deux fichier sont chargés
	*RETURN: Void. Affichage seulement
**/
void affichageSectionHeader(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);

/**
	*BRIEF : Lance l'affichage du contenu d'une section
	*PARAM : elf_struct1
						Structure à afficher
					 elf_struct2
						Structure à afficher si deux fichier sont chargés
					 nSec
						Chaine de caractère contenant soit un numéro soit un nom.
	*RETURN: Void. Affichage seulement
**/
void affichageContenuSection(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2, char* nSec);

/**
	*BRIEF : Lance l'affichage de la table des symboles
	*PARAM : elf_struct1
						Structure à afficher
					 elf_struct2
						Structure à afficher si deux fichier sont chargés
	*RETURN: Void. Affichage seulement
**/
void affichageSymbole(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);

/**
	*BRIEF : Lance l'affichage de la table de réimplantation
	*PARAM : elf_struct1
						Structure à afficher
					 elf_struct2
						Structure à afficher si deux fichier sont chargés
	*RETURN: Void. Affichage seulement
**/
void affichageReimplantation(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);

/**
	*BRIEF : Lance la fusion des deux fichiers en paramètre
	*PARAM : elf_struct1
						Structure à fusionner
					 elf_struct2
						Structure à fusionner
	*RETURN: Void. Affichage seulement
**/
void lancer_fusion(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2);

/**
	*BRIEF : Initialise la structure en paramètre grâce au nom de fichier donné.
	*PARAM : nomFichier
						chaine de caractère représentant un nom de fichier
					 elf_struct
						Structure à initialiser
	*RETURN: Entier: 1->OK -1-> ERREUR
**/
ELF_STRUCT* Init_f_elfstruct(char* nomFichier, ELF_STRUCT* elf_struct);

/**
	*BRIEF : Affiche l'aide. Toutes les options disponible
	*PARAM : name
						nom de la commande exécuté
	*RETURN: Void. Afichage simple.
**/
void usage(char* name);

#endif
