#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "section_header.h"
#include "util.h"
#include "global_struct.h"
#include "reimplantation.h"

const char* relType[] = 
    { 
        "R_ARM_NONE", "R_ARM_PC24", "R_ARM_ABS32", "R_ARM_REL32", "R_ARM_LDR_PC_G0", 
        "R_ARM_ABS16", "R_ARM_ABS12", "R_ARM_THM_ABS5", "R_ARM_ABS8", "R_ARM_SBREL32", 
        "R_ARM_THM_CALL", "R_ARM_THM_PC8", "R_ARM_BREL_ADJ", "R_ARM_TLS_DESC", "R_ARM_THM_SWI8", 
        "R_ARM_XPC25", "R_ARM_THM_XPC22", "R_ARM_TLS_DTPMOD32", "R_ARM_TLS_DTPOFF32", "R_ARM_TLS_TPOFF32",
        "R_ARM_COPY", "R_ARM_GLOB_DAT", "R_ARM_JUMP_SLOT", "R_ARM_RELATIVE", "R_ARM_GOTOFF32", 
        "R_ARM_BASE_PREL", "R_ARM_GOT_BREL", "R_ARM_PLT32", "R_ARM_CALL", "R_ARM_JUMP24",
        "R_ARM_THM_JUMP24", "R_ARM_BASE_ABS", "R_ARM_ALU_PCREL_7_0", "R_ARM_ALU_PCREL_15_8", "R_ARM_ALU_PCREL_23_15", 
        "R_ARM_LDR_SBREL_11_0_NC", "R_ARM_ALU_SBREL_19_12_NC", "R_ARM_ALU_SBREL_27_20_CK", "R_ARM_TARGET1", "R_ARM_SBREL31", 
        "R_ARM_V4BX", "R_ARM_TARGET2", "R_ARM_PREL31", "R_ARM_MOVW_ABS_NC", "R_ARM_MOVT_ABS", 
        "R_ARM_MOVW_PREL_NC", "R_ARM_MOVT_PREL", "R_ARM_THM_MOVW_ABS_NC", "R_ARM_THM_MOVT_ABS", "R_ARM_THM_MOVW_PREL_NC",
        "R_ARM_THM_MOVT_PREL", "", "", "", "", "" 
    };

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Chargement des données dans la structure "elf"
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tab_Reimplantation(ELF_STRUCT * elf){

	int t_rel =0;
	int t_rela =0;

	int offset =0;


	elf->tab_reimplant = malloc(sizeof(char));

	int taille =0;
	//Boucle sur le nombre de section
	for(int i = 0; i< elf->elf_header->e_shnum;i++ ){
		//Si c'est une section de type Rel, incrémente la taille pour la partie réservé à cette table
		if(elf->a_shdr[i].sh_type == SHT_REL){
			taille = elf->a_shdr[i].sh_size/sizeof(Elf32_Rel) + taille;
		}
	}
	//Alloue la partie réservé à cette table grâce à la taille calculé précédemment
	elf->a_rel = malloc(sizeof(Elf32_Rel)*taille);	

	//Maintenant on reparcours selon le nombre de section pour charger la table de réimplantation
	for(int i = 0; i< elf->elf_header->e_shnum;i++ ){
		//Si c'est bien une section Rel
		if(elf->a_shdr[i].sh_type == SHT_REL){
			//On place la tête de lecture sur l'offset de la section
			offset = elf->a_shdr[i].sh_offset ;
			fseek(elf->elf_file,offset,SEEK_SET);

			taille = elf->a_shdr[i].sh_size/sizeof(Elf32_Rel);	
			for(int j = 0; j < taille; j++){
				if (fread(&(elf->a_rel[t_rel]),sizeof(Elf32_Rel),1,elf->elf_file)==-1) {
					return -1;
				}
				t_rel++;
			}
		}
	}
	elf->taille_rel = t_rel;
	elf->taille_rela = t_rela;
	return 1;
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Affichage de la table de réimplantation des Rel
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Affichage_Rel(ELF_STRUCT * elf){
	if (elf->taille_rel==0){
		printf("Aucne table de Rel\n");
	}else{
		printf("╔═══════════════════════════════════════════════╗\n");
		printf("║               \e[1;31mTable des Rel :\e[0m                 ║\n");
		printf("╟─────────┬─────────┬────────────────────┬──────╢\n");
		printf("║%-10s│%-9s│%-20s│%-6s║\n","Décalage","Info","Type","Index");
		printf("╟─────────┼─────────┼────────────────────┼──────╢\n");
	
		for(int i = 0; i < elf->taille_rel;i++){
		
			printf("║%9.8x│",elf->a_rel[i].r_offset);
			printf("%9.8x│",elf->a_rel[i].r_info);
			printf("%-20s│",relType[ELF32_R_TYPE(elf->a_rel[i].r_info)]);
			printf("%-6x║\n",ELF32_R_SYM(elf->a_rel[i].r_info));
		}
		printf("╚═════════╧═════════╧════════════════════╧══════╝\n");
	}
}

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Affichage de la table de réimplantation des Rela
*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Affichage_Rela(ELF_STRUCT * elf){

	if (elf->taille_rela==0){
		printf("Aucne table de Rela\n");
	}else{
		printf("╔══════════════════════════════════════════════════╗\n");
		printf("║                \e[1;31mTable des Rela :\e[0m                  ║\n");
		printf("╟─────────┬─────────┬────────────────────┬─────────╢\n");
		printf("║%-10s│%-9s│%-20s│%-9s║\n","Décalage","Info","Type","Adresse");
		printf("╟─────────┼─────────┼────────────────────┼─────────╢\n");
		for(int i = 0; i < elf->taille_rela;i++){
			printf("║%9.8x\n",elf->a_rela[i].r_offset);
			printf("%9.8d\n",elf->a_rela[i].r_info);
			printf("%-20d\n",ELF32_R_TYPE(elf->a_rela[i].r_info));
			printf("%9.8x\n",elf->a_rela[i].r_addend);
		}
		printf("╚═════════╧═════════╧════════════════════╧═════════╝\n");
	}
}
