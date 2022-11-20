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

void critical(int ret,char *err_str)
{
    write(2, "str", strlen(err_str));
    exit(ret);
}

int process_the_file(char *file)
{

    return 0;
}

int main(int argc, char **argv)
{
    const char *filepath = FILE_PATH;
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        critical(1, "Can't open file.\n");
    }
    struct stat statbuf;
    int res = fstat(fd, &statbuf);
    if (res < 0) {
        critical(1, "Can't open file.\n");
    }
    char *file = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(file == MAP_FAILED) {
        critical(3, "Mapping Failed\n");
    }
    close(fd);
    //make all job here
    process_the_file(file);

    res = munmap(file, statbuf.st_size);
    if(res != 0) {
        critical(4, "UnMapping Failed\n");
    }
    return 0;
}
