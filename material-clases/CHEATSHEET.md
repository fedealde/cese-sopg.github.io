# Procesos, compilación y señales

-   Compila `ejemplo-exec.c` mostrando warnings y el ejecutable lo guarda en `ejemplo-exec`: `gcc -Wall -pedantic ejemplo-exec.c -o ejemplo-exec.out `
-   Muestra el pid de nuestro programa:`ps -elf | grep a.out` con `a.out` nuestro binario ejecutable
-   Ejecutar: `./a.out` si tiene el `.out`, sino, se coloca directamente `./nombre`
-   Contar caracteres: `wc`
-   Saber el resultado del exit de una ejecusión de un proceso: `echo $?`
-   Fork: `fork()`devuelve PID del hijo al padre y 0 en el caso del hijo. -1 para cuando da error
-   `perror("linea/funcion");` : Muestra por salida estandar de error el error decodificado devuelto por la funcion anterior.
-   
-   
-   
-   
-   
-   
-   
-   
- 

```
    int sigaction(int signum, const struct sigaction *_Nullable restrict act, struct sigaction *_Nullable restrict oldact);
```
    struct sigaction {
        void     (*sa_handler)(int);
        void     (*sa_sigaction)(int, siginfo_t *, void *);
        sigset_t   sa_mask;
        int        sa_flags;
        void     (*sa_restorer)(void);
    };
```

sa_handler specifies the action to be associated with signum and can be one of the following:

```
       •  SIG_DFL for the default action.

       •  SIG_IGN to ignore this signal.

       •  A pointer to a signal handling function.  This function
          receives the signal number as its only argument.

sa_mask specifies a mask of signals which should be blocked

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset); -> sigprocmask(SIG_BLOCK, &set, NULL);
sigemptyset(&sa.sa_mask); ->    sigset_t set;      sigemptyset(&set);

* SIG_BLOCK: hace una unión con la máscara proporcionada (para bloquear las
señales indicadas)
*  SIG_UNBLOCK: elimina las señales indicadas de la máscara (para
desbloquearlas)
*  SIG_SETMASK: sobreescribe la máscara completa

int sigwait(const sigset_t *set, int *sig); Devuelve 0 si esta todo ok. Determino ante que señal/es del set actuo y devuelvo en *sig cuál fue la señal que me hizo despertar.

    sigemptyset(&set); // pongo todo en cero
    sigaddset(&set, SIGUSR1); // adjunto SIGUSER1
    sigprocmask(SIG_BLOCK, &set, NULL); // Aplico bloqueo a toda la máscara


Bloquear y desbloquear señales en un proceso, en este caso SIGINT (ctrl+c)
    sigemptyset(&set); 
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, NULL); // bloqueo sigint
    printf("Presionar ctrl+c !!!\n");
    sleep(5); // presionar CTRL-C aca! // mientras duermo sigint no actua
    sigprocmask(SIG_UNBLOCK, &set, NULL); // desbloqueo y actua de ahora en más