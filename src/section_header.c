#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include "section_header.h"
#include "util.h"
#include "global_struct.h"
#include <string.h>


/**********************************/
/************************************/
int header_section(ELF_STRUCT * elf ){
	
	fseek(elf->elf_file,elf->elf_header->e_shoff,SEEK_SET);

	elf->a_shdr = malloc(sizeof(Elf32_Shdr)*elf->elf_header->e_shnum);

	for(int i = 0; i < elf->elf_header->e_shnum; i++) {
		fread(&(elf->a_shdr[i]), sizeof(Elf32_Shdr), 1, elf->elf_file);

		if (elf->elf_header->e_ident[EI_DATA] == 2) {
			elf->a_shdr[i].sh_name = reverse_4(elf->a_shdr[i].sh_name);
			elf->a_shdr[i].sh_type = reverse_4(elf->a_shdr[i].sh_type);
			elf->a_shdr[i].sh_flags = reverse_4(elf->a_shdr[i].sh_flags);
			elf->a_shdr[i].sh_addr = reverse_4(elf->a_shdr[i].sh_addr);
			elf->a_shdr[i].sh_offset = reverse_4(elf->a_shdr[i].sh_offset);
			elf->a_shdr[i].sh_size = reverse_4(elf->a_shdr[i].sh_size);
			elf->a_shdr[i].sh_link = reverse_4(elf->a_shdr[i].sh_link);
			elf->a_shdr[i].sh_info = reverse_4(elf->a_shdr[i].sh_info);
			elf->a_shdr[i].sh_addralign = reverse_4(elf->a_shdr[i].sh_addralign);
			elf->a_shdr[i].sh_entsize = reverse_4(elf->a_shdr[i].sh_entsize);
		}
	}
	return 1;
}

/************************************/
void Affichage_section(ELF_STRUCT * elf ){
	printf("╔═══════════════════════════════════════════════════════════════════════════════════════╗\n");
	printf("║                                     \e[1;31mSection :\e[0m                                         ║\n");
	printf("╟─────┬─────────────────┬───────────────┬─────────┬───────┬───────┬───┬────┬───┬────┬───╢\n");
	printf("║%-5s│%-17s│%-15s│%-9s│%-7s│%-7s│%-3s│%-4s│%-3s│%-4s│%-3s║\n","[Nr]","Nom","Type","Adr","Offset","Taille","ES","Fan","LN","Inf","Al");
	for(int i = 0; i <  elf->elf_header->e_shnum; i++){
		printf("║  %-3d",i);
		get_name(elf,elf->a_shdr[i].sh_name,i);
		printf("│%-15s",case_type(elf->a_shdr[i].sh_type));
		printf("│%-9x",elf->a_shdr[i].sh_addr);
		printf("│%-7x",elf->a_shdr[i].sh_offset);
		printf("│%-7x",elf->a_shdr[i].sh_size);
		printf("│%-3x",elf->a_shdr[i].sh_entsize);
		case_flags(elf->a_shdr[i].sh_flags);

		printf("│%-3x",elf->a_shdr[i].sh_link);
		printf("│%-4x",elf->a_shdr[i].sh_info);
		printf("│%-3x║\n",elf->a_shdr[i].sh_addralign);
	}
	printf("╚═════╧═════════════════╧═══════════════╧═════════╧═══════╧═══════╧═══╧════╧═══╧════╧═══╝\n");
}
/************************************/
void get_name(ELF_STRUCT * elf,Elf32_Word name ,int numero){

	int offset = (int) elf->a_shdr[elf->elf_header->e_shstrndx].sh_offset;
	int nom = (int)  elf->a_shdr[numero].sh_name;

	fseek(elf->elf_file,offset+nom,SEEK_SET);

	char c;
	int cpt = 1;
	while ( (c = fgetc(elf->elf_file)) != '\0') {
		cpt++;
	}

	fseek(elf->elf_file,offset+nom,SEEK_SET);

	char str[cpt];
	fgets(str,cpt,elf->elf_file);

	printf("│%-17s",str);
}
/************************************/
void case_flags(Elf32_Word flag) {
	//printf("\n\n%x",SHF_WRITE);
	//printf("\n\n%x",SHF_ALLOC);
	//printf("\n\n%x",SHF_EXECINSTR);
	//printf("\n\n%x",SHF_MERGE);
	//printf("\n\n%x",SHF_STRINGS);
	//printf("\n\n%x",SHF_INFO_LINK);
	//printf("\n\n%x",SHF_LINK_ORDER);
	//printf("\n\n%x",SHF_OS_NONCONFORMING);
	//printf("\n\n%x",SHF_GROUP);
	//printf("\n\n%x",SHF_TLS);
	//printf("\n\n%x",SHF_COMPRESSED);
	//printf("\n\n%x",SHF_MASKOS);
	//printf("\n\n%x",SHF_MASKPROC);
	//printf("\n\n%x",SHF_ORDERED);
	//printf("\n\n%x",SHF_EXCLUDE);
	//printf("\n\n%x",flag);

	int tmp = flag;
	int cpt = 0;
	int result = 0;
	char str[3]="";
  printf("│");
	while(tmp!=0){

		if(tmp&1){
			result= 1<<cpt;
				 switch(result) {
						case SHF_WRITE:	    		strcat(str,"W");
								break;
						case SHF_ALLOC:	     		strcat(str,"A");
								break;
						case SHF_EXECINSTR:		strcat(str,"X");
								break;
						case SHF_MERGE:	     		strcat(str,"M");
								break;
						case SHF_STRINGS:	   	strcat(str,"S");
								break;
						case SHF_INFO_LINK:	 	strcat(str,"I");
								break;
						case SHF_LINK_ORDER	: 	strcat(str,"L");
								break;
						case SHF_OS_NONCONFORMING: 	strcat(str,"x");
								break;
						case SHF_GROUP:	     		strcat(str,"G");
								break;
						case SHF_TLS:		     	strcat(str,"T");
								break;
						case SHF_COMPRESSED:	  	strcat(str,"C");
								break;
						case SHF_MASKOS:	     	strcat(str,"o");
								break;
						case SHF_MASKPROC:	    	strcat(str,"p");
								break;
						case SHF_ORDERED:	      	strcat(str,"O");
								break;
						case SHF_EXCLUDE:	      	strcat(str,"E");
								break;
						default: 			strcat(str,"    ");
		  }
		}
		tmp= tmp>>1;
		cpt++;
	}
	printf("%-4s",str);
 
}
/*
char* case_flags(Elf32_Word flag) {
    switch(flag) {

	case SHF_WRITE:	    		return " Writable";
	case SHF_ALLOC:	     		return " Occupies memory during execution ";
	case SHF_EXECINSTR:		return " Executable";
	case SHF_MERGE:	     		return " Might be merged ";
	case SHF_STRINGS:	   	return " Contains nul-terminated strings";
	case SHF_INFO_LINK:	 	return " `sh_info' contains SHT index ";
	case SHF_LINK_ORDER	: 	return " Preserve order after combining ";
	case SHF_OS_NONCONFORMING: 	return " Non-standard OS specific handling required";
	case SHF_GROUP:	     		return " Section is member of a group.";
	case SHF_TLS:		     	return " Section hold thread-local data.";
	case SHF_COMPRESSED:	  	return " Section with compressed data.";
	case SHF_MASKOS:	     	return " OS-specific.";
	case SHF_MASKPROC:	    	return " Processor-specific ";
	case SHF_ORDERED:	      	return " Special ordering requirement (Solaris).";
	case SHF_EXCLUDE:	      	return " Section is excluded unless ";
	default: 			return " aucun";
    }

}*/
/************************************/
char* case_type(Elf32_Word sh_type){
	switch(sh_type) {
		case SHT_NULL:	      		return " NULL ";
		case SHT_PROGBITS:	      	return " PROGBITS";
		case SHT_SYMTAB:	      	return " SYMTAB";
		case SHT_STRTAB:	      	return " STRTAB";
		case SHT_RELA:	      		return " Relocation entries with addends ";
		case SHT_HASH:	      		return " Symbol hash table ";
		case SHT_DYNAMIC:	      	return " Dynamic linking information ";
		case SHT_NOTE:	      		return " Notes ";
		case SHT_NOBITS:	      	return " NOBITS";
		case SHT_REL:	      		return " Relocation entries, no addends ";
		case SHT_SHLIB:	      		return " Reserved ";
		case SHT_DYNSYM:	      	return " Dynamic linker symbol table ";
		case SHT_INIT_ARRAY:	      	return " Array of constructors ";
		case SHT_FINI_ARRAY:	      	return " Array of destructors ";
		case SHT_PREINIT_ARRAY:	      	return " Array of pre-constructors ";
		case SHT_GROUP:	      		return " Section group ";
		case SHT_SYMTAB_SHNDX:	      	return " Extended section indeces ";
		case SHT_NUM:	      		return " Number of defined types.  ";
		case SHT_LOOS:	      		return " Start OS-specific.  ";
		case SHT_GNU_ATTRIBUTES:	return " Object attributes.  ";
		case SHT_GNU_HASH:	      	return " GNU-style hash table.  ";
		case SHT_GNU_LIBLIST:	      	return " Prelink library list ";
		case SHT_CHECKSUM:	      	return " Checksum for DSO content.  ";
		case SHT_LOSUNW:	     	return " Sun-specific low bound.  ";
		case SHT_SUNW_COMDAT:		return " SHT_SUNW_COMDAT ";
		case SHT_SUNW_syminfo:		return " SHT_SUNW_syminfo";
		case SHT_GNU_verdef:	      	return " Version definition section.  ";
		case SHT_GNU_verneed:	      	return " Version needs section.  ";
		case SHT_HISUNW:	      	return " Sun-specific high bound.  ";
		case SHT_LOPROC:	      	return " Start of processor-specific ";
		case SHT_HIPROC:	      	return " End of processor-specific ";
		case SHT_LOUSER:	      	return " Start of application-specific ";
		case SHT_HIUSER:	      	return " End of application-specific ";
		default: 			return " ";
	}
}
/*
char* case_type(Elf32_Word sh_type){
	switch(sh_type) {
		case SHT_NULL:	      		return " NULL ";
		case SHT_PROGBITS:	      	return " Program data ";
		case SHT_SYMTAB:	      	return " Symbol table ";
		case SHT_STRTAB:	      	return " String table ";
		case SHT_RELA:	      		return " Relocation entries with addends ";
		case SHT_HASH:	      		return " Symbol hash table ";
		case SHT_DYNAMIC:	      	return " Dynamic linking information ";
		case SHT_NOTE:	      		return " Notes ";
		case SHT_NOBITS:	      	return " Program space with no data (bss) ";
		case SHT_REL:	      		return " Relocation entries, no addends ";
		case SHT_SHLIB:	      		return " Reserved ";
		case SHT_DYNSYM:	      	return " Dynamic linker symbol table ";
		case SHT_INIT_ARRAY:	      	return " Array of constructors ";
		case SHT_FINI_ARRAY:	      	return " Array of destructors ";
		case SHT_PREINIT_ARRAY:	      	return " Array of pre-constructors ";
		case SHT_GROUP:	      		return " Section group ";
		case SHT_SYMTAB_SHNDX:	      	return " Extended section indeces ";
		case SHT_NUM:	      		return " Number of defined types.  ";
		case SHT_LOOS:	      		return " Start OS-specific.  ";
		case SHT_GNU_ATTRIBUTES:	return " Object attributes.  ";
		case SHT_GNU_HASH:	      	return " GNU-style hash table.  ";
		case SHT_GNU_LIBLIST:	      	return " Prelink library list ";
		case SHT_CHECKSUM:	      	return " Checksum for DSO content.  ";
		case SHT_LOSUNW:	     	return " Sun-specific low bound.  ";
		case SHT_SUNW_COMDAT:		return " SHT_SUNW_COMDAT ";
		case SHT_SUNW_syminfo:		return " SHT_SUNW_syminfo";
		case SHT_GNU_verdef:	      	return " Version definition section.  ";
		case SHT_GNU_verneed:	      	return " Version needs section.  ";
		case SHT_HISUNW:	      	return " Sun-specific high bound.  ";
		case SHT_LOPROC:	      	return " Start of processor-specific ";
		case SHT_HIPROC:	      	return " End of processor-specific ";
		case SHT_LOUSER:	      	return " Start of application-specific ";
		case SHT_HIUSER:	      	return " End of application-specific ";
		default: 			return " aucun";
	}
}*/
/************************************/

