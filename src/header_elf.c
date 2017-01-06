#include <stdlib.h>

#include "util.h"
#include "header_elf.h"

int display_header(Elf32_Ehdr *elf_header) {

    int i = 0;

    if (elf_header == NULL) {
        fprintf(stderr, "Header is NULL\n");
        return -1;
    }

    if (   elf_header->e_ident[EI_MAG0] != ELFMAG0
        || elf_header->e_ident[EI_MAG1] != ELFMAG1
        || elf_header->e_ident[EI_MAG2] != ELFMAG2
        || elf_header->e_ident[EI_MAG3] != ELFMAG3
        || elf_header->e_ident[EI_CLASS] != ELFCLASS32)
    {
        fprintf(stderr, "Affichage impossible: le fichier n'est pas de type elf 32bits\n");
        return -1;
    }

    // Start print header
    printf("╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                              \e[1;31mEn-tête ELF:\e[0m                                                 ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ Nombre magique:%-43s", "");
    for (i = 0; i < EI_NIDENT; i++) {
        printf("%2.2x ", elf_header->e_ident[i]);
    }
    printf("║\n╟────────────────────────────────────────────────────────┬──────────────────────────────────────────────────╢\n");

    printf("%-58s │ %-49s║\n", "║ Classe de fichier:", get_file_class(elf_header->e_ident[EI_CLASS]));

    printf("%-58s   │ %-49s║\n", "║ Données encodées:", get_data_encoding(elf_header->e_ident[EI_DATA]));

    printf("%-58s  │ %-49s║\n", "║ Version de l'en-tête ELF:", get_header_version(elf_header->e_ident[EI_VERSION]));

    printf("%-58s │ %-49s║\n", "║ Systeme d'exploitation:", get_operating_system(elf_header->e_ident[EI_OSABI]));

    printf("%-58s │ %-49d║\n", "║ Version de l'ABI :", elf_header->e_ident[EI_ABIVERSION]);

    printf("%-58s │ %-49s║\n", "║ Type de fichier objet:", get_object_file_type(elf_header->e_type));

    printf("%-58s │ %-49s║\n", "║ Architecture requise (machine):", get_architecture(elf_header->e_machine));

    printf("%-58s │ %-49s║\n", "║ Version des fichiers objets:", get_object_file_version(elf_header->e_version));

    printf("%-58s  │ ", "║ Adresse du point d'entrée:");
    if (elf_header->e_entry == 0) printf("Aucune adresse du point d'entrée assossicée (0x%x)║\n", elf_header->e_entry);
    else printf("%#-49x║\n", elf_header->e_entry);

    printf("%-58s   │ ", "║ Début des en-têtes de programme (bytes):");
    if (elf_header->e_phoff == 0) printf("%-49s ║\n","0 (Aucune table d'en-tête de programme)");
    else printf("%-49d║\n", elf_header->e_phoff);

    printf("%-58s   │ ", "║ Début des en-têtes de section (bytes):");
    if (elf_header->e_shoff == 0) printf("%-49s ║\n","0 (Aucune table d'en-tête de section)");
    else printf("%-49d║\n", elf_header->e_shoff);

    printf("%-58s │ %#-49x║\n", "║ Flags:", elf_header->e_flags);

    printf("%-58s  │ %-49d║\n", "║ Taille de l'en-tête ELF (bytes):", elf_header->e_ehsize);

    printf("%-58s  │ %-49d║\n","║ Taille de l'en-tête du programme (bytes):", elf_header->e_phentsize);

    printf("%-58s  │ ","║ Nombre d'en-têtes du programme:");
    if (elf_header->e_phnum == 0) printf("%-49s ║\n","0 (Aucune table d'en-tête de programme)");
    else printf("%-49d║\n", elf_header->e_phnum);

    printf("%-58s  │ %-49d║\n","║ Taille des en-têtes de section (bytes):", elf_header->e_shentsize);

    printf("%-58s  │ ","║ Nombre d'entêtes de section:");
    if (elf_header->e_shnum == 0) printf("%-49s ║\n","0 (Aucune table d'en-tête de section)");
    else printf("%-49d║\n", elf_header->e_shnum);

    printf("%-58s  │ %-49d║\n", "║ Table d'indexes d'en-tête de section:", elf_header->e_shstrndx);
    // End print header
    printf("╚════════════════════════════════════════════════════════╧══════════════════════════════════════════════════╝\n");
    return 1;
 }

int read_header(ELF_STRUCT* elf_struct) {

    int reverse_needed;
    int header_size = sizeof(Elf32_Ehdr);

    fseek(elf_struct->elf_file, 0, SEEK_SET);

    if ( fread(elf_struct->elf_header, header_size, 1, elf_struct->elf_file) == -1 ) {
        return -1;
    }

    // big endian / little endian
    reverse_needed = need_reverse( elf_struct->elf_header->e_ident[EI_DATA] );

    if (reverse_needed) {

        elf_struct->elf_header->e_type = reverse_2(elf_struct->elf_header->e_type);
        elf_struct->elf_header->e_machine = reverse_2(elf_struct->elf_header->e_machine);
        elf_struct->elf_header->e_version = reverse_4(elf_struct->elf_header->e_version);
        elf_struct->elf_header->e_entry = reverse_4(elf_struct->elf_header->e_entry);
        elf_struct->elf_header->e_phoff = reverse_4(elf_struct->elf_header->e_phoff);
        elf_struct->elf_header->e_shoff = reverse_4(elf_struct->elf_header->e_shoff);
        elf_struct->elf_header->e_flags = reverse_4(elf_struct->elf_header->e_flags);
        elf_struct->elf_header->e_ehsize = reverse_2(elf_struct->elf_header->e_ehsize);
        elf_struct->elf_header->e_phentsize = reverse_2(elf_struct->elf_header->e_phentsize);
        elf_struct->elf_header->e_phnum = reverse_2(elf_struct->elf_header->e_phnum);
        elf_struct->elf_header->e_shentsize = reverse_2(elf_struct->elf_header->e_shentsize);
        elf_struct->elf_header->e_shnum = reverse_2(elf_struct->elf_header->e_shnum);
        elf_struct->elf_header->e_shstrndx = reverse_2(elf_struct->elf_header->e_shstrndx);

    }

    return 1;
}

char* get_file_class(unsigned char ei_class) {

    switch (ei_class) {

        case ELFCLASSNONE:  return "Invalid Class";
        case ELFCLASS32:    return "32-bit objects";
        case ELFCLASS64:    return "64-bit objects";
        default:            return "Unknown";
    }

}

char* get_data_encoding(unsigned char ei_data) {

    switch (ei_data) {

        case ELFDATANONE:   return "Invalid data encoding";
        case ELFDATA2LSB:   return "2's complement values, little endian";
        case ELFDATA2MSB:   return "2's complement, big endian";
        default:            return "Unknown";

    }

}

char* get_header_version(unsigned char ei_version) {

    switch (ei_version) {

        case EV_NONE:       return "0 (invalid version)";
        case EV_CURRENT:    return "1 (current version)";
        default:            return "Unknown";

    }

}

char* get_operating_system(unsigned char ei_osabi) {

    switch(ei_osabi) {

        case ELFOSABI_SYSV:         return "Unix System V";
        case ELFOSABI_HPUX:         return "HP-UX";
        case ELFOSABI_NETBSD:       return "NetBSD";
        case ELFOSABI_LINUX:        return "Linux";
        case ELFOSABI_SOLARIS:      return "Solaris";
        case ELFOSABI_IRIX:         return "Irix";
        case ELFOSABI_FREEBSD:      return "FreeBSD";
        case ELFOSABI_TRU64:        return "TRU64 Unix";
        case ELFOSABI_ARM:          return "ARM architecture";
        case ELFOSABI_STANDALONE:   return "Stand-Alone (embedded)";
        default:                    return "Unknown";

    }

}

char* get_object_file_type(Elf64_Half e_type) {

    switch(e_type) {

        case ET_NONE:   return "No File Type";
        case ET_REL:    return "Relocatable File";
        case ET_EXEC:   return "Executable File";
        case ET_DYN:    return "Shared Object File";
        case ET_CORE:   return "Core File";
        case ET_LOPROC: return "Processor Specific";
        case ET_HIPROC: return "Processor Specific";
        default:        return "Unknown";

    }

}

char* get_architecture(Elf64_Half e_machine) {

    switch(e_machine) {

        case EM_NONE:           return "No machine";
        case EM_SPARC:          return "Sun SPARC";
        case EM_386:            return "Intel 80386";
        case EM_68K:            return "Motorola 68000";
        case EM_88K:            return "Motorola 88000";
        case EM_860:            return "Intel i860";
        case EM_MIPS:           return "MIPS RS3000 Big-Endian";
        case EM_S370:           return "IBM System/370 Processor";
        case EM_MIPS_RS3_LE:    return "MIPS RS3000 Little-Endian";
        case EM_PARISC:         return "PA-RISC";
        case EM_VPP500:         return "Fujitsu VPP500";
        case EM_SPARC32PLUS:    return "Sun SPARC 32+";
        case EM_960:            return "Intel 80960";
        case EM_PPC:            return "PowerPC";
        case EM_PPC64:          return "64-bit PowerPC";
        case EM_S390:           return "IBM System/390 Processor";
        case EM_V800:           return "NEX V800";
        case EM_FR20:           return "Fujitsu FR20";
        case EM_RH32:           return "TRW RH-32";
        case EM_RCE:            return "Motorola RCE";
        case EM_ARM:            return "Advanced RISC Machines ARM";
        case EM_ALPHA:          return "Digital Alpha";
        case EM_SH:             return "Hitachi SH";
        case EM_SPARCV9:        return "Sun SPARC V9 (64-bit)";
        case EM_TRICORE:        return "Siemens Tricore embedded processor";
        case EM_ARC:            return "Argonaut RISC Core";
        case EM_H8_300:         return "Hitachi H8/300";
        case EM_H8_300H:        return "Hitachi H8/300H";
        case EM_H8S:            return "Hitachi H8S";
        case EM_H8_500:         return "Hitachi H8/500";
        case EM_IA_64:          return "Intel IA64";
        case EM_MIPS_X:         return "Stanford MIPS-X";
        case EM_COLDFIRE:       return "Motorola ColdFire";
        case EM_68HC12:         return "Motorola M68HC12";
        case EM_MMA:            return "Fujitsu MMA Mulimedia Accelerator";
        case EM_PCP:            return "Siemens PCP";
        case EM_NCPU:           return "Sony nCPU embedded RISC processor";
        case EM_NDR1:           return "Denso NDR1 microprocessor";
        case EM_STARCORE:       return "Motorola Star*Core processor";
        case EM_ME16:           return "Toyota ME16 processor";
        case EM_ST100:          return "STMicroelectronics ST100 processor";
        case EM_TINYJ:          return "Advanced Logic Corp. TinyJ";
        case EM_X86_64:         return "AMDs x86-64 architecture";
        case EM_PDSP:           return "Sony DSP Processor";
        case EM_FX66:           return "Siemens FX66 microcontroller";
        case EM_ST9PLUS:        return "STMicroelectronics ST9+8/16 bit";
        case EM_ST7:            return "STMicroelectronics ST7 8-bit";
        case EM_68HC16:         return "Motorola MC68HC16 Microcontroller";
        case EM_68HC11:         return "Motorola MC68HC11 Microcontroller";
        case EM_68HC08:         return "Motorola MC68HC08 Microcontroller";
        case EM_68HC05:         return "Motorola MC68HC05 Microcontroller";
        case EM_SVX:            return "Silicon Graphics SVx";
        case EM_ST19:           return "STMicroelectronics ST19 8-bit";
        case EM_VAX:            return "Digital VAX";
        case EM_CRIS:           return "Axis Communications 32-bit";
        case EM_JAVELIN:        return "Infineon Technologies 32-bit";
        case EM_FIREPATH:       return "Element 14 64-bit DSP Processor";
        case EM_ZSP:            return "LSI Logic 16-bit DSP Processor";
        case EM_MMIX:           return "Donald Knuth's educational";
        case EM_HUANY:          return "Harvard University";
        case EM_PRISM:          return "SiTera Prism";
        case EM_AVR:            return "Atmel AVR 8-bit microcontroller";
        case EM_FR30:           return "Fujitsu FR30";
        case EM_D10V:           return "Mitsubishi D10V";
        case EM_D30V:           return "Mitsubishi D30V";
        case EM_V850:           return "NEC v850";
        case EM_M32R:           return "Mitsubishi M32R";
        case EM_MN10300:        return "Matsushita MN10300";
        case EM_MN10200:        return "Matsushita MN10200";
        case EM_PJ:             return "picoJava";
        case EM_OPENRISC:       return "OpenRISC 32-bit embedded processor";
        case EM_ARC_A5:         return "ARC Cores Tangent-A5";
        case EM_XTENSA:         return "Tensilica Xtensa architecture";
        default:                return "Unknown";

    }

}

char* get_object_file_version(Elf32_Word e_version) {

    switch (e_version) {

        case EV_NONE:       return "0 (invalid version)";
        case EV_CURRENT:    return "1 (current version)";
        default:            return "Unknown";

    }

}
