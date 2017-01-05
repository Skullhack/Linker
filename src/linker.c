#include <stdlib.h>
#include <stdio.h>

#include "global_struct.h"
#include "init.h"
#include "header_elf.h"
#include "linker.h"
#include "section_header.h"
#include "util.h"


void affichageComplet(ELF_STRUCT* elf_struct) {
	display_header(elf_struct->elf_header);
	Affichage_section(elf_struct);
	printf("Suite en cours");
	//affichage de la suite
}

void affichageHeader(ELF_STRUCT* elf_struct) {
	display_header(elf_struct->elf_header);
}

void affichageSectionHeader(ELF_STRUCT* elf_struct) {
	Affichage_section(elf_struct);
}

void affichageContenuSection(ELF_STRUCT* elf_struct) {
	//CHOIX DE LA SECTION A AFFICHER
	//affichage contenu de section
	printf("En cours");
}

void affichageSymbole(ELF_STRUCT* elf_struct) {
	//affichage table symbole
	printf("En cours");
}

void affichageReimplementation(ELF_STRUCT* elf_struct) {
	//affichage de la table de réimplémentation
	printf("En cours");
}

void fusion(ELF_STRUCT* elf_struct1, ELF_STRUCT* elf_struct2) {
	//FUSION
	printf("FUUUUUU-SION");
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
		scanf("%d",&choix);
		switch(choix) {
			case 1:
				choix = 0;
				while (choix != 1 && choix != 2) {
					printf("\nQuel fichier ?\n");
					scanf("%d",&choix);
					if (choix == 1)
						affichageComplet(elf_struct1);
					else
						affichageComplet(elf_struct2);

				}
			break;
			case 2:
				choix = 0;
				while (choix != 1 && choix != 2) {
					printf("\nQuel fichier ?\n");
					scanf("%d",&choix);
					if (choix == 1)
						affichageHeader(elf_struct1);
					else
						affichageHeader(elf_struct2);
				}
			break;
			case 3:
				choix = 0;
				while (choix != 1 && choix != 2) {
					printf("\nQuel fichier ?\n");
					scanf("%d",&choix);
					if (choix == 1)
						affichageSectionHeader(elf_struct1);
					else
						affichageSectionHeader(elf_struct2);
				}
			break;
			case 4:
				choix = 0;
				while (choix != 1 && choix != 2) {
					printf("\nQuel fichier ?\n");
					scanf("%d",&choix);
					if (choix == 1)
						affichageContenuSection(elf_struct1);
					else
						affichageContenuSection(elf_struct2);
				}
			break;
			case 5:
				choix = 0;
				while (choix != 1 && choix != 2) {
					printf("\nQuel fichier ?\n");
					scanf("%d",&choix);
					if (choix == 1)
						affichageSymbole(elf_struct1);
					else
						affichageSymbole(elf_struct2);
				}
			break;
			case 6:
				choix = 0;
				while (choix != 1 && choix != 2) {
					printf("\nQuel fichier ?\n");
					scanf("%d",&choix);
					if (choix == 1)
						affichageReimplementation(elf_struct1);
					else
						affichageReimplementation(elf_struct2);
				}
			break;
			case 7:
				if (deuxF == 0)
					printf("Un seul fichier en argument, fusion impossible.\n");
				else
					fusion(elf_struct1, elf_struct2);
			break;
		}
	}

	// Sortie propre
	close_elf_struct(elf_struct1);
	close_elf_struct(elf_struct2);

	return EXIT_SUCCESS;

}
