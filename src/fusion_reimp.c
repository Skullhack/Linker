#include <elf.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

//	ATTENTION : DANS LA STRUCTURE, LES SECTIONS (**sections_content) SONT DANS LE MEME ORDRE QUE LES ENTETES DE SECTION (c'est plus simple pour programmer et on réécrira grâce aux offsets dans le fichier final)

// 	A FAIRE :
//	--------------------------------PARTIE 1 :----------------------------
//	Modifier le contenu des sections avec la fusion des sections du fichier 2 (REALLOC de elf_file1->sections_content[])
//	--------------------------------PARTIE 2 :----------------------------
//	Concaténation de chaque nouvelle entête de section après la dernière entête de section (REALLOC de elf_file1->a_shdr)
// 	Modifier le sh_name des nouvelles sections ajoutées
// 	Modifier le size de sh_strtab
//  Modifier le contenu de sh_strtab (REALLOC de elf_file1->sections_content[])
//	Concaténation du contenu de chaque nouvelle section à la suite des sections (REALLOC de elf_file1->sections_content)
//	Dans le header recalculer l'offset de la première entête de section

void offset_section_update(int ind_sect, Elf32_Word size, ELF_STRUCT * elf){
	int k = 0;

	for(k = 0; k < elf->elf_header->e_shnum ; k++){
	
		if(elf->a_shdr[k].sh_offset > elf->a_shdr[ind_sect].sh_offset){
		
			elf->a_shdr[k].sh_offset += size;
		
		}
	}
}

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

void fusion_reimp(ELF_STRUCT * elf_file1, ELF_STRUCT * elf_file2){
	bool missing[elf_file2->elf_header->e_shnum];
	int i = 0, j = 0, ind_sect_max = 0, add_off_sect = 0;
	
	/*	Initialisation du tableau de booléens des sections du fichier 2 afin de 
		marquer celles de type SHT_REL qui ne seront pas fusionnées dans le fichier 3 */
	for(i = 0 ; i < elf_file2->elf_header->e_shnum; i++){
	
		missing[i] = true;
	
	}
	
	// 	-------------------------------PARTIE 1 - FUSION--------------------------------

	/*	Recherche des sections de même type SHT_REL et de même nom dans les 
		fichiers 2 et 3 afin de les fusionner dans le fichier 3 */
	for(i = 0; i < elf_file1->elf_header->e_shnum; i++){
		
		if(elf_file1->a_shdr[i].sh_type == SHT_REL){
			
			for(j = 0; j < elf_file2->elf_header->e_shnum; j++){
				
				if(elf_file2->a_shdr[j].sh_type == SHT_REL && strcmp(get_name(elf_file1, i), get_name(elf_file2, j)) == 0 ){
					
					// 	Dans le tableau de booléens, passage à FALSE des sections fusionnées
					missing[j] = false;
					
					//	Concaténation de sa section à la fin de la section équivalente dans elf_file1->sections_content[i]
					elf_file1->sections_content = realloc(elf_file1->sections_content, sizeof(elf_file1->sections_content) + elf_file2->a_shdr[j].sh_size);
					elf_file1->sections_content[i] = "" + "";
					
					// 	Modification du size de la section du fichier 3 en vu de la fusion
					elf_file1->a_shdr[i].sh_size += elf_file2->a_shdr[j].sh_size;
			
					/*	Décallage du offset de la première entête de section qui se
					 	trouve dans le header */					
					elf_file1->elf_header->e_shoff += elf_file2->a_shdr[j].sh_size;
					
					/*	Appelle de offset_section_update() qui modifie l'offset de toutes
						les sections qui se trouvent après l'offset de la fonction actuelle */
					offset_section_update(i, elf_file2->a_shdr[j].sh_size, elf_file1);
				}
			}
		}
	}
	
	// 	-------------------------------PARTIE 2 - CONCATENATION--------------------------------
	
	// 	Appelle de max_offset_section() qui recherche la section d'offset maximum
	ind_sect_max = max_offset_section(elf_file1);
	/*	Calcule de la position à laquelle ajouter les sections du fichier 2 manquantes
		dans le fichier 3 */
	add_off_sect = elf_file1->a_shdr[ind_sect_max].sh_offset + elf_file1->a_shdr[ind_sect_max].sh_size;
	
	//	Parcours du tableau de booléens et ajout des sections marquées à TRUE
	for(i = 0 ; i < elf_file2->elf_header->e_shnum; i++){
	
		if(missing[i] == true){
		
			/*	Offset de la section de numéro i = offset de la dernière section du
				file3 + le size de la dernière section du file3 */
				
				// REALLOCATION DE LA TAILLE DES ENTETES DE SECTIONS
			
							
		}	
	}
}
