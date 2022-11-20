#include <stdio.h> //printf
#include <elf.h> //elf headers
#include <fcntl.h> //open
#include <string.h> //strlen
#include <stdlib.h> //exit
#include <sys/mman.h> //mmap
#include <sys/types.h> //fstat
#include <sys/stat.h> //fstat
#include <unistd.h> //write

#define FILE_PATH "./material/hello_world"

void critical_exit(int ret, char *err_str)
{
    write(2, "str", strlen(err_str));
    exit(ret);
}

void print_bytes_of_elf_header(char *file)
{
    int i = 0;
    printf("Bytes of elf header: ");
    while (i < 60)
    {
        printf("%#010x ", *(file + i));
        i++;
    }
    printf("\n");
}

void print_elf_info(Elf64_Ehdr* elf_hdr)
{
    printf("ELF header size: %lu\n", (unsigned long)elf_hdr->e_ehsize);
    printf("Start of section header: %lu\n", elf_hdr->e_shoff);
}

void print_section_info(Elf64_Shdr* sections, int index)
{
    printf("Section name: %u\n", sections[index].sh_name);
    printf("Section type: %u\n", sections[index].sh_type);
    printf("Section flags: %lu\n", sections[index].sh_flags);
    printf("Section virtual addr at execution: %lu\n", sections[index].sh_addr);
    printf("Section file offset: %lu\n", sections[index].sh_offset);
    printf("Section size in bytes: %lu\n", sections[index].sh_size);
    printf("Link to another section: %u\n", sections[index].sh_link);
    printf("Additional section information: %u\n", sections[index].sh_info);
    printf("Section alignment: %lu\n", sections[index].sh_addralign);
    printf("Entry size if section holds table: %lu\n", sections[index].sh_entsize);
}

Elf64_Sym *find_symtab(Elf64_Ehdr* elf_hdr, Elf64_Shdr* sections, char *file, int *symtab_index)
{
    Elf64_Sym *symtabs;
    int i = 0;
    while (i < elf_hdr->e_shnum)
    {
//        printf("Section type: %u\n", sections[i].sh_type);
        if (sections[i].sh_type == SHT_SYMTAB)
        {
            symtabs = (Elf64_Sym *)((char *)file + sections[i].sh_offset);
            printf("Has found symtab.\n");
            *symtab_index = i;
        }
        i++;
    }
    return symtabs;
}

void process_the_file(char *file)
{
//    print_bytes_of_elf_header(file);
    Elf64_Ehdr* elf_hdr = (Elf64_Ehdr*)file;
//    print_elf_info(elf_hdr);
    Elf64_Shdr* sections = (Elf64_Shdr*)(file + elf_hdr->e_shoff);
    int symtab_index = 0;
    Elf64_Sym *symtabs = find_symtab(elf_hdr, sections, file, &symtab_index);
//    print_section_info(sections, symtab_index);
    int k = 0;
    printf("Names of symtabs: \n");
    while ((k * sections[symtab_index].sh_entsize) < sections[symtab_index].sh_size)
    {
        unsigned int index = symtabs[k].st_name; //?
        char *str = ((elf_hdr->e_shstrndx)*elf_hdr->e_shentsize) + index;
        printf("Name %d: %s\n", k, str);
        k++;
    }
}

int main(int argc, char **argv)
{
    const char *filepath = FILE_PATH;
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        critical_exit(1, "Can't open file.\n");
    }
    struct stat statbuf;
    int res = fstat(fd, &statbuf);
    if (res < 0) {
        critical_exit(1, "Can't open file.\n");
    }
    char *file = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(file == MAP_FAILED) {
        critical_exit(3, "Mapping Failed\n");
    }
    close(fd);

    process_the_file(file);

    res = munmap(file, statbuf.st_size);
    if(res != 0) {
        critical_exit(4, "UnMapping Failed\n");
    }
    return 0;
}
