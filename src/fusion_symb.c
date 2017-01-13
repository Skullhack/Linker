#include <elf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "section_header.h"
#include "util.h"
#include "global_struct.h"
#include "fusion_symb.h"

#include "table_symbole.h"

/*********************************************************/
void fusion_symb(ELF_STRUCT * elf,ELF_STRUCT * elf1){
	
	/*********************************************************/
	int taille_tab = taille_tab_symb(elf);
	cel * tab = malloc(sizeof(cel)*taille_tab);

	for(int i = 0 ; i < taille_tab; i++){
		//tab[i].sym = elf->a_sym[i];
		memcpy(&(tab[i].sym),&(elf->a_sym[i]),sizeof(Elf32_Sym));
		tab[i].name = recup_name(elf,elf->a_sym[i].st_name);
	}
	/*********************************************************/
	//printf("Taille tab: %d \n",taille_tab);
	/*********************************************************/
	liste l_sym = NULL;
	int taille_sym = taille_tab_symb(elf1);
	for(int i = 0 ; i < taille_sym; i++){
		Ajouter(elf1,i, &l_sym);
	}
	/*********************************************************/
	//printf("Taille sym: %d \n",taille_sym);
	/*********************************************************/

	/**********************************************************
	int i_size_tab = 0;
	while(i_size_tab < elf->elf_header->e_shnum && elf->a_shdr[i_size_tab].sh_type != 3){
		i_size_tab += 1;
		if(i_size_tab==elf->elf_header->e_shstrndx){
			i_size_tab += 1;
		}
	}
	*******
	int i_size_tab1 = 0;
	while(i_size_tab1 < elf1->elf_header->e_shnum && elf1->a_shdr[i_size_tab1].sh_type != 3){
		i_size_tab1 += 1;
		if(i_size_tab1==elf1->elf_header->e_shstrndx){
			i_size_tab1 += 1;
		}
	}
	**********************************************************/
	int taille_res = 0;
	int j =0;
	bool b = false;
	bool b1 = true;
	cel * res = malloc(sizeof(cel)*(taille_tab+taille_sym));
	liste ac = NULL;
	


	for(int i = 0; i < taille_tab; i++){
		if((tab[i].sym.st_info >> 4) == STB_LOCAL){ 				//ELF32_ST_BIND() 
			Ajout(tab[i].sym,res,taille_res);
			
			res[taille_res].name = malloc(sizeof(char)*strlen(tab[i].name));
			strcpy(res[taille_res].name,tab[i].name);
			taille_res ++;
		}
		j=0;
		ac = l_sym;
		b = false;
		while(j < taille_sym && ac !=NULL ){
			if((ac->donnee.sym.st_info >> 4) == STB_LOCAL && b1){
				Ajout(ac->donnee.sym,res,taille_res);
				/**
				res[taille_res].sym.st_name = res[taille_res].sym.st_name + elf->a_shdr[i_size_tab].sh_size;
				**/
				res[taille_res].name = malloc(sizeof(char)*strlen(ac->donnee.name));
				strcpy(res[taille_res].name,ac->donnee.name);
				taille_res ++;
				Supprimer(&l_sym,j);
				j--;
				taille_sym--;
			}

			if(globaux(ac->donnee.sym) && globaux(tab[i].sym)){		
				if(strcmp(ac->donnee.name,tab[i].name)==0){
					if(def(tab[i].sym) && def(ac->donnee.sym)){
						exit(1);
					}else if(ndef(tab[i].sym) && ndef(ac->donnee.sym)){
						Ajout(tab[i].sym,res,taille_res);
						
						res[taille_res].name = malloc(sizeof(char)*strlen(tab[i].name));
						strcpy(res[taille_res].name,tab[i].name);

						taille_res ++;
						Supprimer(&l_sym,j);
						j--;
						taille_sym--;
						b = true;
					}else{
						if(def(tab[i].sym)){ // def(elf,i) && ndef(elf1,j)
							Ajout(tab[i].sym,res,taille_res);

							res[taille_res].name = malloc(sizeof(char)*strlen(tab[i].name));
							strcpy(res[taille_res].name,tab[i].name);

							taille_res ++;
							Supprimer(&l_sym,j);
							taille_sym--;
							j--;
							b = true;
						}else{					// ndef(elf,i) && def(elf1,j)
							Ajout(ac->donnee.sym,res,taille_res);
							/**
						res[taille_res].sym.st_name = res[taille_res].sym.st_name +elf->a_shdr[i_size_tab].sh_size;
							**/
							res[taille_res].name = malloc(sizeof(char)*strlen(ac->donnee.name));
							strcpy(res[taille_res].name,ac->donnee.name);

							taille_res ++;
							Supprimer(&l_sym,j);
							taille_sym--;
							j--;
							b = true;
						}
					}
				}
			}
			j++;
			ac = ac->suiv;
		}
		if(globaux(tab[i].sym) && b ==false){
			Ajout(tab[i].sym,res,taille_res);
			res[taille_res].name = malloc(sizeof(char)*strlen(tab[i].name));
			strcpy(res[taille_res].name,tab[i].name);
			taille_res ++;
		}
	}
	b1 = false;
	
	/************************Ajout derniere variable *********************************/
	ac = 	l_sym;
	while(ac !=NULL){
		Ajout(ac->donnee.sym,res,taille_res);
		/**
		res[taille_res].sym.st_name = res[taille_res].sym.st_name + elf->a_shdr[i_size_tab].sh_size;
		**/
		res[taille_res].name = malloc(sizeof(char)*strlen(ac->donnee.name));
		strcpy(res[taille_res].name,ac->donnee.name);

		taille_res ++;
		ac = ac->suiv;
	}
	
	/********************SIZE SYM / Allocation *********************/
	int i = 0;
	int offset_sym =0;

	while(i < elf->elf_header->e_shnum && elf->a_shdr[i].sh_type != SHT_SYMTAB){
		i += 1;	
	}
	elf->a_shdr[i].sh_size = sizeof(Elf32_Sym)*taille_res;
	elf->a_sym = realloc(elf->a_sym,sizeof(Elf32_Sym)*taille_res);

	/********************OFFSET SYM_NOM *********************/
	offset_sym = elf->a_shdr[i].sh_offset;
	i=0;
	while(i < elf->elf_header->e_shnum && elf->a_shdr[i].sh_type != 3){
		i += 1;
		
		if(i==elf->elf_header->e_shstrndx){
			i += 1;
		}
	}
	elf->a_shdr[i].sh_offset = offset_sym + sizeof(Elf32_Sym)*taille_res;
	
	/********************Remplir tab_Sym / Modif contenu tab_nom *********************/
	
	int cpt_name =0;
	for(int k = 0; k < taille_res; k++){
		cpt_name = strlen(res[k].name)+ cpt_name;
		cpt_name ++;
	}

	char * str_name = NULL;
	str_name = malloc(sizeof(char)*cpt_name);
	cpt_name = 0;

	int cpt =0;
	for(int k = 0; k < taille_res; k++){

		res[k].sym.st_name = cpt;
		for(int l = 0; l < strlen(res[k].name); l++){
				str_name[cpt]=res[k].name[l];
				cpt++;
		}		
		str_name[cpt]='\0';
		cpt++;

		memcpy(&(elf->a_sym[k]),&(res[k].sym), sizeof(Elf32_Sym));
	}

	/**Teste **
	elf->a_shdr[i].sh_size =  elf->a_shdr[i].sh_size + elf1->a_shdr[i_size_tab1].sh_size;
	elf->sections_content[i] = realloc(elf->sections_content[i],sizeof(char)*elf1->a_shdr[i_size_tab1].sh_size);

	memcpy(elf->sections_content[i]+elf->a_shdr[i].sh_size,elf1->sections_content[i_size_tab1],sizeof(char)*elf1->a_shdr[i_size_tab1].sh_size);
	*/

	//free(elf->sections_content[i]);
	//elf->sections_content[i] = malloc(sizeof(char)*cpt);

	FILE * f;			
	f = fopen("sh_strtab_apres.txt", "w");

	for(int l = 0 ; l< cpt; l++){
		elf->sections_content[i][l]=str_name[l];
		//printf(" teste: %c\n",elf->sections_content[i][l]);
		fprintf(f, "%c",str_name[l]);
	}
	fclose(f);

	//memcpy(&(elf->sections_content[i]),&(str_name), sizeof(char)*cpt);
	//elf->sections_content[i] = realloc(elf->sections_content[i],sizeof(*char)*cpt);
	//free(elf->sections_content[i]);
	//elf->sections_content[i] = malloc(sizeof(char)*cpt);
  //memcpy(&(elf->sections_content[i]),&(str_name), sizeof(char)*cpt);
	/*for(int l = 0 ; l<taille_res;l++){
		printf("n°%d : %s  n: %d\n",l, recup_name(elf,elf->a_sym[l].st_name),elf->a_sym[l].st_name);
	}*/

	/************ SIZE tab_nom **************/
	//elf->a_shdr[i].sh_size = sizeof(char)*cpt;

	afficher_table_sym(elf);
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
void copie_str(char * str_name, char * str, int debut,int taille_mot ){
	for(int i = debut ; i < debut + taille_mot; i++){
		str_name[i] = str[i-debut];
	}
	str_name[debut+taille_mot] = '\0';
}
/*********************************************************/
void Ajouter(ELF_STRUCT * elf,int i, liste *p) {
	
	liste nouveau = malloc(sizeof(noeud));
	//nouveau->donnee = donnee;
	memcpy(&(nouveau->donnee.sym),&(elf->a_sym[i]),sizeof(Elf32_Sym));
	nouveau->donnee.name = recup_name(elf,elf->a_sym[i].st_name);
	nouveau->suiv = *p;
	*p = nouveau;
}
/*********************************************************/
void Supprimer(liste * lis,int i){
	
	int j =0;
	liste ac = (*lis);
	liste p = NULL;


	while(ac!= NULL && j<i){
		p = ac;
		ac = ac->suiv;
		j++;
	}
	
	if(j == 0 && p==NULL){
		p= ac->suiv;
	}else{
		p->suiv = ac->suiv;
	}
	free(ac);
}
/****************************************************/
void Ajout(Elf32_Sym element,cel * res,int taille){	
		//res = realloc(res,sizeof(Elf32_Sym)*(taille+1));
		memcpy(&(res[taille].sym), &(element), sizeof(Elf32_Sym));
}
/*********************************************************/
int globaux(Elf32_Sym tab){
	return ((tab.st_info >> 4) == STB_GLOBAL);	
}
/****************************************************/
int def(Elf32_Sym tab){
	return  tab.st_shndx != 0 ;				
}
/****************************************************/
int ndef(Elf32_Sym tab){
	return tab.st_shndx == 0 ;					
}
/****************************************************/
int taille_tab_symb(ELF_STRUCT * elf){
	int i = 0;
	int taille_tab_symb=0;
	while(i < elf->elf_header->e_shnum && elf->a_shdr[i].sh_type != SHT_SYMTAB){
		i += 1;	
	}

	if(i < elf->elf_header->e_shnum){
		taille_tab_symb=elf->a_shdr[i].sh_size;	
	}
	int nb_symb = taille_tab_symb / sizeof(Elf32_Sym);
	return nb_symb;
}
/****************************************************/
char * recup_name(ELF_STRUCT * elf,uint32_t name){

	int i =0 ; 
	while(i < elf->elf_header->e_shnum && elf->a_shdr[i].sh_type != 3){
		i += 1;
		
		if(i==elf->elf_header->e_shstrndx){
			i += 1;
		}
	}	
	int j= name;
	int t=0;
	char * symb_name = malloc(sizeof(char));

	//printf("Valeur de section : %d\n",i);
	
	while(elf->sections_content[i][j]!='\0'){
		symb_name = realloc(symb_name,sizeof(char)*t+1);
		symb_name[t] = elf->sections_content[i][j];	
		t++;
		j ++;
	}
	
	//printf(" Name : %s\n", symb_name);

	return symb_name;
}
/****************************************************/
/****************************************************/
/****************************************************/
void afficher_table_sym(ELF_STRUCT * elf){

	int i=0;
	int taille_tab_symb;
	int nb_symb;
	
	i = 0;	
	while(i < elf->elf_header->e_shnum && elf->a_shdr[i].sh_type != 2){
		i += 1;	
	}
	if(i < elf->elf_header->e_shnum){
		taille_tab_symb=elf->a_shdr[i].sh_size;	
	}
	nb_symb = taille_tab_symb / sizeof(Elf32_Sym);
	
	printf("╔══════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                \e[1;31mTable des symboles:\e[0m                                   ║\n");
    printf("╟────┬─────────┬───────┬────────┬───────┬──────────┬────┬──────────────────────────────╢\n");
    printf("║%-4s│%-9s│%-7s│%-8s│%-7s│%-10s│%-4s│%-30s║\n","Num","Valeur","Taille","Type","Lien","Visible","Ndx","Nom");
    printf("╟────┼─────────┼───────┼────────┼───────┼──────────┼────┼──────────────────────────────╢\n");

	for (i=0; i<nb_symb;i++){
		
		printf("║%-4d│", i);
		printf("%-9.8x│", elf->a_sym[i].st_value);
		printf("%-7d│", elf->a_sym[i].st_size);		
		printf("%-8s│",afficher_type_s(elf->a_sym[i].st_info));
		printf("%-7s│",afficher_bind_s(elf->a_sym[i].st_info));
		printf("%-10s│",afficher_vis_s(elf->a_sym[i].st_other));
		afficher_ndx(elf->a_sym[i].st_shndx);
		printf("│");

		if(strlen(recup_name(elf,elf->a_sym[i].st_name)) == 0){
			printf("%-30s║\n"," ");
		}
		else{		
			printf("%-30s║\n", recup_name(elf,elf->a_sym[i].st_name));
			//printf("%-30d║\n", elf->a_sym[i].st_name);
		}		
		
	}
	printf("╚════╧═════════╧═══════╧════════╧═══════╧══════════╧════╧══════════════════════════════╝\n");
	
}
char* afficher_type_s(unsigned char st_info){
	
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

char* afficher_bind_s(unsigned char st_info){
	
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

char* afficher_vis_s(unsigned char st_other){
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

void afficher_ndx_s(Elf32_Section st_shndx){
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
