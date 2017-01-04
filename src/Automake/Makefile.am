ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS=-Wall -Werror -g
AM_CFLAGS+=-D DEBUG
AM_CFLAGS+=-D WARNING
# Uncomment if performance when running with -DDEBUG is an issue
# Warning, if uncommented, issuing calls to debug functions during options
# parsing might result in debug flag incorrectly set to 0 for some files
#AM_CFLAGS+=-D CACHE_DEBUG_FLAG



SUBDIRS=
SUBDIRS+=. Examples_fusion

bin_PROGRAMS=
bin_PROGRAMS+=program_header
bin_PROGRAMS+=Options_and_debug_example


COMMON=debug.h debug.c util.h util.c
INCLUDE_ELF_STRUCT=elf_struct.h elf_gestion.c elf_gestion.h elf_header.h elf_header.c elf_section_header.h elf_section_header.c elf_symbol_table.h elf_symbol_table.c elf_relocate.c elf_relocate.h elf_section_content.c elf_section_content.h

ETAPE_UN= $(INCLUDE_ELF_STRUCT)

EXTRA_DIST=filtre.pl
Options_and_debug_example_SOURCES=$(COMMON) Options_and_debug_example.c
program_header_SOURCES=$(ETAPE_UN) $(COMMON) program_header.c

EXTRA_DIST+=

