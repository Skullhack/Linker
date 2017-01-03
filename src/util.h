/** 
  * @file util.h
  * @date 9 Jan 2016
  * @brief Utilitaries.
  */

/*
ELF Loader - chargeur/implanteur d'exécutables au format ELF à but pédagogique
Copyright (C) 2012 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/

#ifndef __UTIL_H__
#define __UTIL_H__

#include <elf.h>
#include <ctype.h>
#include <stdio.h>
#include "elf_struct.h"
#include "elf_gestion.h"

#define reverse_2(x) ((((x)&0xFF)<<8)|(((x)>>8)&0xFF))
#define reverse_4(x) ((((x)&0xFF)<<24)|((((x)>>8)&0xFF)<<16)|\
						((((x)>>16)&0xFF)<<8)|(((x)>>24)&0xFF))
#define min(x,y) ((x)<(y)?(x):(y))

/** 
  * @brief Tells us if big endian or little endian. 
  *
  * @return 1 if big endian.
  * @return 0 if little endian.
  */
int is_big_endian();

/**
  * @brief Tells us if a given string is a number.
  *
  * @param number The string to be looked at.
  * @return 1 if the string is a valid number (negative or positive).
  * @return 0 otherwise.
  */
int is_number(char *number);

/**
  * @brief Close a file then free everything given in the parameters.
  *
  * @param f The file to close.
  * @param count The number of elements to free.
  * @param ... The elements to free.
  * @return The number of elements freed.
  */
int fclose_free_everything(FILE *f, int count, ...);

/**
  * @brief Tells us if we need to reverse the bytes.
  *  
  * If we're dealing with a little endian architecture while using a big endian architecture
  * Then we should reverse the bytes. Same goes for big endian while using little endian.
  * @param ei_data The type of the data we're dealing with (i.e big or little endian). 1 = little endian, 2 = big endian.
  * @return 1 if we need to reverse the bytes.
  * @return 0 otherwise.
  */
int this_needs_reverse(unsigned char ei_data);

void print_usage(char* program_name);
int error_elf_struct(ELF_STRUCT* elf_struct, char *message);

#endif
