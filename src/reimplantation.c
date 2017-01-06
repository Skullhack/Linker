#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "section_header.h"
#include "util.h"
#include "global_struct.h"



void tab_Reimplantation(ELF_STRUCT * elf){

	int t_rel =0;
	int t_rela =0;
	int offset =0;
	int tab_name =0;
	int taille =0;

	elf->tab_reimplant = malloc(sizeof(char));
	elf->a_rel = malloc(sizeof(Elf32_Rel));

	for(int i; i< elf->elf_header->e_shnum;i++ ){
		
		if(elf->a_shdr[i].sh_type == SHT_REL){
			
			/***********************************/
			offset = elf->a_shdr[i].sh_offset ;
			fseek(elf->elf_file,offset,SEEK_SET);

			taille = elf->a_shdr[i].sh_size/sizeof(Elf32_Rel);	
			t_rel = t_rel + taille;
			elf->a_rel = realloc(elf->a_rel,sizeof(Elf32_Rel)*t_rel);
			fread(elf->a_rel,sizeof(Elf32_Rel),taille,elf->elf_file);

			/***********************************/
	
			for(int j = t_rel-taille; j<t_rel;j++){

				/***********************************/
				elf->tab_reimplant = realloc(elf->tab_reimplant,sizeof(char)*tab_name+1);
				elf->tab_reimplant[tab_name]=i;
				tab_name++;
				/***********************************/

				if (elf->elf_header->e_ident[EI_DATA] == 2) {
					elf->a_rel[j].r_offset = reverse_4(elf->a_rel[j].r_offset);
					elf->a_rel[j].r_info = reverse_4(elf->a_rel[j].r_info );
				}
			}
			
		}else if(elf->a_shdr[i].sh_type == SHT_RELA){
			
			/***********************************/
			offset = elf->a_shdr[i].sh_offset ;
			fseek(elf->elf_file,offset,SEEK_SET);

			taille = elf->a_shdr[i].sh_size/sizeof(Elf32_Rela);	
			t_rela = t_rela + taille;
			elf->a_rel = realloc(elf->a_rel,sizeof(Elf32_Rela)*t_rela);
			fread(elf->a_rela,sizeof(Elf32_Rela),taille,elf->elf_file);

			/***********************************/
	
			for(int j = t_rela-taille; j<t_rela;j++){

				/***********************************/
				elf->tab_reimplant = realloc(elf->tab_reimplant,sizeof(char)*tab_name+1);
				elf->tab_reimplant[tab_name]=i;
				tab_name++;
				/***********************************/

				if (elf->elf_header->e_ident[EI_DATA] == 2) {
					elf->a_rela[j].r_offset = reverse_4(elf->a_rela[j].r_offset);
					elf->a_rela[j].r_info = reverse_4(elf->a_rela[j].r_info );
					elf->a_rela[j].r_addend = reverse_4(elf->a_rela[j].r_addend );
				}
			}
		} 
	}
	elf->taille_rel = t_rel;
	elf->taille_rela = t_rela;
}

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
			printf("%-20d│",ELF32_R_TYPE(elf->a_rel[i].r_info));
			printf("%-6x║\n",ELF32_R_SYM(elf->a_rel[i].r_info));
		}
		printf("╚═════════╧═════════╧════════════════════╧══════╝\n");
	}
}


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
