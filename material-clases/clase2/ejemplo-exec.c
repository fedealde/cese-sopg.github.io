
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)// los parametros se reciben en un arreglo de cadenas, el primer arreglo es el nombre, no nos sirve pero es por convencion
{
    pid_t pid;
    switch (pid = fork())
    {
        case -1:
            perror("fork"); /* something went wrong */
            exit(1);
            break;

        case 0:
            printf(" CHILD: This is the child process!\n");
            printf(" CHILD: My PID is %d\n", getpid());
            printf(" CHILD: My parent's PID is %d\n", getppid());
            printf(" CHILD: Calling exec...\n ");
            char* const args[] = {"/bin/ls","-lah",NULL};// El primero es solo el nombre, no el programa en sí, podemos enviar cualquier cosa por que al final no lo usa, el otro si lo usa, se pasa null al final para avisar que termino
            execv("/bin/ls", args);//cargo en memoria el .out de ls y lo ejecuta
            printf(" CHILD: I'm outta here!\n");// esto ya no se ejecuta
            exit(0);//esto tampoco ejecuta
            break;

        default:
            printf("PARENT: This is the parent process!\n");
            printf("PARENT: My PID is %d\n", getpid());
            printf("PARENT: My childs PID is %d\n", pid);// a pesar que cambie el codigo, ls es
            int ret;
            wait(&ret);
            printf("PARENT: My child's exit status is: %d\n", WEXITSTATUS(ret));// vemos que ls devuelve 0, sigue teniendo al hijo aunque no sea el mismo codigo.
            printf("PARENT: I'm outta here!\n");
    }
}
