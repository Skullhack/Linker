#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <getopt.h>

#include "global_struct.h"
#include "init.h"
#include "header_elf.h"
#include "linker.h"
#include "section_header.h"
#include "table_symbole.h"
#include "reimplantation.h"
#include "section_elf.h"
#include "fusion.h"
#include "util.h"
#include "fusion_symb.h"



int main(int argc, char *argv[]) {
	ELF_STRUCT* elf_struct1 = NULL;
	ELF_STRUCT* elf_struct2 = NULL;

	if (argc < 3) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	/*
	Gestion des options
	*/
	int opt; //Contien le caractère mis en option (apres le -)
	/*
	Variable servant a retenir quelles options sont entrées.
	*/
	int affh=0;
	int affS=0;
	char* affx=NULL; //Contiendra le numéro eventuel de la section_content à afficher
	int affs=0;
	int affr=0;
	int fusion=0;
	int affM=0;

	struct option longopts[] = {
		{ "ALL", required_argument, NULL, 'A' },
		{ "all", required_argument, NULL, 'a' },
		{ "menu", no_argument, NULL, 'm' },
		{ "header", no_argument, NULL, 'h' },
		{ "shdr", no_argument, NULL, 'S' },
		{ "section", required_argument, NULL, 'x' },
		{ "symbole", no_argument, NULL, 's' },
		{ "reimp", no_argument, NULL, 'r' },
		{ "fusion", no_argument, NULL, 'f' },
		{ "help", no_argument, NULL, '?' },
		{ NULL, 0, NULL, 0 }
	};

	while ( (opt = getopt_long(argc,argv,"aA:hmSx:srf?", longopts, NULL)) !=-1) {
		switch(opt) {
		//Erreur si on tape ./linker -A [fichier...] il essaye de lire le fichier -A
		case 'a':
			affh=1;
			affS=1;
			affs=1;
			affr=1;
			break;
		case 'A':
			affh=1;
			affS=1;
			affx=optarg;
			affs=1;
			affr=1;
			break;
		case 'h':
			affh=1;
			break;
		case 'S':
			affS=1;
			break;
		case 'x':
			affx=optarg;
			break;
		case 's':
			affs=1;
			break;
		case 'r':
			affr=1;
			break;
		case 'f':
			fusion=1;
			break;
		case 'm':
			affM = 1;
			break;

		case '?':
			usage(argv[0]);
			break;
		default:
			fprintf(stderr, "Unrecognized option %c\n", opt);
			usage(argv[0]);
			exit(1);
		}
	}
	/*
	Option retenu, maintenant on traite.
	*/

	/*Temp, utile que pour l'affichage alors qu'on peut surement l'utiliser tout le temps*/
	//On boucle sur tous les fichiers entrés
	for (int i=optind;i<argc;i++) {
		elf_struct1 = Init_f_elfstruct(argv[i],elf_struct1);
		if (elf_struct1!=NULL) {
			if (affh) {
				affichageHeader(elf_struct1,elf_struct2);
			}
			if (affS) {
				affichageSectionHeader(elf_struct1,elf_struct2);
			}
			if (affx!=NULL) {
				affichageContenuSection(elf_struct1,elf_struct2,affx);
			}
			if (affs) {
				affichageSymbole(elf_struct1,elf_struct2);
			}
			if (affr) {
				affichageReimplantation(elf_struct1,elf_struct2);
			}
		}
		/*Temp, utilisé que pour la fusion de DEUX fichiers*/
		if (fusion) {
				elf_struct2 = Init_f_elfstruct(argv[optind+1],elf_struct2);
				lancer_fusion(elf_struct1, elf_struct2);
		}
		
		if (affM) {
				elf_struct2 = Init_f_elfstruct(argv[optind+1],elf_struct2);
				menu(elf_struct1, elf_struct2);
		}
	}
	
	// Sortie propre
	if (elf_struct2 != NULL)
		close_elf_struct(elf_struct2);
	close_elf_struct(elf_struct1);

	return EXIT_SUCCESS;

}


/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetInteger(const char *prompt, int *i) {
	int Invalid = 0;
	int EndIndex;
	char buffer[100];
	do {
		if (Invalid)
			fputs("Entrez un chiffre de 1 à 8.\n\n", stdout);
		Invalid = 1;
		fputs(prompt, stdout);
		if (NULL == fgets(buffer, sizeof(buffer), stdin))
			return 1;
		errno = 0;
	} while ((1 != sscanf(buffer, "%d %n", i, &EndIndex)) || buffer[EndIndex] || errno);
	return 0;
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu(ELF_STRUCT * elf_struct1, ELF_STRUCT * elf_struct2) {
	int choix = 0;
	int sec = 0;
	while (choix != 8) {
		printf("\nMenu principal\n");
		printf("--------------------------------------------------\n");
		printf("1 = afficher le header\n");
		printf("2 = afficher les headers de section\n");
		printf("3 = afficher le contenu d'une section\n");
		printf("4 = afficher la table des symboles\n");
		printf("5 = afficher la table de réimplantation\n");
		printf("6 = affichage complet des informations\n");
		printf("7 = effectuer la fusion des fichiers objets donnés\n");
		printf("8 = quitter le programme\n\n");
		GetInteger("Entrez un chiffre proposé : \n", &choix);
		switch(choix) {
			case 1:
				affichageHeader(elf_struct1, elf_struct2);
			break;
			case 2:
				affichageSectionHeader(elf_struct1, elf_struct2);
			break;
			case 3:
				sec = 0;
				GetInteger("Quel section ? (numéro)\n", &sec);
				char s = sec;
				affichageContenuSection(elf_struct1, elf_struct2, &s);
			break;
			case 4:
				affichageSymbole(elf_struct1, elf_struct2);
			break;
			case 5:
				affichageReimplantation(elf_struct1, elf_struct2);
			break;
			case 6:
				affichageComplet(elf_struct1, elf_struct2);
			break;
			case 7:
				lancer_fusion(elf_struct1, elf_struct2);
				display_header(elf_struct1->elf_header);
				Affichage_section(elf_struct1);
				afficher_table_sym(elf_struct1);
				Affichage_Rel(elf_struct1);
				Affichage_Rela(elf_struct1);
			break;
			case 8:
				printf("Fermeture du programme\n");
				exit(1);
			break;
			default:
				printf("Entrez un chiffre de 1 à 8.\n\n");
			break;
		}
	}
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void affichageComplet(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	if (elf_struct2 == NULL) {
		display_header(elf_struct1->elf_header);
		Affichage_section(elf_struct1);
		afficher_table(elf_struct1);
		Affichage_Rel(elf_struct1);
		Affichage_Rela(elf_struct1);
	} else {
		int choix = 0;
		while (choix != 1 && choix != 2) {
			GetInteger("Quel fichier ? (numéro)\n", &choix);
			if (choix == 1) {
				display_header(elf_struct1->elf_header);
				Affichage_section(elf_struct1);
				afficher_table(elf_struct1);
				Affichage_Rel(elf_struct1);
				Affichage_Rela(elf_struct1);
			} else {
				display_header(elf_struct2->elf_header);
				Affichage_section(elf_struct2);
				afficher_table(elf_struct2);
				Affichage_Rel(elf_struct2);
				Affichage_Rela(elf_struct2);
			}
		}
	}
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void affichageHeader(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	if (elf_struct2 == NULL) {
		display_header(elf_struct1->elf_header);
	} else {
		int choix = 0;
		while (choix != 1 && choix != 2) {
			GetInteger("Quel fichier ? (numéro)\n", &choix);
			if (choix == 1) {
				display_header(elf_struct1->elf_header);
			} else {
				display_header(elf_struct2->elf_header);
			}
		}
	}
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void affichageSectionHeader(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	if (elf_struct2 == NULL) {
		Affichage_section(elf_struct1);
	} else {
		int choix = 0;
		while (choix != 1 && choix != 2) {
			GetInteger("Quel fichier ? (numéro)\n", &choix);
			if (choix == 1) {
				Affichage_section(elf_struct1);
			} else {
				Affichage_section(elf_struct2);
			}
		}
	}
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void affichageContenuSection(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2, char* nSec) {
	if (elf_struct2 == NULL) {
		choix_section(elf_struct1,nSec);
	} else {
		int choix = 0;
		while (choix != 1 && choix != 2) {
			GetInteger("Quel fichier ? (numéro)\n", &choix);
			if (choix == 1) {
				choix_section(elf_struct1,nSec);
			} else {
				choix_section(elf_struct2,nSec);
			}
		}
	}
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void affichageSymbole(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	if (elf_struct2 == NULL) {
		afficher_table(elf_struct1);
	} else {
		int choix = 0;
		while (choix != 1 && choix != 2) {
			GetInteger("Quel fichier ? (numéro)\n", &choix);
			if (choix == 1) {
				afficher_table(elf_struct1);
			} else {
				afficher_table(elf_struct2);
			}
		}
	}
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void affichageReimplantation(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	if (elf_struct2 == NULL) {
		Affichage_Rel(elf_struct1);
		Affichage_Rela(elf_struct1);
	} else {
		int choix = 0;
		while (choix != 1 && choix != 2) {
			GetInteger("Quel fichier ? (numéro)\n", &choix);
			if (choix == 1) {
				Affichage_Rel(elf_struct1);
				Affichage_Rela(elf_struct1);
			} else {
				Affichage_Rel(elf_struct2);
				Affichage_Rela(elf_struct2);
			}
		}
	}
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void lancer_fusion(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	if (elf_struct2 == NULL) {
		printf("Un seul fichier en argument, fusion impossible.\n");
	} else {
		Fusion(elf_struct1, elf_struct2);
		printf("Fusion terminée.\n");
	}
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ELF_STRUCT* Init_f_elfstruct(char* nomFichier, ELF_STRUCT* elf_struct) {
	FILE *f = fopen(nomFichier, "r");
	if (f == NULL) {
		fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s en mode lecture\n", nomFichier);
		return NULL;
	} 

	elf_struct = malloc( sizeof(ELF_STRUCT) );
	if (elf_struct == NULL) {
		fprintf(stderr, "Erreur allocation elf_struct\n");
		return NULL;
	} 
	if ( !init_elf_struct(elf_struct, f) ) {
		fprintf(stderr, "Erreur d'initialisation : %s", get_error(elf_struct));
		return NULL;
	}
	
	return elf_struct;
}


/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Renvoi le nombre ecrit dans la console
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void usage(char* name) {
		printf("%s [OPTION...] [FILE...]\n",name);
		printf("OPTIONS\n");
		printf("  -a, --all\n");
		printf("    Affiche les différentes parties des fichiers\n");
		printf("  -A, --ALL=N\n");
		printf("    Affiche les différentes parties des fichiers ainsi que la section de nom ou de numéro N\n");

		printf("\n  -h, --header\n");
		printf("    Affiche le header des fichiers\n");
		printf("  -S, --shdr\n");
		printf("    Affiche le section header des fichiers\n");
		printf("  -x, --section=N\n");
		printf("    Affiche la section de nom ou de numéro N des fichiers\n");
		printf("  -s, --symbole\n");
		printf("    Affiche la table des symboles des fichiers\n");
		printf("  -r, --reimp\n");
		printf("    Affiche la table de réimplantation des fichiers\n");
		printf("  -m, --menu\n");
		printf("    Lance le programme en mode menu\n");

		printf("\n  -?, --help\n");
		printf("    Affiche l'aide\n");
}
