#include <elf.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

/*	!!! ATTENTION !!! : DANS LA STRUCTURE, LES SECTIONS (**sections_content) SONT DANS 
	LE MEME ORDRE QUE LES ENTETES DE SECTION (c'est plus simple pour programmer et on 
	réécrira grâce aux offsets dans le fichier final)	*/

/*	Très utile pour débugger, permet de voir le contenu d'une section 
	(ouvrir le fichier avec l'éditeur de texte online https://hexed.it/ )	*/
/*FILE * f;			
f = fopen("shstrtab_avant.txt", "w");			
for(j = 0; j < elf_file1->a_shdr[elf_file1->elf_header->e_shstrndx].sh_size; j++){
	fprintf(f, "%c", elf_file1->sections_content[elf_file1->elf_header->e_shstrndx][j]);			
}			
fclose(f);*/


//	Programme principale de fusion des tables de réimplantation de 2 fichiers objets
void fusion_reimp(ELF_STRUCT * elf_file1, ELF_STRUCT * elf_file2){
	bool missing[elf_file2->elf_header->e_shnum];
	int i = 0, j = 0, ind_sect_max = 0, add_off_sect = 0;
	char *section_f1, *section_f2, *section_fus;
	
	/*	Initialisation du tableau de booléens des sections du fichier 2 afin de 
		marquer celles de type SHT_REL qui ne seront pas fusionnées dans le fichier 1	*/
	for(i = 0 ; i < elf_file2->elf_header->e_shnum; i++){
	
		if(elf_file2->a_shdr[i].sh_type == SHT_REL){
			missing[i] = true;
		}
		else{
			missing[i] = false;
		}
	
	}
	
	// 	-------------------------------PARTIE 1 - FUSION--------------------------------

	/*	Recherche des sections de même type SHT_REL et de même nom dans les 
		fichiers 2 et 1 afin de les fusionner dans le fichier 1	*/
	for(i = 0; i < elf_file1->elf_header->e_shnum; i++){
		
		if(elf_file1->a_shdr[i].sh_type == SHT_REL){
			
			for(j = 0; j < elf_file2->elf_header->e_shnum; j++){
				
				if(elf_file2->a_shdr[j].sh_type == SHT_REL && strcmp(get_name(elf_file1, i), get_name(elf_file2, j)) == 0 ){
					
					// 	Dans le tableau de booléens, passage à FALSE des sections fusionnées
					missing[j] = false;
					
					/*	Récupération des 2 sections à fusionner + déclaration et définition 
						de la variable qui va accueillir la fusion des 2 sections	*/
					section_f1 = malloc(sizeof(char)*elf_file1->a_shdr[i].sh_size);
					section_f1 = elf_file1->sections_content[i];
					
					section_f2 = malloc(sizeof(char)*elf_file2->a_shdr[j].sh_size);
					section_f2 = elf_file2->sections_content[j];
					
					section_fus = malloc(sizeof(char)*(elf_file1->a_shdr[i].sh_size + elf_file2->a_shdr[j].sh_size));
					
					/*	Concaténation de la section du fichier 2 à la fin de la section 
						équivalente dans elf_file1->sections_content[i]	*/
					elf_file1->sections_content = realloc(elf_file1->sections_content, sizeof *(elf_file1->sections_content) * elf_file1->elf_header->e_shnum + elf_file2->a_shdr[j].sh_size);
					seccat(section_f1, section_f2, section_fus, elf_file1->a_shdr[i].sh_size, elf_file2->a_shdr[j].sh_size);					
					elf_file1->sections_content[i] = section_fus;
					
					/*	Modification du size de la section du fichier 1 en rajoutant le 
						size de la section du fichier 2	*/
					elf_file1->a_shdr[i].sh_size += elf_file2->a_shdr[j].sh_size;
			
					/*	Décallage du offset de la première entête de section qui se
					 	trouve dans le header	*/					
					elf_file1->elf_header->e_shoff += elf_file2->a_shdr[j].sh_size;
					
					/*	Appel de offset_section_update() qui modifie l'offset de toutes
						les sections qui se trouvent après l'offset de la section actuelle	*/
					maj_offset(elf_file1, i, elf_file2->a_shdr[j].sh_size);
					
				}
			}
		}
	}
	
	// 	-------------------------------PARTIE 2 - CONCATENATION--------------------------------
	
	//	Parcours du tableau de booléens et ajout des sections marquées à TRUE
	for(i = 0 ; i < elf_file2->elf_header->e_shnum; i++){
	
		if(elf_file2->a_shdr[i].sh_type == SHT_REL && missing[i] == true){
		
			// 	Dans le tableau de booléens, passage à FALSE des sections ajoutées
			missing[i] = false;

			//	------------------------------------------AJOUT DE L'ENTETE DE LA SECTION------------------------------------------
			//	Ajout de l'entête de section du fichier 2 après toutes les entêtes de section du fichier 1
			elf_file1->a_shdr = realloc(elf_file1->a_shdr, sizeof(Elf32_Shdr) * (elf_file1->elf_header->e_shnum + 1));
			memcpy(&(elf_file1->a_shdr[elf_file1->elf_header->e_shnum]), &(elf_file2->a_shdr[i]), sizeof(Elf32_Shdr));
			elf_file1->elf_header->e_shnum += 1;
			
			//	------------------------------------------AJOUT DU NOM DE LA SECTION------------------------------------------
			//	Modification du sh_name de la nouvelle section ajoutée
			elf_file1->a_shdr[elf_file1->elf_header->e_shnum - 1].sh_name = elf_file1->a_shdr[elf_file1->elf_header->e_shstrndx].sh_size;			
			
			//	Récupération du nom de la section à ajouter
			char nom_section[strlen(get_name(elf_file2, i))];
			strcpy(nom_section, get_name(elf_file2, i));
			
			// 	Modification du size de sh_strtab (le +1 correspond au caractère '\0')
			elf_file1->a_shdr[elf_file1->elf_header->e_shstrndx].sh_size += strlen(nom_section) + 1;
			
			//	Concaténation du nom de la section à la fin de .shstrtab
			elf_file1->sections_content = realloc(elf_file1->sections_content, sizeof *(elf_file1->sections_content) * elf_file1->elf_header->e_shnum + (strlen(nom_section)+1) );			
			for(j = 0; j < strlen(nom_section); j++){
				elf_file1->sections_content[elf_file1->elf_header->e_shstrndx][elf_file1->a_shdr[elf_file1->elf_header->e_shnum - 1].sh_name + j] = nom_section[j];
			}			
			elf_file1->sections_content[elf_file1->elf_header->e_shstrndx][elf_file1->a_shdr[elf_file1->elf_header->e_shnum - 1].sh_name + j] = '\0';			
			
			//	Modification de l'offset de toutes les sections suivant .shstrtab
			maj_offset(elf_file1,elf_file1->elf_header->e_shstrndx, strlen(nom_section) + 1);
			
			//	------------------------------------------AJOUT DE LA SECTION------------------------------------------			
			// 	Appel de max_offset_section() qui recherche l'indice de la section d'offset maximum
			ind_sect_max = max_offset_section(elf_file1);
			//	Calcul de l'offset auquel ajouter les sections du fichier 2 manquantes dans le fichier 1
			add_off_sect = elf_file1->a_shdr[ind_sect_max].sh_offset + elf_file1->a_shdr[ind_sect_max].sh_size;
			
			//	Modification de l'offset de la section ajoutée
			elf_file1->a_shdr[elf_file1->elf_header->e_shnum - 1].sh_offset = add_off_sect;
					
			//	Ajout du contenu de la section du fichier 2 après toutes les autres sections du fichier 1
			elf_file1->sections_content = realloc(elf_file1->sections_content, sizeof *(elf_file1->sections_content) * elf_file1->elf_header->e_shnum + elf_file1->a_shdr[elf_file1->elf_header->e_shnum - 1].sh_size);
			memcpy(&(elf_file1->sections_content[elf_file1->elf_header->e_shnum - 1]), &(elf_file2->sections_content[i]), elf_file1->a_shdr[elf_file1->elf_header->e_shnum - 1].sh_size);
			
			//	Modification dans le header de l'offset de la première entête de section
			elf_file1->elf_header->e_shoff += (strlen(nom_section) + 1) + elf_file2->a_shdr[i].sh_size;

		}	
	}
}
