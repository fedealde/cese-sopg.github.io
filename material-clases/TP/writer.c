#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FIFO_NAME "myfifo"
#define FIFO_MSG_SIZE 21
#define FIFO_MSG_TO_SEND_SIZE 27
#define FIFO_SIGN_TO_SEND_SIZE 7
#define _RW_ALL 0666

// Variables globales y constantes
int fifo_fd;
const char *sign_header = "SIGN: \0";
const char *sign_usr1 = "1\0";
const char *sign_usr2 = "2\0";

// Manejador de signals
void sig_handler(int sig) {

    switch (sig) {
    case SIGINT:
        write(1, "\n\nCtrl+C pressed. SIGINT handler executing...\n",
              strlen("\n\nCtrl+C pressed. SIGINT handler executing...\n"));
        exit(EXIT_FAILURE);
        break;

    case SIGPIPE:
        write(1, "\nSIGPIPE received...\n", strlen("\nSIGPIPE received...\n"));
        exit(EXIT_FAILURE);
        break;

    default:
        write(1, "\nANOTHER SIGNAL\n", strlen("\nANOTHER SIGNAL\n"));
        exit(EXIT_FAILURE);
        break;
    }
}

void sigusr_handler(int sig) {

    ssize_t ret;
    char aux_handler[FIFO_SIGN_TO_SEND_SIZE];
    bzero(aux_handler, sizeof(aux_handler));
    strcat(aux_handler, sign_header);

    if (SIGUSR1 == sig) {
        strcat(aux_handler, sign_usr1);
    } else {
        strcat(aux_handler, sign_usr2);
    }

    ret = write(fifo_fd, aux_handler, strlen(aux_handler));

    if (-1 == ret) {
        perror("write(FIFO_NAME)sigusr_handler");
        exit(EXIT_FAILURE);
    } else {
        printf("-->>Writer: %ld bytes wroten in FIFO_NAME from "
               "sigusr_handler due to %s: \"%s\"\n",
               ret, strsignal(sig), aux_handler);
    }
}

int main(void) {

    const char *data_header = "DATA: \0}";
    char msg[FIFO_MSG_SIZE], aux[FIFO_MSG_TO_SEND_SIZE];

    ssize_t ret;
    struct sigaction sa;

    // Presentacion del proseso
    printf("Writer is running. PID: %d\n", getpid());
    write(STDOUT_FILENO, "Creating FIFO_NAME...\n",
          strlen("Creating FIFO_NAME...\n"));

    // Creacion y apertura de la fifo
    if (-1 == mknod(FIFO_NAME, S_IFIFO | _RW_ALL, 0) && errno != EEXIST) {
        perror("mknod(FIFO_NAME)");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "FIFO_NAME was created\n",
          strlen("FIFO_NAME was created\n"));
    write(STDOUT_FILENO, "FIFO_NAME is opening\n",
          strlen("FIFO_NAME is opening\n"));

    fifo_fd = open(FIFO_NAME, O_WRONLY);

    if (-1 == fifo_fd) {
        perror("open(FIFO_NAME)");
        exit(EXIT_FAILURE);
    }

    // Configuracion de signals
    write(STDOUT_FILENO, "Configuring SIGNATIONS...\n",
          strlen("Configuring SIGNATIONS...\n"));

    sa.sa_handler = sigusr_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    ret = sigaction(SIGUSR1, &sa, NULL);
    if (ret == -1) {
        perror("Sigaction (SIGUSR1)");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = sigusr_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    ret = sigaction(SIGUSR2, &sa, NULL);
    if (ret == -1) {
        perror("Sigaction (SIGUSR2)");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = sig_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    ret = sigaction(SIGINT, &sa, NULL);
    if (ret == -1) {
        perror("Sigaction (SIGINT)");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = sig_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    ret = sigaction(SIGPIPE, &sa, NULL);
    if (ret == -1) {
        perror("Sigaction (SIGPIPE)");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO,
          "Waiting for DATA (20 characters max), SIGUSR1 or SIGUSR2...\n",
          strlen(
              "Waiting for DATA (20 characters max), SIGUSR1 or SIGUSR2...\n"));

    while (1) {

        bzero(msg, sizeof(msg));
        bzero(aux, sizeof(aux));
        ret = read(STDIN_FILENO, msg, (size_t)FIFO_MSG_SIZE - 1);

        // Manejo de errores de read
        if (-1 == ret) {
            perror("read(STDIN_FILENO)");
            exit(EXIT_FAILURE);
        } else if (0 == ret) {

            write(STDOUT_FILENO, "\nWriter: -EOF- . Closing FIFO_NAME...\n",
                  strlen("\nWriter: -EOF- . Closing FIFO_NAME...\n"));
            ret = close(fifo_fd);

            if (0 == ret) {
                write(STDOUT_FILENO, "Writer: FIFO_NAME close...\n",
                      strlen("Writer: FIFO_NAME close...\n"));
                exit(EXIT_SUCCESS);
            } else {
                perror("close(FIFO_NAME)");
                exit(EXIT_FAILURE);
            }

        } else {

            if ((FIFO_MSG_SIZE - 1) > ret) {
                ret--;
            }
            msg[ret] = '\0';
            /*Con esto eliminamos el \n al reemplazarlo por \0 entonces
            solo envía la información necesaria, evitando salto de linea en el
            otro priceso*/

            // Escritura del writer en la fifo
            strcat(aux, data_header);
            strcat(aux, msg);
            ret = write(fifo_fd, aux, strlen(aux));

            if (-1 == ret) {
                perror("write(FIFO_NAME)");
                exit(EXIT_FAILURE);
            } else {
                printf("-->>Writer: %ld bytes wroten in FIFO_NAME: \"%s\"\n",
                       ret, aux);
            }
        }
    }
    return 0;
}

// 12345678901234567890123456789012345
// kill -SIGUSR1 24399
// kill -SIGUSR2 24399