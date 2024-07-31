# Procesos, compilación y señales

-   Compila `ejemplo-exec.c` mostrando warnings y el ejecutable lo guarda en `ejemplo-exec`: `gcc -Wall -pedantic ejemplo-exec.c -o ejemplo-exec.out `
-   Muestra el pid de nuestro programa:`ps -elf | grep a.out` con `a.out` nuestro binario ejecutable
-   Ejecutar: `./a.out` si tiene el `.out`, sino, se coloca directamente `./nombre`
-   Contar caracteres: `wc`
-   Saber el resultado del exit de una ejecusión de un proceso: `echo $?`
-   Fork: `fork()`devuelve PID del hijo al padre y 0 en el caso del hijo. -1 para cuando da error
-   `perror("linea/funcion");` : Muestra por salida estandar de error el error decodificado devuelto por la funcion anterior.
-   
-   bzero(buffer, sizeof(buffer));
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


1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
6) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	10) SIGUSR1
11) SIGSEGV	12) SIGUSR2	13) SIGPIPE	14) SIGALRM	15) SIGTERM
16) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
26) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
31) SIGSYS	34) SIGRTMIN	35) SIGRTMIN+1	36) SIGRTMIN+2	37) SIGRTMIN+3
38) SIGRTMIN+4	39) SIGRTMIN+5	40) SIGRTMIN+6	41) SIGRTMIN+7	42) SIGRTMIN+8
43) SIGRTMIN+9	44) SIGRTMIN+10	45) SIGRTMIN+11	46) SIGRTMIN+12	47) SIGRTMIN+13
48) SIGRTMIN+14	49) SIGRTMIN+15	50) SIGRTMAX-14	51) SIGRTMAX-13	52) SIGRTMAX-12
53) SIGRTMAX-11	54) SIGRTMAX-10	55) SIGRTMAX-9	56) SIGRTMAX-8	57) SIGRTMAX-7
58) SIGRTMAX-6	59) SIGRTMAX-5	60) SIGRTMAX-4	61) SIGRTMAX-3	62) SIGRTMAX-2
63) SIGRTMAX-1	64) SIGRTMAX	


kill -SIGUSR2 15918
