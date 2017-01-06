#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>

#include "util.h"
#include "global_struct.h"

void afficher_type(unsigned char st_info){
	
	st_info = st_info & 0xf;
	
	switch(st_info){
		case 0 :
		 	printf("Type : NOTYPE\n");
		 	break;
		case 1 :
		 	printf("Type : OBJECT\n");
			break;
		case 2 :
		 	printf("Type : FUNC\n");
			break;			 
		case 3 :
		 	printf("Type : SECTION\n");
			break;			 
		case 4 :
		 	printf("Type : FILE\n");
			break;			 
		case 13 :
		 	printf("Type : LOPROC\n");
			break;			 
		case 15 :
			printf("Type : HIPROC\n");
			break;
		default: 
		 	printf("Il y a une erreur quelque part...\n Veuillez corriger l'erreur à ma place.\n Cordialement BananeGuimauve.\n");
	}
}

void afficher_bind(unsigned char st_info){
	
	st_info = st_info >> 4;
	
	switch(st_info){
		case 0 :
		 	printf("Bind : LOCAL\n");
		 	break;
		case 1 :
		 	printf("Bind : GLOBAL\n");
			break;
		case 2 :
		 	printf("Bind : WEAK\n");
			break;			 
		case 13 :
		 	printf("Bind : LOPROC\n");
			break;			 
		case 15 :
		 	printf("Bind : HIPROC\n");
			break;
		default: 
		 	printf("Il y a une erreur quelque part...\n Veuillez corriger l'erreur à ma place.\n Cordialement BATMAN.\n");
	}
}

int creer_table_symbole(ELF_STRUCT * elf){
	int reverse_needed;
	int i=0;
	int taille_tab_symb;
	uint32_t adr_tab_symb;
	int nb_symb;
	
	//On obtient le numero de l'entete de section de la table des symboles 
	while(i < elf->elf_header->e_shnum && elf->a_shdr[i].sh_type != 2){
		i += 1;	
	}
	
	if(i < elf->elf_header->e_shnum){
		taille_tab_symb=elf->a_shdr[i].sh_size;	
		adr_tab_symb=elf->a_shdr[i].sh_offset;		
	}	
	else{ 
		return -1;
	}	
	
	fseek(elf->elf_file, adr_tab_symb, SEEK_SET);
	
	nb_symb = taille_tab_symb / sizeof(Elf32_Sym);
	
	// We read the symbole table one by one	
	for (i=0; i<nb_symb;i++){
   		if ( fread(&(elf->a_sym[i]), sizeof(Elf32_Sym), 1, elf->elf_file) == -1 ) {
        	return -1;
    	}
    }    
    
    // big endian / little endian
    reverse_needed = need_reverse( elf->elf_header->e_ident[EI_DATA] );

    if (reverse_needed) {    
    	for (i=0; i<nb_symb;i++){
		    elf->a_sym[i].st_name = reverse_4(elf->a_sym[i].st_name);
		    elf->a_sym[i].st_value = reverse_4(elf->a_sym[i].st_value);
		    elf->a_sym[i].st_size = reverse_4(elf->a_sym[i].st_size);
		    elf->a_sym[i].st_shndx = reverse_2(elf->a_sym[i].st_shndx);	    
		}
    }

	return 1;	
}

void afficher_table(ELF_STRUCT * elf){
	int i=0, j=0;
	int taille_tab_symb;
	int nb_symb;
	uint32_t adr_tab_str;
	int adr_symb_name;
	char symb_name[50]="";
	
	//Methode 1 (dégueulasse): On cherche la section shstrtab dans un premier temps
	/*while(i < elf->elf_header->e_shnum && elf->a_shdr[i].sh_type != 3){
		i += 1;
	}	
	//On la saute
	i+=1;	
	//Puis on cherche la section strtab
	while(i < elf->elf_header->e_shnum && elf->a_shdr[i].sh_type != 3){
		i += 1;
	}*/	
	
	//Methode 2 (ne marche pas) : On cherche la section strtab
	/*while(i < elf->elf_header->e_shnum && strcmp(give_section_name(elf, i), ".strtab") != 0){
		i+=1;
	}*/
	
	//AIDE
	//Affiche le nom de chaque section
	/*while(i < elf->elf_header->e_shnum){
		printf("Le nom de la section est : %s et son indice : %d\n", give_section_name(elf, i), i);
		i += 1;
	}*/
	
	/*	Méthode 3 : On cherche la section strtab mais si l'indice est egal 
		à elf_header->e_shstrndx (qui correspond à l'indice de shstrtab), on le saute */
	while(i < elf->elf_header->e_shnum && elf->a_shdr[i].sh_type != 3){
		i += 1;
		
		if(i==elf->elf_header->e_shstrndx){
			i += 1;
		}
	}
	
	//Si on l'a trouvée, on récupère le décalage/offset pour se rendre à la table des strings
	if(i < elf->elf_header->e_shnum){
		adr_tab_str=elf->a_shdr[i].sh_offset;			
	}	
	
	//On cherche la section symtab
	i = 0;	
	while(i < elf->elf_header->e_shnum && elf->a_shdr[i].sh_type != 2){
		i += 1;	
	}
	
	//Si on l'a trouvée, on récupère la taille de la table des symboles
	if(i < elf->elf_header->e_shnum){
		taille_tab_symb=elf->a_shdr[i].sh_size;	
	}
	
	//Le nombre de symboles = la taille de la table des symboles / la taille de la structures d'un seul symbole
	nb_symb = taille_tab_symb / sizeof(Elf32_Sym);
	
	//On parcours la structure de chaque symbole
	for (i=0; i<nb_symb;i++){
	
		//Affichage de toutes les variables de la structure du symbole sauf le nom
		printf("Num : %d\n", i);
		printf("Value : %.8x\n", elf->a_sym[i].st_value);
		printf("Size : %d\n", elf->a_sym[i].st_size);		
		afficher_type(elf->a_sym[i].st_info);
		afficher_bind(elf->a_sym[i].st_info);
		printf("Vis : DEFAULT\n");
		printf("Ndx : %d\n", elf->a_sym[i].st_shndx);
		
		//Affichage du nom
		//Si la valeur de l'indice/st_name du symbole est nulle, il n'a pas de nom
		if(elf->a_sym[i].st_name == 0){
			printf("Name : Le symbole n'a aucun nom\n\n");
		}
		//Sinon il en possède un et on l'affiche
		else{
			/*	Calcul du décalage à effectuer jusqu'au premier caractère du symbole = 
				decalage jusqu'à la table des strings + indice du symbole au sein de la table des strings*/
			adr_symb_name = adr_tab_str + elf->a_sym[i].st_name;
			
			/*	On effectue le décalage, l'ordre des paramètres de fseek est : le descripteur de fichiers,
				le décalage à effectuer et la position à partir de laquelle on effectue le décalage (SEEK_SET = le début du fichier)*/
			fseek(elf->elf_file, adr_symb_name, SEEK_SET);
			
			//On lit la chaîne de caractères du symbole et on s'arrête lorsque l'on rencontre \0
			j=-1;	
			do{
				j += 1;
				symb_name[j] = fgetc(elf->elf_file);
				//printf("le caractère est : %c\n", symb_name[j]);			
			}while(symb_name[j] != '\0');
			
			printf("Name : %s\n\n", symb_name);
		}		
	}
}
