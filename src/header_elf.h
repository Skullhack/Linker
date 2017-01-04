/**
  * @file elf_header.h
  *	@date 8 Jan 2016
  *	@brief Module used to read and display an ELF header.
  */

#ifndef __ELF_HEADER_H__
#define __ELF_HEADER_H__

#include <elf.h>
#include <stdio.h>
#include "struct_elf.h"
/**
  * @brief Displays information from an ELF header.
  *
  *	Prints in that particular order:
  *	- Magic Number
  *	- File Class
  *	- Data Encoding
  *	- Elf header Version Number
  *	- Operating system and ABI
  *	- ABI Version
  *	- Object file type
  *	- Required architecture
  *	- Object file version
  *	- Entry point (starting the process)
  *	- header table's file offset
  *	- section table's file offset
  *	- flags
  *	- Header's size
  *	- Size of one entry in the file's program header table
  *	- Number of entries in the file's program header table
  *	- Size of one entry in the file's section header table
  *	- Number of entries in the file's section header table
  *	- section header table index of the entry associated
  *	  with the section name string table
  * @param elf_header The header to be displayed.
  *	@return -1 if not a 32bits ELF header.
  * @return 1 otherwhise.
  */
int display_header(Elf32_Ehdr *elf_header);

/**
  * @brief Gets the human-readeable file's class, or capacity.
  *
  *	@param ei_class The Elf32_Ehdr->e_ident[] member identifying the file's class, or capacity.
  * @return String containing the human-readeable file's class, or capacity.
  */
char* get_file_class(unsigned char ei_class);

/**
  * @brief Gets the human-readeable data encoding.
  *
  *	@param ei_data The Elf32_Ehdr->e_ident[] member identifying the data encoding.
  * @return String containing the human-readeable data encoding.
  */
char* get_data_encoding(unsigned char ei_data);

/**
  * @brief Gets the human-readeable ELF header version number.
  *
  *	@param ei_version The Elf32_Ehdr->e_ident[] member identifying the ELF header version number.
  * @return String containing the human-readeable ELF header version number.
  */
char* get_header_version(unsigned char ei_version);


/**
  * @brief Gets the human-readeable operating system.
  *
  *	@param ei_osabi The Elf32_Ehdr->e_ident[] member identifying the operating system.
  * @return String containing the human-readeable operating system.
  */
char* get_operating_system(unsigned char ei_osabi);

/**
  * @brief Gets the human-readeable object file type.
  *
  *	@param e_type The Elf32_Ehdr member identifying the object file type.
  * @return String containing the human-readeable object file type.
  */
char* get_object_file_type(Elf64_Half e_type);

/**
  * @brief Gets the human-readeable required architecture for an individual file.
  *
  *	@param e_machine The Elf32_Ehdr member identifying the required architecture for an individual file.
  * @return String containing the human-readeable required architecture for an individual file.
  */
char* get_architecture(Elf64_Half e_machine);

/**
  * @brief Gets the human-readeable object file version.
  *
  *	@param e_version The Elf32_Ehdr member identifying the object file version.
  * @return String containing the human-readeable object file version.
  */
char* get_object_file_version(Elf32_Word e_version);

/**
  * @brief Reads data of an ELF header from a given file and puts the data into a given header struct.
  *
  * @param f The file we're reading from. The file must have been opened beforehand.
  * @param elf_header The header struct to be filled. Memory must have been allocated beforehand.
  * @return -1 if fread error.
  * @return  1 otherwise.
  */
int read_header(ELF_STRUCT* elf_struct);

#endif
