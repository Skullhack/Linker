CC=gcc
CFLAGS=-Wall -Werror

all: linker program_header

linker : linker.o init.o header_elf.o util.o section_header.o table_symbole.o
	$(CC) -o $@ $^

program_header: program_header.o init.o header_elf.o util.o section_header.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

linker.o: linker.h global_struct.h init.h header_elf.h util.h section_header.h
gestion_elf.o: init.h global_struct.h header_elf.h section_header.h
header_elf.o: header_elf.h global_struct.h
section_header.o: section_header.h global_struct.h util.h
table_symbole.o: table_symbole.h global_struct.h
util.o: util.h

clean:
	rm *.o program_header linker
