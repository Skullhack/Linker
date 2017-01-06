CC=gcc
CFLAGS=-Wall -Werror

all: linker program_header program_section_header program_table_symbole

linker : linker.o init.o header_elf.o util.o section_header.o table_symbole.o
	$(CC) -o $@ $^

program_header: program_header.o init.o header_elf.o util.o section_header.o table_symbole.o
	$(CC) -o $@ $^

program_section_header: program_section_header.o init.o header_elf.o section_header.o util.o table_symbole.o reimplantation.o
	$(CC) -o $@ $^

program_table_symbole: program_table_symbole.o init.o header_elf.o util.o section_header.o table_symbole.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

linker.o: linker.h global_struct.h init.h header_elf.h util.h section_header.h
gestion_elf.o: init.h global_struct.h header_elf.h section_header.h
header_elf.o: header_elf.h global_struct.h
section_header.o: section_header.h global_struct.h util.h
table_symbole.o: table_symbole.h global_struct.h util.h
util.o: util.h
reimplantation.o : section_header.h reimplantation.h  global_struct.h util.h
clean:
	rm *.o linker program_header program_section_header program_table_symbole
