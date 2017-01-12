#include "util.h"
#include "section_elf.h"
#include <stdint.h>
#include <string.h>

/*
Demande le numéro ou le nom de la section à afficher avant de lancer l'affichage
*/
int choix_section(ELF_STRUCT * elf_struct, char* nSec) {
	/*printf("Quelle section afficher ?\n");
	
	char sec[100]; */
	int nbSection = elf_struct->elf_header->e_shnum;

	/*printf("Saisir le numéro (la recherche par nom n'est pas implanté pour le moment...) : ");
	fgets(sec, sizeof(sec),stdin);*/

	//if (sec[0]=='.') { //a terminer, si le nom est saisie.
	if (nSec[0]=='.') {
		//char secName[strlen(sec)-1];
		//memcpy(secName,sec,strlen(sec)-1);
		
		int nbAffiche=0;

		int nbSection = elf_struct->elf_header->e_shnum;
		char* nametmp;

		for (int i=0; i<nbSection ; i++ ) {
			
			nametmp = get_name(elf_struct,i);
			if ( strcmp(nSec,nametmp)==0) {
				display_section(elf_struct,i);
				nbAffiche++;
			}
		}
		if (nbAffiche==0) {
			printf("La section %s n'existe pas.\n",nSec);
		}
	} else if (*nSec < nbSection) {
		display_section(elf_struct,*nSec);
	} else if (nSec[0]!='0' && atoi(nSec)==0) {
			printf("Le nom saisie n'est pas valide\n");
			return -1;
	} else {
		if (atoi(nSec)>=nbSection || atoi(nSec)<0) {
			printf("La section %d n'existe pas.\n",atoi(nSec));
			return -1;
		}
		display_section(elf_struct,atoi(nSec));
	}
	return 0;
}

/*
Affiche la section indiqué en paramètre par le "id". Il peut etre soit un nombre soit un nom de section
*/
int display_section(ELF_STRUCT* elf_struct, int id) {

		uint32_t size = (uint32_t) elf_struct->a_shdr[id].sh_size; //taille de la section
		unsigned char dbyte; //mot lu, correspond à 2 bytes
		int cptB=-1; //compteur de dByte, il permet de revenir à la ligne et de changer l'adressage
		unsigned int cptAdrs=0; //l'adresse des mots

		printf("╔═══════════════════════════════════════════════════════════════╗\n");
    	printf("║                       \e[1;31mELF Section n°:%d\e[0m                        ║\n",id);
    	printf("╠═══════════════════════════════════════════════════════════════╣\n");
		

		if (size != 0){
			for (int i=0; i<size; i++) {//parcours la section
				dbyte = elf_struct->sections_content[id][i]; //récupère le ième mot de la section ID
				cptB++;

				if (cptB%16 == 0) { //Si on a finis la ligne, on reviens a la ligne et affiche l'adresse
					printf("║0x%.8x\t", cptAdrs); // sous la forme 0x00000000
					cptAdrs= cptAdrs+16; //inc l'adresse de 16 pour afficher les 8prochains dBytes
				}
				printf("%.2x ",dbyte); //Affiche le dByte
				if (cptB%16 == 15){
					printf("║\n");
				}
			}
			if (cptB%16 != 15){
				for (int i=cptB%16 + 1 ; i<15; i++){
					printf("   ");
				}
				printf("   ║\n");
			}
		}else{ 
			printf("║%-63s║\n","Aucun contenu");
		}
		printf("╚═══════════════════════════════════════════════════════════════╝\n");
	return 1;

}

int read_section(Elf32_Shdr* monShdr, ELF_STRUCT* elf_struct, char* section_content) {

	Elf32_Off offset = monShdr->sh_offset; //debut de la section
	uint32_t size = (uint32_t) monShdr->sh_size; //taille de la section
	fseek(elf_struct->elf_file, offset, SEEK_SET); //on deplace la tete sur le debut

	if ( fread(section_content, size, 1, elf_struct->elf_file) == -1) { //on read et la place dans le section_content passé en paramètre
		return -1;
	}

	if (elf_struct->elf_header->e_ident[EI_DATA] == 2) { //on doit tout permuter le tableau

		int end=size-1;
		char temp;
		for (int i=0;i<(size)/2; i++) {
			temp = section_content[i];
			section_content[i] = section_content[end];
			section_content[end] = temp;
			end--;
		}
	}

	return 1;
}
