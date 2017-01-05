CC=gcc
CFLAGS=-Wall -Werror

all: linker program_header

linker : linker.o gestion_elf.o header_elf.o util.o section_header.o
	$(CC) -o $@ $^

program_header: program_header.o gestion_elf.o header_elf.o util.o section_header.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

linker.o: linker.h struct_elf.h gestion_elf.h header_elf.h util.h section_header.h
gestion_elf.o: gestion_elf.h struct_elf.h header_elf.h section_header.h
header_elf.o: header_elf.h struct_elf.h
section_header.o: struct_elf.h util.h
util.o: util.h

clean:
	rm *.o program_header linker
