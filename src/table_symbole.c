#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>

#include "util.h"
#include "global_struct.h"
#include "table_symbole.h"

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Chargement des données dans la structure "elf"
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Affichage de la table des symboles
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void afficher_table(ELF_STRUCT * elf){
	int i=0, j=0;
	int taille_tab_symb;
	int nb_symb;
	uint32_t adr_tab_str;
	int adr_symb_name;
	char symb_name[50]="";
	
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
	
	printf("╔══════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                \e[1;31mTable des symboles:\e[0m                                   ║\n");
    printf("╟────┬─────────┬───────┬────────┬───────┬──────────┬────┬──────────────────────────────╢\n");
    printf("║%-4s│%-9s│%-7s│%-8s│%-7s│%-10s│%-4s│%-30s║\n","Num","Valeur","Taille","Type","Lien","Visible","Ndx","Nom");
    printf("╟────┼─────────┼───────┼────────┼───────┼──────────┼────┼──────────────────────────────╢\n");
    
	//On parcours la structure de chaque symbole
	for (i=0; i<nb_symb;i++){
		
		//Affichage de toutes les variables de la structure du symbole sauf le nom
		printf("║%-4d│", i);
		printf("%-9.8x│", elf->a_sym[i].st_value);
		printf("%-7d│", elf->a_sym[i].st_size);		
		printf("%-8s│",case_typeSym(elf->a_sym[i].st_info));
		printf("%-7s│",case_bind(elf->a_sym[i].st_info));
		printf("%-10s│",case_vis(elf->a_sym[i].st_other));
		afficher_ndx(elf->a_sym[i].st_shndx);
		printf("│");
		//Affichage du nom
		//Si la valeur de l'indice/st_name du symbole est nulle, il n'a pas de nom
		if(elf->a_sym[i].st_name == 0){
			printf("%-30s║\n","");
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
			
			printf("%-30s║\n", symb_name);
		}		
		
	}
	printf("╚════╧═════════╧═══════╧════════╧═══════╧══════════╧════╧══════════════════════════════╝\n");
	
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Traduit les variables en texte
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* case_typeSym(unsigned char st_info){
	
	st_info = ELF32_ST_TYPE(st_info);
	switch(st_info){
		case 0 :
		 	return "NOTYPE";
		 	break;
		case 1 :
		 	return "OBJECT";
			break;
		case 2 :
		 	return "FUNC";
			break;			 
		case 3 :
		 	return "SECTION";
			break;			 
		case 4 :
		 	return "FILE";
			break;			 
		case 13 :
		 	return "LOPROC";
			break;			 
		case 15 :
			return "HIPROC";
			break;
		default: 
		 	return "UNDEF";
	}
}

char* case_bind(unsigned char st_info){
	
	st_info = ELF32_ST_BIND(st_info);
	
	switch(st_info){
		case 0 :
		 	return "LOCAL";
		 	break;
		case 1 :
		 	return "GLOBAL";
			break;
		case 2 :
		 	return "WEAK";
			break;			 
		case 13 :
		 	return "LOPROC";
			break;			 
		case 15 :
		 	return "HIPROC";
			break;
		default: 
		 	return " ";
	}
}

char* case_vis(unsigned char st_other){
	switch(st_other){
		case 0 :
		 	return "DEFAULT";
		 	break;
		case 1 :
		 	return "INTERNAL";
			break;
		case 2 :
		 	return "HIDDEN";
			break;			 
		case 3 :
		 	return "PROTECTED";
			break;
		default: 
		 	return " ";
	}
}

void afficher_ndx(Elf32_Section st_shndx){
	switch(st_shndx){
		case 0 :
		 	printf("%-4s","UND");
		 	break;
		case 65521 :
		 	printf("%-4s","ABS");
			break;
		case 65522 :
		 	printf("%-4s","COM");
			break;
		default:
			printf("%-4d", st_shndx);
	}
}


