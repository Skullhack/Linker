#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stddef.h>

#include "global_struct.h"
#include "init.h"
#include "header_elf.h"
#include "linker.h"
#include "section_header.h"
#include "util.h"

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


void affichageComplet(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	//à compléter	
	if (elf_struct2 == NULL) {
		display_header(elf_struct1->elf_header);
		Affichage_section(elf_struct1);
		printf("Suite en cours");
	} else {
		int choix = 0;
		while (choix != 1 && choix != 2) {
			GetInteger("Quel fichier ? (numéro)\n", &choix);
			if (choix == 1) {
				display_header(elf_struct1->elf_header);
				Affichage_section(elf_struct1);
				printf("Suite en cours");
			} else {
				display_header(elf_struct2->elf_header);
				Affichage_section(elf_struct2);
				printf("Suite en cours");
			}
		}
	}
}

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

void affichageContenuSection(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	//CHOIX DE LA SECTION A AFFICHER
	//affichage contenu de section
	printf("En cours");
}

void affichageSymbole(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	//affichage table symbole
	printf("En cours");
}

void affichageReimplementation(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2t) {
	//affichage de la table de réimplémentation
	printf("En cours");
}

void fusion(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	if (elf_struct2 == NULL) {
		printf("Un seul fichier en argument, fusion impossible.\n");
	} else {
	//FUSION (petit dessin pour la soutenance) YOLOLOLO
	printf("FUUUUUU-SION");
	}
}

int main(int argc, char *argv[]) {

	int deuxF = 0;
	int choix = 0;
	FILE *f1 = NULL;
	FILE *f2 = NULL;
	ELF_STRUCT* elf_struct1 = NULL;
	ELF_STRUCT* elf_struct2 = NULL;

	// Checks for argc
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "1 ou 2 arguments autorisés (noms des fichiers objet)\n");
		return EXIT_FAILURE;
	}

	if (argc == 3)
		deuxF = 1;

	if (deuxF) {
		f2 = fopen(argv[2], "r");
		if (f2 == NULL) {
			fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s en mode lecture\n", argv[2]);
			return EXIT_FAILURE;
		}

		elf_struct2 = malloc( sizeof(ELF_STRUCT) );
		if (elf_struct2 == NULL) {
			fprintf(stderr, "Erreur allocation elf_struct\n");
			return EXIT_FAILURE;
		}

		if ( !init_elf_struct(elf_struct2, f2) ) {
			fprintf(stderr, "Erreur d'initialisation : %s", get_error(elf_struct2));
			return EXIT_FAILURE;
		}
	}

	f1 = fopen(argv[1], "r");
	if (f1 == NULL) {
		fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s en mode lecture\n", argv[1]);
		return EXIT_FAILURE;
	}

	elf_struct1 = malloc( sizeof(ELF_STRUCT) );
	if (elf_struct1 == NULL) {
		fprintf(stderr, "Erreur allocation elf_struct\n");
		return EXIT_FAILURE;
	}

	if ( !init_elf_struct(elf_struct1, f1) ) {
		fprintf(stderr, "Erreur d'initialisation : %s", get_error(elf_struct1));
		return EXIT_FAILURE;
	}
	
	choix = 0;
	while (choix != 8) {
		printf("\nMenu principal\n");
		printf("--------------------------------------------------\n");
		printf("1 = affichage complet des informations\n");
		printf("2 = afficher le header\n");
		printf("3 = afficher les headers de section\n");
		printf("4 = afficher le contenu d'une section\n");
		printf("5 = afficher la table des symboles\n");
		printf("6 = afficher la table de réimplémentation\n");
		printf("7 = effectuer la fusion des fichiers objets donnés\n");
		printf("8 = quitter le programme\n\n");
		GetInteger("Entrez un chiffre proposé : \n", &choix);
		switch(choix) {
			case 1:
				affichageComplet(elf_struct1, elf_struct2);
			break;
			case 2:
				affichageHeader(elf_struct1, elf_struct2);
			break;
			case 3:
				affichageSectionHeader(elf_struct1, elf_struct2);
			break;
			case 4:
				affichageContenuSection(elf_struct1, elf_struct2);
			break;
			case 5:
				affichageSymbole(elf_struct1, elf_struct2);
			break;
			case 6:
				affichageReimplementation(elf_struct1, elf_struct2);
			break;
			case 7:
				fusion(elf_struct1, elf_struct2);
			break;
			case 8:
				printf("Fermeture du programme\n");
			break;
			default:
				printf("Entrez un chiffre de 1 à 8.\n\n");
			break;
		}
	}

	// Sortie propre
	close_elf_struct(elf_struct1);
	if (deuxF)
		close_elf_struct(elf_struct2);

	return EXIT_SUCCESS;

}
