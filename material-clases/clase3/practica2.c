#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

        /*Comportamiento de waitpid

waitpid(-1, NULL, WNOHANG): Aquí, el proceso padre espera a cualquier proceso hijo (-1), 
no guarda el estado de terminación (NULL), y no bloquea si no hay hijos terminados (WNOHANG).

Retorno de waitpid:

> 0: Retorna el PID del proceso hijo terminado. Esto significa que hay al menos un proceso hijo que ha terminado.

0: No hay procesos hijos que hayan terminado.

-1: Error (por ejemplo, si no hay procesos hijos o si ocurre algún otro error). 
errno se establece para indicar el error específico.

while (waitpid(-1, NULL, WNOHANG) > 0)

Bucle while: El bucle continúa ejecutándose mientras waitpid retorne un valor mayor que 0, lo que indica que hay 
procesos hijos que han terminado y han sido "recolectados". waitpid en bucle: Llamar a waitpid en un bucle permite 
limpiar múltiples procesos hijos que podrían haber terminado casi simultáneamente. Cada llamada a waitpid con WNOHANG 
"recolecta" un proceso hijo terminado sin bloquear, y el bucle se detiene cuando no hay más procesos hijos que hayan 
terminado (cuando waitpid retorna 0).

*/




// Handler para la señal SIGCHLD
void handle_sigchld(int sig) {
    (void)sig;  // Para evitar la advertencia del compilador sobre el parámetro no usado
    static int x = 1;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        // Limpiar los procesos hijos terminados
        printf("Termino hijo %d\n",x++); // Este mensaje se muestra una vez nada más, porque
        //a pesar de que llegan 2 SIGCHLD más, waitpid NO devuelve >0, entonces NO imprime
    }
}

int main() {
    struct sigaction sa;

    // Configurar el handler para SIGCHLD
    sa.sa_handler = handle_sigchld;
    sigemptyset(&sa.sa_mask); //ponemos todo en cero
    sa.sa_flags = 0;  // Puedes probar con SA_RESTART aquí
    //Vemos que signation devuelve un entero, acá le avisamos al sistema que sa tiene que tratarla
    //para el caso de la señal SIGCHLD. NULL quiere decir default
    if (sigaction(SIGCHLD, &sa, NULL) == -1) { 
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Proceso hijo
        printf("Hijo: durmiendo por 5 segundos\n");
        sleep(5);
        printf("Hijo: terminando\n");
        exit(EXIT_SUCCESS);
    } else {
        // Proceso padre
        printf("Padre: esperando el evento de señal %d\n",getpid());
        
        // Probar la reacción a SIGCHLD con read()
        char buffer[10];
        printf("Padre: esperando entrada (read)\n");
        if (read(STDIN_FILENO, buffer, sizeof(buffer)) == -1) {
            perror("read");
        } else {
            printf("Padre: entrada recibida\n");
        }

        // Probar la reacción a SIGCHLD con sleep()
        printf("Padre: durmiendo por 10 segundos (sleep)\n");
        if (sleep(10) != 0) {
            printf("Padre: sueño interrumpido\n");
        } else {
            printf("Padre: sueño completo\n");
        }

        // Probar la reacción a SIGCHLD con pause()
        printf("Padre: esperando señal (pause)\n");
        pause();
        printf("Padre: señal recibida\n");

        printf("Padre: terminando\n");
    }

    return 0;
}

/*

        SIN SA_RESTART

Padre: esperando el evento de señal 42997 //Este PID usamos para mandarle kill -SIGCHLD 42997 en sleep y pause
Padre: esperando entrada (read)
Hijo: durmiendo por 5 segundos
Hijo: terminando
read: Interrupted system call
Padre: durmiendo por 10 segundos (sleep)
Padre: sueño interrumpido                  //Vemos que sleep termino con algo !0, o sea, fue interrumpido, no 
                                            //genera interrupted system call
Padre: esperando señal (pause)              //Pause pasa de largo si generar interrupted system call (EINTR)
Padre: señal recibida
Padre: terminando

--------------------------------------------------------

        CON SA_RESTART

Padre: esperando el evento de señal 43739
Padre: esperando entrada (read)
Hijo: durmiendo por 5 segundos
Hijo: terminando
->                                          //Llegó la señal de que termino el hijo pero SA_RESTART hace que read
                                            //no se interrumpa o sea reentrante, por lo que no aparece EINTR
abcdef                                     //Ingresamos algo para que pase el read y mandamos señales a mano 
Padre: entrada recibida
Padre: durmiendo por 10 segundos (sleep)
Padre: sueño interrumpido
Padre: esperando señal (pause)
Padre: señal recibida
Padre: terminando

*/