#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "common.h"


#define FIFO_NAME "myfifo"

int main(void) {
    char s[300];
    int num, fd;
int na = nada ();
na ++;

    mknod(FIFO_NAME, S_IFIFO | 0666, 0);

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
