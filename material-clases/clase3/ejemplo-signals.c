#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

//ps -elf | grep a.out

void handler(int sig) {
    /*Si ejecutamos una señal que no es segura como printf, puede generar problemas ya que no 
    son reentrantes*/
    write(1, "se presiono ctrl+c!!\n", 21);

}

int main(void) {
    struct sigaction sa;

    //Es la funcion a ejecutar al momento de que llegue la señal
    sa.sa_handler = handler;
    
    /* SA_RESTART hace que las funciones interrumpibles NO devuelvan error
    entonces evitamos que corrompa nuestro manejo de errores, entonces reinicia el error 
    y no devuelven  EINTR*/

    //sa.sa_flags = 0;  //Descomentamos esto y comentamos abajo para ver la diferencia
    sa.sa_flags = SA_RESTART; 

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
