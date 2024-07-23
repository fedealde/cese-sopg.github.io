#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

//MANEJO DE SEÑALES SIGWAIT


// Handler para la señal SIGCHLD
// void handle_sigchld(int sig) {
//     (void)sig;  // Para evitar la advertencia del compilador sobre el parámetro no usado
//     while (waitpid(-1, NULL, WNOHANG) > 0) {
//         // Limpiar los procesos hijos terminados
//     }
// }

int main() {
 
    printf("pid: %d\n", getpid());
    sigset_t set; //Genero el set de señales
    sigemptyset(&set); // Las pongo en cero
    sigaddset(&set, SIGUSR1); // Añado la de interes
    sigprocmask(SIG_BLOCK, &set, NULL); // Bloqueo esta señal  kill -SIGUSR1 13663
    int sig, i = 0;

    while (i<3) {
        
        i++;
        printf("Espero sigusr1\n");
        int r = sigwait(&set, &sig); //Espero a la señal bloqueada
        printf("r = %d, sig = %d  i = %d\n", r, sig, i); // Imprimo al desbloquear

    }

    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, NULL); // bloqueo sigint
    printf("Presionar ctrl+c !!!\n");
    sleep(5); // presionar CTRL-C aca! // mientras duermo sigint no actua
    sigprocmask(SIG_UNBLOCK, &set, NULL); // desbloqueo y actua de ahora en más
    printf("salida normal\n");

    return 0;
}

