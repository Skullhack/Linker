#ifndef _reimpl_

#define _reimpl_

#include <elf.h>

#include "section_header.h"
#include "util.h"
#include "global_struct.h"

void tab_Reimplantation(ELF_STRUCT * elf);
void Affichage_Rel(ELF_STRUCT * elf);
void Affichage_Rela(ELF_STRUCT * elf);
#endif
