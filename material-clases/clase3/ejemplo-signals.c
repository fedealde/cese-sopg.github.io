#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void handler(int sig) {
    write(1, "se presiono ctrl+c!!\n", 21);
}

int main(void) {
    struct sigaction sa;

    sa.sa_handler = handler;
    sa.sa_flags = 0; // SA_RESTART;
    sigemptyset(&sa.sa_mask);

    int r = sigaction(SIGINT, &sa, NULL);
    if (r == -1) {
        perror("sigaction");
        exit(1);
}


    char s[200];
    if (fgets(s, sizeof(s), stdin) == NULL) {
        perror("error en fgets");
    } else {
        printf("ingresaste: %s\n", s);
    }

    return 0;
}
