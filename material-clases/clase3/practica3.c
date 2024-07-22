#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>




// Handler para la señal SIGCHLD
// void handle_sigchld(int sig) {
//     (void)sig;  // Para evitar la advertencia del compilador sobre el parámetro no usado
//     while (waitpid(-1, NULL, WNOHANG) > 0) {
//         // Limpiar los procesos hijos terminados
//     }
// }

int main() {
    struct sigaction sa;
    int pipes[2];

    // Configurar el handler para SIGCHLD
    //sa.sa_handler = handle_sigchld;
    sa.sa_handler = SIG_IGN; //Le digo que ignore la señal ya que no quiero que haga nada
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;  // Puedes probar con SA_RESTART aquí
    if (sigaction(SIGPIPE, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if(pipe(pipes)!=0) {
    perror("error pipe");
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Proceso hijo

        close(pipes[0]); //Cierro lectura
        printf("Hijo: cerró fd lectura y estará durmiendo por 5 segundos\n");
        sleep(5);
        ssize_t w = write(pipes[1], "Msg child\n ", sizeof("Msg child\n "));
        if(n==-1){
            perror("write");
            exit(1);
        }else{
        sleep(10);
        printf("Hijo: terminando\n");
        exit(EXIT_SUCCESS);}


    } else {
        // Proceso padre
        close(pipes[1]); //Cierro escritura
        printf("Padre: cerró fd escritura y estará leyendo el pipe %d\n",getpid());
        
        // Probar la reacción a SIGCHLD con read()
        char buffer[10];
        printf("Padre: esperando entrada (read)\n");
        ssize_t ret = read(pipes[1], buffer, sizeof(buffer));
        if(ret > 0) {
            printf("PARENT: llego de hijo %dbytes: %s\n", ret, buffer);
            }else if (ret == -1) {
            perror("read");
                } else if (ret == 0)
                printf("PARENT: hay EOF!\n");
                exit(1);
      
    }

    return 0;
}

