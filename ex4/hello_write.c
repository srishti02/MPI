// "hello_write.c" - A C code that demonstrates writing data to a file 
// at a specific position
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int fd;
    off_t offset;

    if (1 < argc)
	offset = atoi(argv[1]);

    fd = open("bonjour.txt",O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);
    lseek(fd, offset, SEEK_SET);
    write(fd,"Hello world!",strlen("Hello world!"));
    close(fd);
    
    return 0;
}
    
