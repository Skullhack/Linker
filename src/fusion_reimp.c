#include <elf.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

/*	!!! ATTENTION !!! : DANS LA STRUCTURE, LES SECTIONS (**sections_content) SONT DANS 
	LE MEME ORDRE QUE LES ENTETES DE SECTION (c'est plus simple pour programmer et on 
	réécrira grâce aux offsets dans le fichier final)	*/

// 	A FAIRE :
//	--------------------------------PARTIE 1 :----------------------------
//	Modifier le contenu des sections avec la fusion des sections du fichier 2 (REALLOC de elf_file1->sections_content[])		----------> FAIT
//	--------------------------------PARTIE 2 :----------------------------
//	Concaténation de chaque nouvelle entête de section après la dernière entête de section (REALLOC de elf_file1->a_shdr)		----------> FAIT
// 	Modifier le sh_name des nouvelles sections ajoutées																			----------> FAIT
// 	Modifier le size de sh_strtab																								----------> FAIT
//  Modifier le contenu de sh_strtab (REALLOC de elf_file1->sections_content[])													----------> FAIT
//	Modification de l'offset de toutes les sections qui se trouvent après .shstrtab (nouvel appel de offset_section_update())	---------->	FAIT
//	Concaténation du contenu de chaque nouvelle section à la suite des sections (REALLOC de elf_file1->sections_content)		---------->	FAIT
//	Dans le header recalculer l'offset de la première entête de section

//	Renvoie l'indice de la section d'offset maximum
int max_offset_section(ELF_STRUCT * elf){
	int i = 0, ind_max = 0, max_offset = 0;
	
	ind_max = i;
	max_offset = elf->a_shdr[i].sh_offset ;

	for(i = 1; i < elf->elf_header->e_shnum ; i++){
	
		if(elf->a_shdr[i].sh_offset > max_offset){
		
			ind_max = i;
			max_offset = elf->a_shdr[i].sh_offset;		
			
		}
	}
	
	return ind_max;
}

//	Programme principale de fusion des tables de réimplantation de 2 fichiers objets
void fusion_reimp(ELF_STRUCT * elf_file1, ELF_STRUCT * elf_file2){
	bool missing[elf_file2->elf_header->e_shnum];
	int i = 0, j = 0, ind_sect_max = 0, add_off_sect = 0;
	
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
					
					/*	Concaténation de la section du fichier 2 à la fin de la section 
						équivalente dans elf_file1->sections_content[i]	*/
					elf_file1->sections_content = realloc(elf_file1->sections_content, sizeof *(elf_file1->sections_content) * elf_file1->elf_header->e_shnum + elf_file2->a_shdr[j].sh_size);	//	A CONFIRMER
					elf_file1->sections_content[i] = strcat(elf_file1->sections_content[i], elf_file2->sections_content[j]);	//	A CONFIRMER
					
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
			elf_file1->a_shdr = realloc(elf_file1->a_shdr, sizeof(Elf32_Shdr) * (elf_file1->elf_header->e_shnum + 1));	//	A CONFIRMER
			memcpy(&(elf_file1->a_shdr[elf_file1->elf_header->e_shnum]), &(elf_file2->a_shdr[i]), sizeof(Elf32_Shdr));	//	A CONFIRMER
			elf_file1->elf_header->e_shnum += 1;
			
			//	------------------------------------------AJOUT DU NOM DE LA SECTION------------------------------------------
			//	Modification du sh_name de la nouvelle section ajoutée
			elf_file1->a_shdr[elf_file1->elf_header->e_shnum - 1].sh_name = elf_file1->a_shdr[elf_file1->elf_header->e_shstrndx].sh_size;
			
			// 	Modification du size de sh_strtab (le +1 correspond au caractère '\0')
			elf_file1->a_shdr[elf_file1->elf_header->e_shstrndx].sh_size += strlen(get_name(elf_file2, i)) + 1;
			
			//	Concaténation du nom de la section à la fin de .shstrtab
			elf_file1->sections_content = realloc(elf_file1->sections_content, sizeof *(elf_file1->sections_content) * elf_file1->elf_header->e_shnum + (strlen(get_name(elf_file2, i))+1) );	//	A CONFIRMER
			elf_file1->sections_content[elf_file1->elf_header->e_shstrndx] = strcat(elf_file1->sections_content[elf_file1->elf_header->e_shstrndx], get_name(elf_file2, i));	//	A CONFIRMER
			
			//	Modification de l'offset de toutes les sections suivant .shstrtab
			maj_offset(elf_file1,elf_file1->elf_header->e_shstrndx, strlen(get_name(elf_file2, i)) + 1);
			
			//	------------------------------------------AJOUT DE LA SECTION------------------------------------------			
			// 	Appel de max_offset_section() qui recherche l'indice de la section d'offset maximum
			ind_sect_max = max_offset_section(elf_file1);
			//	Calcul de l'offset auquel ajouter les sections du fichier 2 manquantes dans le fichier 1
			add_off_sect = elf_file1->a_shdr[ind_sect_max].sh_offset + elf_file1->a_shdr[ind_sect_max].sh_size;
			
			//	Modification de l'offset de la section ajoutée
			elf_file1->a_shdr[elf_file1->elf_header->e_shnum - 1].sh_offset = add_off_sect;
					
			//	Ajout du contenu de la section du fichier 2 après toutes les autres sections du fichier 1
			elf_file1->sections_content = realloc(elf_file1->sections_content, sizeof *(elf_file1->sections_content) * elf_file1->elf_header->e_shnum + elf_file1->a_shdr[elf_file1->elf_header->e_shnum - 1].sh_size);	//	A CONFIRMER
			memcpy(&(elf_file1->sections_content[elf_file1->elf_header->e_shnum - 1]), &(elf_file2->sections_content[i]), elf_file1->a_shdr[elf_file1->elf_header->e_shnum - 1].sh_size);	//	A CONFIRMER
			
			//	Modification dans le header de l'offset de la première entête de section
			elf_file1->elf_header->e_shoff += (strlen(get_name(elf_file2, i)) + 1) + elf_file2->a_shdr[i].sh_size;

		}	
	}
}
