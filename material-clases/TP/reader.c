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
#define FILE_NAME_DATA "log.txt"
#define FILE_NAME_SIGN "signals.txt"
#define FIFO_MSG_SIZE 22
#define FIFO_MSG_TO_SEND_SIZE 27
#define FIFO_SIGN_TO_SEND_SIZE 7
#define _RW_ALL 0666

//Constantes y variables globales
const char *data_header = "DATA: ";
const char *sign_header = "SIGN: ";
const char *sign_usr1 = "1\0";
const char *sign_usr2 = "2\0";
int fifo_fd, sign_fd, log_fd;

//Manejador de señales
void sig_handler(int sig) {

    switch (sig) {
    case SIGINT:
        write(1, "\n\nCtrl+C pressed. SIGINT handler executing...\n",
              strlen("\n\nCtrl+C pressed. SIGINT handler executing...\n"));
        exit(EXIT_FAILURE);
        break;

    default:
        write(1, "\nANOTHER SIGNAL\n", strlen("\nANOTHER SIGNAL\n"));
        exit(EXIT_FAILURE);
        break;
    }
}

//Parseador de mensajes para ser escritos en log.txt o signals.txt
void check_and_write_msg(const char *msg) {

    char aux[FIFO_MSG_SIZE] = {0};
    ssize_t ret;

    if (strncmp(msg, data_header, strlen(data_header)) == 0) {

        strcpy(aux, msg + strlen(data_header));

        aux[(uint8_t)strlen(aux)] = '\n';
        aux[(uint8_t)strlen(aux)+1] = '\0';

        ret = write(log_fd, aux, strlen(aux));

        if (-1 == ret) {
            perror("write(FILE_NAME_DATA)");
            exit(EXIT_FAILURE);
        } else {
            aux[(uint8_t)strlen(aux)-1] = '\0'; //Quito \n para que no salte de linea en el printf
            printf("-->>Reader: %ld bytes (counting '\\n') wroten in FILE_NAME_DATA: \"%s\"\n",
                   ret, aux);
        }

    } else if (strncmp(msg, sign_header, strlen(sign_header)) == 0) {

        strcpy(aux, msg + strlen(sign_header));

        aux[(uint8_t)strlen(aux)] = '\n';
        aux[(uint8_t)strlen(aux)+1] = '\0';

        ret = write(sign_fd, aux, strlen(aux));

        if (-1 == ret) {
            perror("write(FILE_NAME_SIGN)");
            exit(EXIT_FAILURE);
        } else {
            aux[(uint8_t)strlen(aux)-1] = '\0'; //Quito \n para que no salte de linea en el printf
            printf("-->>Reader: %ld bytes (counting '\\n') wroten in FILE_NAME_SIGN: \"%s\"\n",
                   ret, aux);
        }

    } else {

        write(STDOUT_FILENO,
              "From check_and_write_msg: MSG doesn't match headers\n",
              strlen("From check_and_write_msg: MSG doesn't match headers\n"));
    }
}

int main(void) {

    char msg[FIFO_MSG_TO_SEND_SIZE] = {0};

    ssize_t ret;
    struct sigaction sa;

//Presentacion del proceso
    printf("Reader is running. PID: %d\n", getpid());
    write(STDOUT_FILENO, "Creating FIFO_NAME...\n",
          strlen("Creating FIFO_NAME...\n"));

    if (-1 == mknod(FIFO_NAME, S_IFIFO | _RW_ALL, 0) && errno != EEXIST) {
        perror("mknod(FIFO_NAME)");
        exit(EXIT_FAILURE);
    }

//Creacion y apertura de la fifo
    write(STDOUT_FILENO, "FIFO_NAME was created\n",
          strlen("FIFO_NAME was created\n"));
    write(STDOUT_FILENO, "FIFO_NAME is opening\n",
          strlen("FIFO_NAME is opening\n"));

    fifo_fd = open(FIFO_NAME, O_RDONLY);

    if (-1 == fifo_fd) {
        perror("open(FIFO_NAME)");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "Creating FILE_NAME_DATA...\n",
          strlen("Creating FILE_NAME_DATA...\n"));

    if (-1 == mknod(FILE_NAME_DATA, S_IFREG | _RW_ALL, 0) && errno != EEXIST) {
        perror("mknod(FILE_NAME_DATA)");
        exit(EXIT_FAILURE);
    }

//Creacion y apertura del archivo log.txt
    write(STDOUT_FILENO, "FILE_NAME_DATA was created\n",
          strlen("FILE_NAME_DATA was created\n"));
    write(STDOUT_FILENO, "FILE_NAME_DATA is opening\n",
          strlen("FILE_NAME_DATA is opening\n"));

    log_fd = open(FILE_NAME_DATA, O_WRONLY);

    if (-1 == log_fd && errno != EEXIST) {
        perror("open(FILE_NAME_DATA)");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "Creating FILE_NAME_SIGN...\n",
          strlen("Creating FILE_NAME_SIGN...\n"));

    if (-1 == mknod(FILE_NAME_SIGN, S_IFREG | _RW_ALL, 0) && errno != EEXIST) {
        perror("mknod(FILE_NAME_SIGN)");
        exit(EXIT_FAILURE);
    }

//Creacion y apertura del archivo signals.txt
    write(STDOUT_FILENO, "FILE_NAME_SIGN was created\n",
          strlen("FILE_NAME_SIGN was created\n"));
    write(STDOUT_FILENO, "FILE_NAME_SIGN is opening\n",
          strlen("FILE_NAME_SIGN is opening\n"));

    sign_fd = open(FILE_NAME_SIGN, O_WRONLY);

    if (-1 == sign_fd && errno != EEXIST) {
        perror("open(FILE_NAME_SIGN)");
        exit(EXIT_FAILURE);
    }

//Configuracion de signals
    write(STDOUT_FILENO, "Configuring SIGNATIONS...\n",
          strlen("Configuring SIGNATIONS...\n"));

    sa.sa_handler = sig_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    ret = sigaction(SIGINT, &sa, NULL);
    if (ret == -1) {
        perror("Sigaction (SIGINT)");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "Waiting for INFO...\n",
          strlen("Waiting for INFO...\n"));

    while (1) {

//Recepcion de mensajes de la fifo y manejo de errores
        bzero(msg, sizeof(msg));
        ret = read(fifo_fd, msg, (size_t)FIFO_MSG_TO_SEND_SIZE - 1);

        if (-1 == ret) {
            perror("read(fifo_fd)");
            exit(EXIT_FAILURE);
        } else if (0 == ret) {

            write(STDOUT_FILENO, "\nReader: -EOF- . Closing FIFO_NAME...\n",
                  strlen("\nReader: -EOF- . Closing FIFO_NAME...\n"));
            ret = close(fifo_fd);

            if (0 == ret) {
                write(STDOUT_FILENO, "Reader: FIFO_NAME close...\n",
                      strlen("Reader: FIFO_NAME close...\n"));
                exit(EXIT_SUCCESS);
            } else {
                perror("close(FIFO_NAME)");
                exit(EXIT_FAILURE);
            }

        } else {

//Presentacion del mensaje recibido y envío del mismo al parseador
            printf("-->>Reader: %ld bytes read in FIFO_NAME: \"%s\"\n", ret,
                   msg);

            check_and_write_msg(msg);
        }
    }

    return 0;
}
