#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "common.h"

#define FIFO_NAME "myfifo"

int main(void) {
    char s[300];
    int num, fd;
int na = nada ();
na ++;

    mknod(FIFO_NAME, S_IFIFO | 0666, 0);

    printf("waiting for readers...\n");
    fd = open(FIFO_NAME, O_WRONLY);
    printf("got a reader--type some stuff\n");

    while (1) {
        char *r = fgets(s, 300, stdin);
        if (r == NULL){
            if (feof(stdin)){
                break;
            }else {
                printf("error en fgets\n");
                return 1;
            }
        }
        if ((num = write(fd, s, strlen(s))) == -1)
            perror("write");
        else
            printf("writer: wrote %d bytes\n", num);
    }
    return 0;
}
