CC=gcc
CFLAGS=-Wall -Werror

all: linker program_header program_section_header program_section_content program_table_symbole program_reimplantation

linker : linker.o init.o header_elf.o util.o section_header.o section_elf.o table_symbole.o reimplantation.o
	$(CC) -o $@ $^

program_header: program_header.o init.o header_elf.o util.o section_header.o section_elf.o table_symbole.o reimplantation.o
	$(CC) -o $@ $^

program_section_header: program_section_header.o init.o header_elf.o section_header.o section_elf.o util.o table_symbole.o reimplantation.o
	$(CC) -o $@ $^

program_section_content: program_section_content.o init.o header_elf.o section_header.o section_elf.o util.o table_symbole.o reimplantation.o
	$(CC) -o $@ $^

program_table_symbole: program_table_symbole.o init.o header_elf.o util.o section_header.o section_elf.o table_symbole.o reimplantation.o
	$(CC) -o $@ $^

program_reimplantation: program_reimplantation.o init.o header_elf.o util.o section_header.o section_elf.o table_symbole.o reimplantation.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

linker.o: linker.h global_struct.h init.h header_elf.h util.h section_header.h
reimplantation.o : section_header.h reimplantation.h  global_struct.h util.h
init.o: init.h global_struct.h header_elf.h section_header.h section_elf.h table_symbole.h reimplantation.h
header_elf.o: header_elf.h global_struct.h
section_header.o: section_header.h global_struct.h util.h
section_elf.o: section_elf.h global_struct.h util.h
table_symbole.o: table_symbole.h global_struct.h util.h
util.o: util.h
fusion_reimp.o: util.h

clean:
	rm *.o linker program_header program_section_header program_section_content program_table_symbole program_reimplantation
