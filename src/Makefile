CC=gcc
CFLAGS=-Wall -Werror

program_header: program_header.o gestion_elf.o header_elf.o util.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

gestion_elf.o: gestion_elf.h struct_elf.h
header_elf.o: header_elf.h struct_elf.h
util.o: util.h

clean:
	rm *.o program_header
