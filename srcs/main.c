#include "stdio.h" //printf
#include "elf.h" //elf headers
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

void process_the_file(char *file)
{
    char *tmp = file;
    int i = 0;
    printf("Bytes of elf header: ");
    while (i < 60)
    {
        printf("%#010x ", *(tmp + i));
        i++;
    }
    printf("\n");
    Elf64_Ehdr* ehdr = (Elf64_Ehdr*)tmp;
    printf("ELF header size: %lu\n", (unsigned long)ehdr->e_ehsize);
    printf("Start of section header: %lu\n", ehdr->e_shoff);
    Elf64_Shdr* section_header = (Elf64_Shdr*)(tmp + ehdr->e_shoff);

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
