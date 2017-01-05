#include <stdlib.h>
#include <stdio.h>
#include <elf.h>

int creer_table_symbole(GLOBAL_STRUCT * elf){
	//table_symboles tab_symb;
	//uint16_t taille_sections;
	int reverse_needed;
	int i=0;
	int taille_tab_symb;
	uint32_t adr_tab_symb;
	int nb_symb;
	
	//On obtient le numero de l'entete de section de la table des symboles 
	while(i < elf.elf_header->e_shnum && elf.a_shdr[i]->sh_type != 2){
		i += 1;	
	}
	
	if(i < elf.elf_header->e_shnum){
		taille_tab_symb=elf.a_shdr[i]->sh_size;	
		adr_tab_symb=elf.a_shdr[i]->sh_offset;		
	}	
	else{ 
		return -1;
	}	

	//taille_sections = header->e_shnum * header->e_shentsize;
	
	fseek(f, taille_tab_symb, adr_tab_symb);
	
	nb_symb = taille_tab_symb / sizeof(Elf32_Sym);
	
	// We read the symbole table one by one
	
	for (i=0; i<nb_symb;i++){
   		if ( fread(&(elf.*a_sym[i]), sizeof(Elf32_Sym), 1, f) == -1 ) {
        	return -1;
    	}
    }    
    
    // A CHANGER A PARTIR D'ICI !!!
    // big endian / little endian
    reverse_needed = need_reverse( elf.elf_header->e_ident[EI_DATA] );

    if (reverse_needed) {    
    	for (i=0; i<nb_symb;i++){

		    elf.a_sym[i]->st_name = reverse_4(elf.a_sym[i]->st_name);
		    elf.a_sym[i]->st_value = reverse_4(elf.a_sym[i]->st_value);
		    elf.a_sym[i]->st_size = reverse_4(elf.a_sym[i]->st_size);
		    elf.a_sym[i]->st_shndx = reverse_2(elf.a_sym[i]->st_shndx);
		    
		}
    }

	return 1;	
}

void afficher_table(Elf32_Sym tab_symb){
	
	for (i=0; i<nb_symb;i++){
	
		printf("Le st_name de la table des symboles %d est : %d\n", elf.a_sym[i]->st_name);
	
	}
	
}
