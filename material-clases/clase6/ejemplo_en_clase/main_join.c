#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *start_thread(void *message) {
    printf("%s\n", (const char *)message);
    // No es seguro par signals pero si para threads
    sleep(5);
    printf("finalizando thread%s\n", (const char *)message);
    return NULL;
}

int main(void) {
    const char *message1 = "Thing 1";
    const char *message2 = "Thing 2";

    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, start_thread, (void *)message1);//Si le pasaramos atributo como
    //un nombre, podriamos verlo en la lista, sino vemos el TID
    pthread_create(&thread2, NULL, start_thread, (void *)message2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    //pthread_detach(thread1); //Si utilizamos esto, a pesar de que esten en espera de 5 seg, mueren
    //pthread_detach(thread2); //Es dificil ver si va a ejecutarse en orden, o si alcanza a imprimir
    //Este detach permite que si el hilo termine, libere recursos

    printf("finalizando proceso\n"); //Este es el thread principal, si termina este, termina TODO
    //Tiene sentido utilizar join en vez de detach porque sino mata todo sin esperar

    return 0;
}
