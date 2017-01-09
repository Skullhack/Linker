#include <elf.h>

void fusion_reimp(ELF_STRUCT * elf_file1, ELF_STRUCT * elf_file2, ELF_STRUCT * elf_file3){
	bool missing[elf_file2->elf_header->e_shnum];
	
	/* Initialisation du tableau des sections du fichier 2 avec un flag
	   pour savoir si chacune a une section du même nom qui existe dans le fichier 1*/ 
	for(int i = 0 ; i < elf_file2->elf_header->e_shnum; i++){
	
		missing[i].flag = TRUE;
	
	}

	for(int i = 0; i < elf_file3->elf_header->e_shnum; i++){
		
		if(elf_file3->a_shdr[i].sh_type == SHT_REL){
			
			for(int j = 0; j < elf_file2->elf_header->e_shnum; j++){
				
				if(elf_file2->a_shdr[j].sh_type == SHT_REL && //et même nom){
					
					tab_miss[j].flag = FALSE;
					
					//Concatener et recalculer les offsets
					
				}
				
			}
		}
	}

}
