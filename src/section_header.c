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
	printf("╔═════════════════════════════════════════════════════════════════════════════════════════╗\n");
	printf("║                                  \e[1;31mEn-tête de section :\e[0m                                   ║\n");
	printf("╟─────┬───────────────────┬───────────────┬─────────┬───────┬───────┬───┬────┬───┬────┬───╢\n");
	printf("║%-5s│%-19s│%-15s│%-9s│%-7s│%-7s│%-3s│%-4s│%-3s│%-4s│%-3s║\n","[Nr]","Nom","Type","Adr","Offset","Taille","ES","Flg","LN","Inf","Al");
	printf("╟─────┼───────────────────┼───────────────┼─────────┼───────┼───────┼───┼────┼───┼────┼───╢\n");
	for(int i = 0; i <  elf->elf_header->e_shnum; i++){
		printf("║  %-3d",i);
		printf("│%-19s",get_name(elf,i));
		printf("│%-15s",case_type(elf->a_shdr[i].sh_type));
		printf("│%-9.8x",elf->a_shdr[i].sh_addr);
		printf("│%-7.6x",elf->a_shdr[i].sh_offset);
		printf("│%-7.6x",elf->a_shdr[i].sh_size);
		printf("│%-3.2x",elf->a_shdr[i].sh_entsize);
		case_flags(elf->a_shdr[i].sh_flags);

		printf("│%3x",elf->a_shdr[i].sh_link);
		printf("│%4x",elf->a_shdr[i].sh_info);
		printf("│%3x║\n",elf->a_shdr[i].sh_addralign);
	}
	printf("╚═════╧═══════════════════╧═══════════════╧═════════╧═══════╧═══════╧═══╧════╧═══╧════╧═══╝\n");
	printf("\nClés des Flags :");
	printf("\n  W (écriture), A (allocation), X(exécution), M (fusion)");
	printf("\n  S (chaînes), I (info), L (ordre des liens), G (groupe)");
	printf("\n  T (TLS), E (exclu), x (inconnu), o (spécifique à l'OS)");
	printf("\n  O (traitement additionel requis pour l'OS)");
	printf("\n  p (spécifique au processeur), N (OS non conforme)");
	printf("\n  C (compressé)\n");
}

/************************************/
void case_flags(Elf32_Word flag) {

	int tmp = flag;
	int cpt = 0;
	int result = 0;
	char str[3]="";
  printf("│");
	while(tmp!=0){

		if(tmp&1){
			result= 1<<cpt;
				 switch(result) {
						case SHF_WRITE:	    				strcat(str,"W");
								break;
						case SHF_ALLOC:	     				strcat(str,"A");
								break;
						case SHF_EXECINSTR:					strcat(str,"X");
								break;
						case SHF_MERGE:	     				strcat(str,"M");
								break;
						case SHF_STRINGS:	   				strcat(str,"S");
								break;
						case SHF_INFO_LINK:	 				strcat(str,"I");
								break;
						case SHF_LINK_ORDER	: 			strcat(str,"L");
								break;
						case SHF_OS_NONCONFORMING: 	strcat(str,"N");
								break;
						case SHF_GROUP:	     				strcat(str,"G");
								break;
						case SHF_TLS:		     				strcat(str,"T");
								break;
						case SHF_COMPRESSED:	  		strcat(str,"C");
								break;
						case SHF_MASKOS:	     			strcat(str,"o");
								break;
						case SHF_MASKPROC:	    		strcat(str,"p");
								break;
						case SHF_ORDERED:	      		strcat(str,"O");
								break;
						case SHF_EXCLUDE:	      		strcat(str,"E");
								break;
						default: 										strcat(str,"x");
		  }
		}
		tmp= tmp>>1;
		cpt++;
	}
	printf("%-4s",str);
 
}

/************************************/
char* case_type(Elf32_Word sh_type){
	switch(sh_type) {
		case SHT_NULL:	      		return " NULL ";
		case SHT_PROGBITS:	      return " PROGBITS";
		case SHT_SYMTAB:	      	return " SYMTAB";
		case SHT_STRTAB:	      	return " STRTAB";
		case SHT_RELA:	      		return " RELA";
		case SHT_HASH:	      		return " HASH";
		case SHT_DYNAMIC:	      	return " DYNAMIC";
		case SHT_NOTE:	      		return " NOTE";
		case SHT_NOBITS:	      	return " NOBITS";
		case SHT_REL:	      			return " REL";
		case SHT_SHLIB:	      		return " SHLIB";
		case SHT_DYNSYM:	      	return " DYNSYM";
		case SHT_INIT_ARRAY:	    return " INIT_ARRAY";
		case SHT_FINI_ARRAY:	    return " FINI_ARRAY";
		case SHT_PREINIT_ARRAY:	  return " PREINIT_ARRAY";
		case SHT_GROUP:	      		return " GROUP";
		case SHT_SYMTAB_SHNDX:	  return " SYMTAB_SHNDX";
		case SHT_NUM:	      			return " NUM";
		case SHT_LOOS:	      		return " LOOS";
		case SHT_GNU_ATTRIBUTES:	return " GNU_ATTRIBUTES";
		case SHT_GNU_HASH:	      return " GNU_HASH";
		case SHT_GNU_LIBLIST:	    return " GNU_LIBLIST";
		case SHT_CHECKSUM:	      return " CHECKSUM";
		case SHT_LOSUNW:	     		return " LOSUNW";
		case SHT_SUNW_COMDAT:			return " SUNW_COMDAT";
		case SHT_SUNW_syminfo:		return " SUNW_syminfo";
		case SHT_GNU_verdef:	    return " GNU_verdef";
		case SHT_GNU_verneed:	    return " GNU_verneed";
		case SHT_HISUNW:	      	return " HISUNW";
		case SHT_LOPROC:	      	return " LOPROC";
		case SHT_HIPROC:	      	return " HIPROC";
		case SHT_LOUSER:	      	return " LOUSER";
		case SHT_HIUSER:	      	return " HIUSER";
		default: 									return " ";
	}
}
/************************************/

