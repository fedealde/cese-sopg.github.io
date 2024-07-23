#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "myfifo"

int main(void) {
    char s[300];
    int num, fd;

    mknod(FIFO_NAME, S_IFIFO | 0666, 0); 
    //https://man7.org/linux/man-pages/man2/mknod.2.html
    //https://man7.org/linux/man-pages/man7/inode.7.html 
    // | ABCD, A:tipo de archivo, B propietario, C: grupo, D: otros
    //  4/2/1, se suman, donde 1 exec 2 write 4 read (ej: 3 = write y exec pero no read)

    printf("waiting for writers...\n");
    fd = open(FIFO_NAME, O_RDONLY);
    printf("got a writer\n");

    do {
        if ((num = read(fd, s, 300)) == -1)
            perror("read");
        else {
            s[num] = '\0';
            printf("reader: read %d bytes: \"%s\"\n", num, s);
        }
    } while (num > 0);

    return 0;
}
